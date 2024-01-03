import java.util.Set;
import java.util.ArrayList;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Collections;
import java.util.Comparator;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;


public class TokenCount_v3
{
    public static void main(String[] args) throws Exception {
        System.out.println("TokenCount_v3");
        if (args.length != 2) {
            System.out.println("usage: java TokenCount number-of-pages XML-file");
            System.exit(0);
        }
        
        // use ConcurrentHashMap instead of HashMap for synchronized accessing the token frequency map
        final ConcurrentHashMap<String, Integer> tokenFreq = new ConcurrentHashMap<String, Integer>();
        final int TopNum = 30;
        final int queueLength = 100;
        
        Integer numPages = Integer.parseInt(args[0]);
        String fileName = args[1];
        int processorCount = Runtime.getRuntime().availableProcessors();
        int consumerThreadCount = processorCount > 1 ? processorCount - 1 : 1;
    
        // print number of available processors
        System.out.println(processorCount + " available processors");
        
        // create shared queue
        ArrayBlockingQueue<Page> sharedQueue = new ArrayBlockingQueue<Page>(queueLength);
        
        // create thread pool
        ExecutorService pool = Executors.newCachedThreadPool();
        
        /*************** begin timed code ***********************/
        final long before = System.nanoTime();
        
        // create producer thread
        Thread parser = new Thread(new Parser(sharedQueue, fileName, numPages, consumerThreadCount));
        parser.start();
        
        // create N-1 consumer threads on an N-processor system
        Counter counter = new Counter(sharedQueue, tokenFreq);
        for (int i = 0; i < consumerThreadCount; ++i) {
            pool.submit(counter);
        }
        
        // wait until termination of threads
        try {
            parser.join();
            pool.shutdown();
            while (pool.awaitTermination(60, TimeUnit.SECONDS) == false);
            pool.shutdownNow();
        }
        catch (InterruptedException ex) {
            System.out.println(ex.getMessage());
        }
        
        final long after = System.nanoTime();
        /*************** end timed code ***********************/
        
        System.out.println("Time to process " + numPages + " pages = " + (after - before)/1000000 + " milliseconds");

        // sort tokenFreq by value & print top 30 most common tokens
        Set<Entry<String, Integer>> entries = tokenFreq.entrySet();
        ArrayList<Entry<String, Integer>> list = new ArrayList<Entry<String, Integer>>(entries);
        Collections.sort(list, new Comparator<Map.Entry<String, Integer>>()
                {
                  public int compare(Map.Entry<String, Integer> obj1, Map.Entry<String, Integer> obj2)
                  {
                      return (obj2.getValue()).compareTo(obj1.getValue());
                  }
                } );
        
        for(int i = 0; i < TopNum; ++i) {
            System.out.println(list.get(i).getKey() + " appears " + list.get(i).getValue() + " times");
        }
    }

    private static class Parser implements Runnable
    {
        private String fileName;
        private BlockingQueue<Page> sharedQueue;
        private int numPages;
        private int consumerThreadCount;
        
        public Parser(BlockingQueue<Page> sharedQueue, String fileName, int numPages, int consumerThreadCount) {
            this.fileName = fileName;
            this.sharedQueue = sharedQueue;
            this.numPages = numPages;
            this.consumerThreadCount = consumerThreadCount;
        }
        
        public void run() {
            try {
                // parses the XML and pushes pages onto a shared queue.
                Iterable<Page> allPages = new Pages(numPages, fileName);
                for (Page page: allPages) {
                    sharedQueue.put(page);
                }
                // push poison pill items to indicate that there are no more pages
                while (consumerThreadCount > 0){
                    sharedQueue.put(new PoisonPill());
                    --consumerThreadCount;
                }
            }
            catch (Exception ex) {
                System.out.println(ex.getMessage());
            }
        }
    }
    
    private static class Counter implements Runnable
    {
        private BlockingQueue<Page> sharedQueue;
        private ConcurrentHashMap<String, Integer> tokenFreq;
        
        public Counter(BlockingQueue<Page> sharedQueue, ConcurrentHashMap<String, Integer> tokenFreq) {
            this.sharedQueue = sharedQueue;
            this.tokenFreq = tokenFreq;
        }
        
        public void run() {
            try {
                while (true) {
                    Page page = sharedQueue.take();
                    // there are no more pages when get the poison pill item
                    if (page.isPoisonPill()) {
                        return;
                    }
                    
                    // on each page, find all tokens then increase the count for each token
                    Iterable<String> allTokens = new Words(page.getText());
                    for (String token: allTokens) {
                        countToken(token);
                    }
                }
            }
            catch (Exception ex) {
                System.out.println(ex.getMessage());
            }

        }
        
        private void countToken(String token) {
            while (true) {
                Integer currentCount = tokenFreq.get(token);
                if (currentCount == null) {
                    if (tokenFreq.putIfAbsent(token, 1) == null)
                        return;
                }
                else {
                    if (tokenFreq.replace(token, currentCount, currentCount + 1))
                        return;
                }
            }
        }
    }
}






