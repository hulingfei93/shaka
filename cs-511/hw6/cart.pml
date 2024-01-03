/*  carts  */
byte north_carts[5];
byte west_carts[2];
byte south_carts[1];
byte east_carts[2];

byte north_len;
byte west_len;
byte south_len;
byte east_len;

byte north_cur;
byte west_cur;
byte south_cur;
byte east_cur;

byte intersection;
byte results[10];
byte result_cur;

/* dir: 1 - north, 2 - west, 3 - south, 4 - east */
proctype begin(byte length, byte dir) {
    byte i = 0;
    
    do
    :: i == length -> break;
    :: else ->
        /* wait for cross */
        if
        :: dir == 1 -> 
            atomic{
                intersection = intersection + 1;
                assert(intersection == 1);
                results[result_cur] = north_carts[north_cur];
                north_cur = north_cur + 1;
                result_cur = result_cur + 1;
                i = i + 1;
                intersection = intersection - 1;
            }
        :: dir == 2 ->
            atomic{
                intersection = intersection + 1;
                assert(intersection == 1);
                results[result_cur] = west_carts[west_cur];
                west_cur = west_cur + 1;
                result_cur = result_cur + 1;
                i = i + 1;
                intersection = intersection - 1;
            }
        :: dir == 3 ->
            atomic{
                intersection = intersection + 1;
                assert(intersection == 1);
                results[result_cur] = south_carts[south_cur];
                south_cur = south_cur + 1;
                result_cur = result_cur + 1;
                i = i + 1;
                intersection = intersection - 1;
            }
        :: else ->
            atomic{ 
                intersection = intersection + 1;
                assert(intersection == 1);
                results[result_cur] = east_carts[east_cur];
                east_cur = east_cur + 1;
                result_cur = result_cur + 1;
                i = i + 1;
                intersection = intersection - 1;
            }
        fi
    od
}


init {
    north_carts[0] = 1;
    north_carts[1] = 5;
    north_carts[2] = 8;
    north_carts[3] = 9;
    north_carts[4] = 10;
    west_carts[0] = 2;
    west_carts[1] = 6;
    south_carts[0] = 3;
    east_carts[0] = 4;
    east_carts[1] = 7;
    
    north_len = 5;
    west_len = 2;
    south_len = 1;
    east_len = 2;
    
    north_cur = 0;
    west_cur = 0;
    south_cur = 0;
    east_cur = 0;
    
    intersection = 0;
    result_cur = 0;
    
    atomic {
        run begin(north_len, 1)  ;
        run begin(west_len, 2)  ;
        run begin(south_len, 3)  ;
        run begin(east_len, 4)
    }
    (_nr_pr == 1) -> printf("at end of simulation\n");
    
    assert(result_cur == 10);
    
    byte k = 0;
    do
    :: k == 10 -> break;
    :: else ->
        printf("%d ", results[k]);
        k = k + 1;
    od;
    printf("\n");
    printf("simulation passed assertion!\n");
}
