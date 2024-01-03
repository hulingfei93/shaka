#include <iostream>
#include <vector>
#include <sys/timeb.h>

#include "RandomNumberGenerator.h"
#include "BinarySearchTree.h"
#include "PriorityQueue.h"

using namespace std;

long long getSystemTime();

void sort_test()
{
    const int loopCount = 2000;
    const int randomNumberCount = 1000;
    long long sumOfBST = 0; // milliseconds
    long long sumOfPQ = 0; // milliseconds
    long long startTime, endTime;
    RandomNumberGenerator rng;

    for (int i = 1; i <= loopCount; ++i)
    {
        // generator random numbers
        vector<int> vec;
        for (int j = 1; j <= randomNumberCount; ++j)
        {
            vec.push_back(rng.GenerateInteger(0, 999));
        }

        // insert and sort based on BST
        BinarySearchTree BST;
        vector<int>::iterator iter;
        startTime = getSystemTime();
        for (iter = vec.begin(); iter != vec.end(); ++iter)
        {
            BST.insert(*iter);
        }
        while (!BST.isEmpty())
        {
            //cout << BST.deleteMin() << "\t";
            BST.deleteMin();
        }
        endTime = getSystemTime();
        sumOfBST += (endTime - startTime);
        //cout << endl;
        cout << "BST: " << i << " of " << loopCount << " times finished." << endl;


        // insert and sort based on PQ
        PriorityQueue PQ(randomNumberCount);
        startTime = getSystemTime();
        for (iter = vec.begin(); iter != vec.end(); ++iter)
        {
            PQ.insert(*iter);
        }
        while (!PQ.isEmpty())
        {
            //cout << PQ.deleteMin() << "\t";
            PQ.deleteMin();
        }
        endTime = getSystemTime();
        sumOfPQ += (endTime - startTime);
        //cout << endl;
        cout << "QT: " << i << " of " << loopCount << " times finished." << endl;
    }

    // summary
    cout << "************Summary************" << endl;
    cout << "Test times: " << loopCount << endl;
    cout << "Random number count: " << randomNumberCount << endl;
    cout << "BST sum of CPU execution time: " << (double)sumOfBST / 1000 << " seconds." << endl;
    cout << "BST avg of CPU execution time: " << (double)sumOfBST / 1000 / loopCount << " seconds." << endl;
    cout << "PQ sum of CPU execution time: " << (double)sumOfPQ / 1000 << " seconds." << endl;
    cout << "PQ avg of CPU execution time: " << (double)sumOfPQ / 1000 / loopCount << " seconds." << endl;
}

long long getSystemTime()
{
    struct timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
}
