#include <core.h>

void swap(int *p1, int *p2) {
    
    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

/*****************************************************
 *
 * Rotate Array
 *
 * Rotate an array of n elements to the right by k steps.
 *
 * For example, with n = 7 and k = 3, the array [1,2,3,4,5,6,7] is rotated to [5,6,7,1,2,3,4].
 *
*****************************************************/
void rotate_wrap(int *nums, int i, int j) {
    
    int temp;

    while (i < j) {
        temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;

        ++i;
        --j;
    }
}

void rotate(int *nums, int nums_size, int k) {
    
    k %= nums_size;

    rotate_wrap(nums, 0, nums_size - k - 1);
    rotate_wrap(nums, nums_size - k, nums_size - 1);
    rotate_wrap(nums, 0, nums_size - 1);
}

/*****************************************************
 *
 * Remove Element
 *
 * Given an array and a value, remove all instances of that value in place and return the new length.
 *
 * The order of elements can be changed. It doesn't matter what you leave beyond the new length.'
 *
*****************************************************/
int remove_element(int *nums, int num_size, int val) {
    
    int i = 0, j = 0;

    while (j < num_size){
        if (nums[j] != val) {
            if (i != j) {
                nums[i++] = nums[j];
            }
            else {
                ++i;
            }
        }
        ++j;
    }

    return i;
}

/*****************************************************
 *
 * Remove Duplicates from Sorted Array
 *
 * Given a sorted array, remove the duplicates in place 
 * such that each element appear only once and return the new length.
 *
 * Do not allocate extra space for another array, 
 * you must do this in place with constant memory.
 *
 * For example,
 * Given input array nums = [1,1,2],
 *
 * Your function should return length = 2, with the 
 * first two elements of nums being 1 and 2 respectively. 
 * It doesn't matter what you leave beyond the new length.'
 *
*****************************************************/
int remove_duplicates(int *nums, int num_size) {
    
    if (num_size < 2) {
        return num_size;
    }

    int i = 0, j = 1;

    while (j < num_size) {
        if (nums[j] != nums[i]) {
            if (++i != j) {
                nums[i] = nums[j];
            }   
        }
        ++j;
    }

    return i + 1;
}

/*****************************************************
 *
 * Remove Duplicates from Sorted Array II
 *
 * Follow up for "Remove Duplicates":
 * What if duplicates are allowed at most twice?
 *
 * For example,
 * Given sorted array nums = [1,1,1,2,2,3],
 *
 * Your function should return length = 5, with the 
 * first five elements of nums being 1, 1, 2, 2 and 3. 
 * It doesn't matter what you leave beyond the new length.
 *
*****************************************************/
int remove_duplicates_2(int *nums, int nums_size) {
    
    if (nums_size < 3) {
        return nums_size;
    }

    int i = 1, j;

    for (j = 2; j < nums_size; ++j) {
        if (nums[j] != nums[i]) {
            if (++i != j) {
                nums[i] = nums[j];
            }
        }
        else {
            if (nums[j] != nums[i - 1]) {
                nums[++i] = nums[j];
            }
        }
    }

    return i + 1;
}

/*****************************************************
 *
 * Majority Element
 *
 * Given an array of size n, find the majority element. 
 * The majority element is the element that appears more than . n/2 . times.
 *
 * You may assume that the array is non-empty and the majority element always exist in the array.
 *
*****************************************************/
int majority_element(int *nums, int nums_size) {
    
    int majority, majority_count, i;

    for (i = 0, majority_count = 0; i < nums_size; ++i) {
        if (majority_count == 0) {
            majority = nums[i];
            majority_count = 1;
        }
        else
        {
            if (nums[i] == majority) {
                ++majority_count;
            }
            else {
                --majority_count;
            }
        }
    }

    return majority;
}

/*****************************************************
 *
 * Majority Element II
 *
 * Given an integer array of size n, find all elements 
 * that appear more than . n/3 . times. The algorithm 
 * should run in linear time and in O(1) space.
 *
*****************************************************/
int
majority_element_2_count(int *nums, int nums_size, int target) {
    
    int i, count = 0;

    for (i = 0; i < nums_size; ++i) {
        if (nums[i] == target) {
            ++count;
        }
    }

    return count;
}

int *
majority_element_2(int *nums, int nums_size, int *return_size) {
    
    if (nums_size <= 0) {
        return NULL;
    }

    int i;
    int count1, count2, candidate1, candidate2;

    for (i = count1 = count2 = 0; i < nums_size; ++i) {
        if (count1 > 0 && count2 > 0) {
            if (nums[i] == candidate1) {
                ++count1;
            }
            else if (nums[i] == candidate2) {
                ++count2;
            }
            else {
                --count1;
                --count2;
            }
        }
        else if (count1 == 0 && count2 > 0) {
            if (nums[i] == candidate2) {
                ++count2;
            }
            else {
                candidate1 = nums[i];
                count1 = 1;
            }
        }
        else if (count1 > 0 && count2 == 0) {
            if (nums[i] == candidate1) {
                ++count1;
            }
            else {
                candidate2 = nums[i];
                count2 = 1;
            }
        }
        else {
            candidate1 = nums[i];
            count1 = 1;
        }
    }

    int *result = (int *)malloc(sizeof(int) * 2);
    i = 0;
    *return_size = 0;
    
    if (count1 > 0) {
        count1 = majority_element_2_count(nums, nums_size, candidate1);
        if (count1 > nums_size / 3) {
            result[i++] = candidate1;
            ++*return_size;
        }
    }
    
    if (count2 > 0) {
        count2 = majority_element_2_count(nums, nums_size, candidate2);
        if (count2 > nums_size / 3) {
            result[i++] = candidate2;
            ++*return_size;
        }
    }

    return result;
}

/*****************************************************
 *
 * First Missing Positive
 *
 * Given an unsorted integer array, find the first missing positive integer.
 *
 * For example,
 * Given [1,2,0] return 3,
 * and [3,4,-1,1] return 2.
 *
 * Your algorithm should run in O(n) time and uses constant space.
 *
*****************************************************/
int 
first_missing_positive(int *nums, int nums_size) {
    
    int i;
    int temp;

    for (i = 0; i < nums_size; ++i) {
        while (nums[i] > 0 && nums[i] <= nums_size && nums[nums[i] - 1] != nums[i]) {
            temp = nums[nums[i] - 1];
            nums[nums[i] - 1] = nums[i];
            nums[i] = temp;
        }
    }

    for (i = 0; i < nums_size; ++i) {
        if (i + 1 != nums[i]) {
            return i + 1;
        }
    }

    return nums_size + 1;
}

/*****************************************************
 *
 * Merge Sorted Array
 *
 * Given two sorted integer arrays nums1 and nums2, merge nums2 into nums1 as one sorted array.
 *
 * Note:
 * You may assume that nums1 has enough space (size that is greater or equal to m + n) 
 * to hold additional elements from nums2. The number of elements 
 * initialized in nums1 and nums2 are m and n respectively.
 *
*****************************************************/
void merge(int *nums1, int m, int *nums2, int n) {
    
    int c1, c2, c;

    c1 = m - 1;
    c2 = n - 1;
    c = m + n - 1;

    while (c1 >= 0 && c2 >= 0) {
        if (nums1[c1] > nums2[c2]) {
            nums1[c--] = nums1[c1--];
        }
        else if (nums1[c1] < nums2[c2]) {
            nums1[c--] = nums2[c2--];
        }
        else {
            nums1[c--] = nums1[c1--];
            nums1[c--] = nums2[c2--];
        }
    }

    while (c2 >= 0) {
        nums1[c--] = nums2[c2--];
    }
}


/*****************************************************
 *
 * Spiral Matrix
 *
 * Given a matrix of m x n elements (m rows, n columns), return all elements of the matrix in spiral order.
 *
 * For example,
 * Given the following matrix:
 *
 * [
 *  [ 1, 2, 3 ],
 *  [ 4, 5, 6 ],
 *  [ 7, 8, 9 ]
 * ]
 *
 * You should return [1,2,3,6,9,8,7,4,5].
 *
*****************************************************/
int *spiral_order(int **matrix, int matrix_row_size, int matrix_col_size) {
    
    if (matrix_row_size == 0 || matrix_col_size == 0) {
        return NULL;
    }

    int *spiral_matrix = (int *)malloc(sizeof(int) * matrix_row_size * matrix_col_size);
    int i = 0, j = 0, cur = 0;
    int min_row = 0, max_row = matrix_row_size - 1, min_col = 0, max_col = matrix_col_size - 1;

    /* direction: right -- 0, down -- 1, left -- 2, up -- 3 */
    unsigned char direction = 0;

    while (1) {
        if (direction == 0) {           // right
            while (j <= max_col) {
                spiral_matrix[cur++] = matrix[i][j++];      
            }
            j = max_col;
            ++min_row;

            // no more element
            if (i + 1 > max_row) {      
                break;
            }

            // change to down direction
            ++i;
            direction = 1;


        }
        if (direction == 1) {       // down
            while (i <= max_row) {
                spiral_matrix[cur++] = matrix[i++][j];  
            }
            i = max_row;
            --max_col;

            // no more element
            if (j - 1 < min_col) {      
                break;
            }

            // change to left direction
            --j;
            direction = 2;
        }
        if (direction == 2) {       // left
            while (j >= min_col) {
                spiral_matrix[cur++] = matrix[i][j--];  
            }
            j = min_col;
            --max_row;

            // no more element
            if (i - 1 < min_row) {      
                break;
            }

            // change to up direction
            --i;
            direction = 3;
        }
        if (direction == 3) {                           // up
            while (i >= min_row) {
                spiral_matrix[cur++] = matrix[i--][j];  
            }
            i = min_row;
            ++min_col;

            // no more element
            if (j + 1 > max_col) {      
                break;
            }

            // change to up direction
            ++j;
            direction = 0;
        }
    }

    return spiral_matrix;
}


/*****************************************************
 *
 * Spiral Matrix II
 *
 * Given an integer n, generate a square matrix filled with elements from 1 to n2 in spiral order.
 *
 * For example,
 * Given n = 3,
 *
 * You should return the following matrix:
 * [
 *  [ 1, 2, 3 ],
 *  [ 8, 9, 4 ],
 *  [ 7, 6, 5 ]
 * ]
 *
*****************************************************/
int **generate_matrix(int n) {

    if (n <= 0) {
        return NULL;
    }

    int **matrix = (int **)malloc(sizeof(int *) * n);
    int m;
    for (m = 0; m < n; ++m) {
        matrix[m] = (int *)malloc(sizeof(int) * n);
    }

    int i = 0, j = 0, num = 1;
    int min_row = 0, max_row = n - 1, min_col = 0, max_col = n - 1;

    /* direction: right -- 0, down -- 1, left -- 2, up -- 3 */
    unsigned char direction = 0;

    while (1) {
        if (direction == 0) {           // right
            while (j <= max_col) {
                matrix[i][j++] = num++;
            }
            j = max_col;
            ++min_row;

            // no more element
            if (i + 1 > max_row) {      
                break;
            }

            // change to down direction
            ++i;
            direction = 1;


        }
        if (direction == 1) {       // down
            while (i <= max_row) {
                matrix[i++][j] = num++;
            }
            i = max_row;
            --max_col;

            // no more element
            if (j - 1 < min_col) {      
                break;
            }

            // change to left direction
            --j;
            direction = 2;
        }
        if (direction == 2) {       // left
            while (j >= min_col) {
                matrix[i][j--] = num++;
            }
            j = min_col;
            --max_row;

            // no more element
            if (i - 1 < min_row) {      
                break;
            }

            // change to up direction
            --i;
            direction = 3;
        }
        if (direction == 3) {                           // up
            while (i >= min_row) {
                matrix[i--][j] = num++;
            }
            i = min_row;
            ++min_col;

            // no more element
            if (j + 1 > max_col) {      
                break;
            }

            // change to up direction
            ++j;
            direction = 0;
        }
    }

    return matrix;
}

/*****************************************************
 *
 * Unique Paths
 *
 * A robot is located at the top-left corner of a m x n grid 
 * (marked 'Start' in the diagram below).
 * 
 * The robot can only move either down or right at any point in time. 
 * The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).
 *
 * How many possible unique paths are there?
 *
 * Note: m and n will be at most 100.
 *
*****************************************************/
int unique_paths(int m, int n) {
    
    if (m <= 0 || n <= 0) {
        return 0;
    }

    if (m < n) {
        return unique_paths(n, m);
    }

    int *buffer1 = (int *)malloc(sizeof(int) * n);
    int *buffer2 = (int *)malloc(sizeof(int) * n);

    int i, j;
    int *cur = buffer1, *last;

    for (i = 0; i < n; ++i) {
        cur[i] = 1;
    }

    for (i = 1; i < m; ++i) {
        /* swap buffers */
        if (cur == buffer1) {
            cur = buffer2;
            last = buffer1;
        }
        else {
            cur = buffer1;
            last = buffer2;
        }

        cur[0] = 1;
        for (j = 1; j < n; ++j) {
            cur[j] = cur[j - 1] + last[j];
        }
    }

    return cur[n - 1];
}

/*****************************************************
 *
 * Unique Paths II
 *
 * Follow up for "Unique Paths":
 *
 * Now consider if some obstacles are added to the grids. How many unique paths would there be?
 *
 * An obstacle and empty space is marked as 1 and 0 respectively in the grid.
 *
 * For example,
 * There is one obstacle in the middle of a 3x3 grid as illustrated below.
 *
 * [
 *   [0,0,0],
 *   [0,1,0],
 *   [0,0,0]
 * ]
 * The total number of unique paths is 2.
 *
 * Note: m and n will be at most 100.
 *
*****************************************************/
int unique_paths_with_obstacles(int **obstacle_grid, int obstacle_grid_row_size, int obstacle_grid_col_size) {
    
    int m = obstacle_grid_row_size, n = obstacle_grid_col_size;

    if (m <= 0 || n <= 0) {
        return 0;
    }

    int *buffer1 = (int *)malloc(sizeof(int) * n);
    int *buffer2 = (int *)malloc(sizeof(int) * n);

    int i, j;
    int *cur = buffer1, *last;
    int **grid = obstacle_grid;

    cur[0] = grid[0][0] == 0 ? 1 : 0;
    for (i = 1; i < n; ++i) {
        cur[i] = grid[0][i] == 0 ? cur[i - 1] : 0;
    }

    for (i = 1; i < m; ++i) {
        /* swap buffers */
        if (cur == buffer1) {
            cur = buffer2;
            last = buffer1;
        }
        else {
            cur = buffer1;
            last = buffer2;
        }

        cur[0] = grid[i][0] == 0 ? last[0] : 0;
        for (j = 1; j < n; ++j) {
            cur[j] = grid[i][j] == 0 ? cur[j - 1] + last[j] : 0;
        }
    }

    return cur[n - 1];
}

/*****************************************************
 *
 * Product of Array Except Self
 *
 * Given an array of n integers where n > 1, nums, 
 * return an array output such that output[i] is equal to 
 * the product of all the elements of nums except nums[i].
 *
 * Solve it without division and in O(n).
 *
 * For example, given [1,2,3,4], return [24,12,8,6].
 *
 * Follow up:
 * Could you solve it with constant space complexity? 
 * (Note: The output array does not count as extra space 
 * for the purpose of space complexity analysis.)
 *
*****************************************************/
int *product_except_self(int *nums, int nums_size, int *return_size) {
    
    if (nums_size < 2) {
        return NULL;
    }

    int i, temp = 1;
    int *results = (int *)malloc(sizeof(int) * nums_size);

    results[0] = 1;

    for (i = 1; i < nums_size; ++i) {
        temp *= nums[i - 1];
        results[i] = temp;
    }

    temp = 1;
    for (i = nums_size - 2; i >= 0; --i) {
        temp *= nums[i + 1];
        results[i] *= temp;
    }

    *return_size = nums_size;
    return results;
}

/*****************************************************
 *
 * Minimum Path Sum
 *
 * Given a m x n grid filled with non-negative numbers, 
 * find a path from top left to bottom right 
 * which minimizes the sum of all numbers along its path.
 *
 * Note: You can only move either down or right at any point in time.
 *
*****************************************************/
int min_path_sum(int **grid, int grid_row_size, int grid_col_size) {

    int m = grid_row_size, n = grid_col_size;
    
    if (m <= 0 || n <= 0) {
        return 0;
    }

    int *buffer1 = (int *)malloc(sizeof(int) * n);
    int *buffer2 = (int *)malloc(sizeof(int) * n);

    int *cur = buffer1, *prev = buffer2;
    int i, j;
    int min_sum;

    cur[0] = grid[0][0];
    for (i = 1; i < n; ++i) {
        cur[i] = cur[i - 1] + grid[0][i];
    }

    for (i = 1; i < m; ++i) {
        /* swap the buffers */
        if (cur == buffer1) {
            cur = buffer2;
            prev = buffer1;
        }
        else {
            cur = buffer1;
            prev = buffer2;
        }

        cur[0] = prev[0] + grid[i][0];
        for (j = 1; j < n; ++j) {
            cur[j] = (cur[j - 1] < prev[j] ? cur[j - 1] : prev[j]) + grid[i][j];
        }
    }

    min_sum = cur[n - 1];

    free(buffer1);
    free(buffer2);

    return min_sum;
}

/*****************************************************
 *
 * Maximum Product Subarray
 *
 * Find the contiguous subarray within an array 
 * (containing at least one number) which has the largest product.
 *
 * For example, given the array [2,3,-2,4],
 * the contiguous subarray [2,3] has the largest product = 6.
 *
*****************************************************/
int max_product(int *nums, int nums_size) {
    
    if (nums_size <= 0) {
        return 0;
    }

    int max, min_at, max_at, min_at_prev, max_at_prev;
    int i;

    max = min_at = max_at = nums[0];
    for (i = 1; i < nums_size; ++i) {
        min_at_prev = min_at;
        max_at_prev = max_at;

        min_at = min_at_prev * nums[i] < max_at_prev * nums[i] ? min_at_prev * nums[i] : max_at_prev * nums[i];
        min_at = min_at < nums[i] ? min_at : nums[i];
        max_at = min_at_prev * nums[i] < max_at_prev * nums[i] ? max_at_prev * nums[i] : min_at_prev * nums[i];
        max_at = max_at < nums[i] ? nums[i] : max_at;

        max = max < max_at ? max_at : max;
    }

    return max;
}

/*****************************************************
 *
 * Sort Colors
 *
 * Given an array with n objects colored red, white or blue, 
 * sort them so that objects of the same color are adjacent, 
 * with the colors in the order red, white and blue.
 *
 * Here, we will use the integers 0, 1, and 2 to represent 
 * the color red, white, and blue respectively.
 *
 * Note:
 * You are not suppose to use the library's sort function for this problem.'
 *
*****************************************************/
void sort_colors_swap(int *p1, int *p2) {
    
    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

void sort_colors(int *nums, int nums_size) {

    int i, p, r;

    p = 0;
    r = nums_size - 1;

    for (i = 0; i < 2; ++i) {
        while (p < r) {
            while (p < r && nums[p] <= i) ++p;
            while (p < r && nums[r] > i) --r;
            sort_colors_swap(nums + p, nums + r);
        }
        r = nums_size - 1;
    }
}

/*****************************************************
 *
 * Rotate Image
 *
 * You are given an n x n 2D matrix representing an image.
 *
 * Rotate the image by 90 degrees (clockwise).
 *
 * Follow up:
 * Could you do this in-place?
 *
*****************************************************/
void 
rotate_matrix_swap(int *p1, int *p2) {
    
    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

void 
rotate_matrix(int **matrix, int matrix_row_size, int matrix_col_size) {
    
    if (matrix_row_size != matrix_col_size) {
        return;
    }

    int m = matrix_row_size, n = matrix_col_size;

    if (m < 1) {
        return;
    }

    int i, j, top, bottom;

    /* reverse up to down */
    for (j = 0; j < n; ++j) {
        for (top = 0, bottom = m - 1; top < bottom; ++top, --bottom) {
            rotate_matrix_swap(&matrix[top][j], &matrix[bottom][j]);
        }
    }

    /* swap the symmetry  */
    for (i = 0; i < m; ++i) {
        for (j = i + 1; j < n; ++j) {
            rotate_matrix_swap(&matrix[i][j], &matrix[j][i]);
        }
    }
}

/*****************************************************
 *
 * Maximum Subarray
 *
 * Find the contiguous subarray within an array 
 * (containing at least one number) which has the largest sum.
 *
 * For example, given the array [.2,1,.3,4,.1,2,1,.5,4],
 * the contiguous subarray [4,.1,2,1] has the largest sum = 6.
 *
*****************************************************/
int 
max_sub_array(int *nums, int nums_size) {

    if (nums_size < 1) {
        return 0;
    }

    int max, max_at;
    int i;

    max = max_at = nums[0];

    for (i = 1; i < nums_size; ++i) {
        max_at = max_at + nums[i] > nums[i] ? max_at + nums[i] : nums[i];
        max = max > max_at ? max : max_at;
    }

    return max;
}


/*****************************************************
 *
 * Set Matrix Zeroes
 *
 * Given a m x n matrix, if an element is 0, set its 
 * entire row and column to 0. Do it in place.
 *
*****************************************************/
void 
set_zeroes(int **matrix, int matrix_row_size, int matrix_col_size) {
    
    int m = matrix_row_size, n = matrix_col_size;

    if (m <= 0 || n <= 0) {
        return;
    }

    int col0 = 1, i, j;

    for (i = 0; i < m; ++i) {
        if (matrix[i][0] == 0) {
            col0 = 0;
        }
        for (j = 1; j < n; ++j) {
            if (matrix[i][j] == 0) {
                matrix[0][j] = matrix[i][0] = 0;
            }
        }
    }

    for (i = m - 1; i >= 0; --i) {
        for (j = n - 1; j >= 0 ; --j) {
            if (j == 0) {
                if (col0 == 0) {
                    matrix[i][0] = 0;
                }
            }
            else {
                if (matrix[i][0] == 0 || matrix[0][j] == 0) {
                    matrix[i][j] = 0;
                }
            }
        }
    }
}

/*****************************************************
 *
 * Best Time to Buy and Sell Stock
 *
 * Say you have an array for which the ith element is 
 * the price of a given stock on day i.
 *
 * If you were only permitted to complete at most one 
 * transaction (ie, buy one and sell one share of the stock), 
 * design an algorithm to find the maximum profit.
 *
*****************************************************/
int 
max_profit_stock(int *prices, int prices_size) {

    if (prices_size <= 0) {
        return 0;
    }

    /* 
     * max_profit[i] = max(max_profit[i - 1], prices[i] - min_price[i - 1])  
     * min_price[i] = min(min_price[i - 1], prices[i]) 
     */
    int min_price, max_profit, i;

    min_price = prices[0];
    max_profit = 0;

    for (i = 1; i < prices_size; ++i) {
        max_profit = algo_max(max_profit, prices[i] - min_price);
        min_price = algo_min(min_price, prices[i]);
    }

    return max_profit;
}

/*****************************************************
 *
 * Best Time to Buy and Sell Stock II
 *
 * Say you have an array for which the ith element is 
 * the price of a given stock on day i.
 *
 * Design an algorithm to find the maximum profit. You 
 * may complete as many transactions as you like 
 * (ie, buy one and sell one share of the stock multiple times). 
 * However, you may not engage in multiple transactions at the same time 
 * (ie, you must sell the stock before you buy again).
 *
*****************************************************/
int 
max_profit_stock_2(int *prices, int prices_size) {
    
    int max_profit = 0, i;

    for (i = 1; i < prices_size; ++i) {
        if (prices[i] > prices[i - 1]) {
            max_profit += prices[i] - prices[i - 1];
        }
    }

    return max_profit;
}

/*****************************************************
 *
 * Best Time to Buy and Sell Stock III
 *
 * Say you have an array for which the ith element is 
 * the price of a given stock on day i.
 *
 * Design an algorithm to find the maximum profit. 
 * You may complete at most two transactions.
 *
 * Note:
 * You may not engage in multiple transactions at the 
 * same time (ie, you must sell the stock before you buy again).
 *
*****************************************************/
int 
max_profit_stock_3(int *prices, int prices_size) {

    if (prices_size < 1) {
        return 0;
    }

    int *prev, *cur, *tmp;
    int i;

    prev = (int *)malloc(sizeof(int) * 4);
    cur = (int *)malloc(sizeof(int) * 4);

    cur[0] = cur[2] = -prices[0];
    cur[1] = cur[3] = 0;

    for (i = 1; i < prices_size; ++i) {
        /* swap buffer */
        tmp = prev;
        prev = cur;
        cur = tmp;

        cur[0] = algo_max(prev[0], -prices[i]);
        cur[1] = algo_max(prev[1], prev[0] + prices[i]);
        cur[2] = algo_max(prev[2], prev[1] - prices[i]);
        cur[3] = algo_max(prev[3], prev[2] + prices[i]);
    }

    return cur[3];
}

/*****************************************************
 *
 * Container With Most Water
 *
 * Given n non-negative integers a1, a2, ..., an, where 
 * each represents a point at coordinate (i, ai). n vertical 
 * lines are drawn such that the two endpoints of line i is 
 * at (i, ai) and (i, 0). Find two lines, which together with 
 * x-axis forms a container, such that the container contains 
 * the most water.
 *
*****************************************************/
int 
most_water(int *height, int height_size) {
    
    int water = 0, left = 0, right = height_size - 1, h;

    while (left < right) {
        h = algo_min(height[left], height[right]);
        water = algo_max(water, (right - left) * h);

        while (left < right && height[left] <= h) {
            ++left;
        }
        while (left < right && height[right] <= h) {
            --right;
        }
    }

    return water;
}

/*****************************************************
 *
 * Trapping Rain Water
 *
 * Given n non-negative integers representing an elevation 
 * map where the width of each bar is 1, compute how much 
 * water it is able to trap after raining.
 *
 * For example, 
 * Given [0,1,0,2,1,0,1,3,2,1,2,1], return 6.
 *
*****************************************************/
int 
trap_water(int *height, int height_size) {
    
    int *max_left, *max_right;
    int water = 0, i, n = height_size;

    if (n < 1) {
        return water;
    }

    max_left = (int *)malloc(sizeof(int) * n);
    max_right = (int *)malloc(sizeof(int) * n);

    /* 
     * For each bar i, the water it can trap is
     * water[i] = min(max_left[i], max_right[i]) - height[i],
     * max_left[i] represents the max bar of left, including itself 
     * max_right[i] represents the max bar of right, including itself 
     */
    max_left[0] = height[0];
    for (i = 1; i < n; ++i) {
        max_left[i] = algo_max(max_left[i - 1], height[i]);
    }

    max_right[n - 1] = height[n - 1];
    for (i = n - 2; i >= 0; --i) {
        max_right[i] = algo_max(max_right[i + 1], height[i]);
    }

    for (i = 0; i < n; ++i) {
        water += algo_min(max_left[i], max_right[i]) - height[i];
    }

    return water;

}

/*****************************************************
 *
 * Next Permutation
 *
 * Implement next permutation, which rearranges numbers 
 * into the lexicographically next greater permutation of numbers.
 *
 * If such arrangement is not possible, it must rearrange 
 * it as the lowest possible order (ie, sorted in ascending order).
 *
 * The replacement must be in-place, do not allocate extra memory.
 *
 * Here are some examples. Inputs are in the left-hand column 
 * and its corresponding outputs are in the right-hand column.
 * 1,2,3 -> 1,3,2
 * 3,2,1 -> 1,2,3
 * 1,1,5 -> 1,5,1
 *
*****************************************************/
void 
next_permutation(int *nums, int nums_size) {
    
    int n = nums_size;

    if (n <= 1) {
        return;
    }

    int i, j;

    for (i = n - 2; i >= 0; --i) {
        if (nums[i] < nums[i + 1]) {
            break;
        }
    }

    if (i >= 0){
        j = n - 1;
        while (nums[i] >= nums[j]) {
            --j;
        }
        swap(&nums[i], &nums[j]);
    }
    
    for (++i, j = n - 1; i < j; ++i, --j) {
        swap(&nums[i], &nums[j]);
    }

}

/*****************************************************
 *
 * Word Search
 *
 * Given a 2D board and a word, find if the word exists in the grid.
 *
 * For example,
 * Given board =
 * [
 *   ["ABCE"],
 *   ["SFCS"],
 *   ["ADEE"]
 * ]
 * word = "ABCCED", -> returns true,
 * word = "SEE", -> returns true,
 * word = "ABCB", -> returns false.
 *
*****************************************************/
bool 
word_search_exist_imp(char **board, int m, int n, int i, int j, char *word, int word_cur) {
    
    if (word[word_cur] == '\0') {
        return true;
    }
    if (i < 0 || i >= m || j < 0 || j >= n) {
        return false;
    }
    if (board[i][j] == '\0') {
        return false;
    }
    if (word[word_cur] != board[i][j]) {
        return false;
    }

    char tmp = board[i][j];
    board[i][j] = '\0';
    if (word_search_exist_imp(board, m, n, i - 1, j, word, word_cur + 1)
        || word_search_exist_imp(board, m, n, i, j + 1, word, word_cur + 1)
        || word_search_exist_imp(board, m, n, i + 1, j, word, word_cur + 1)
        || word_search_exist_imp(board, m, n, i, j - 1, word, word_cur + 1)) {      
        return true;
    }
    board[i][j] = tmp;

    return false;
}

bool 
word_search_exist(char **board, int board_row_size, int board_col_size, char *word) {
    
    int m = board_row_size, n = board_col_size;

    if (m == 0 || n == 0) {
        return false;
    }

    int i, j;

    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
            if(word_search_exist_imp(board, m , n, i, j, word, 0)) {
                return true;
            }
        }
    }

    return false;
}

/*****************************************************
 *
 * Minimum Size Subarray Sum
 *
 * Given an array of n positive integers and a positive integer s, 
 * find the minimal length of a subarray of which the sum . s. 
 * If there isn't one, return 0 instead.
 *
 * For example, given the array [2,3,1,2,4,3] and s = 7,
 * the subarray [4,3] has the minimal length under the problem constraint.
 *
*****************************************************/
int 
min_subarray_len(int s, int *nums, int nums_size) {
    
    int min_len = INT_MAX, begin = 0, end = 0, sum = 0;

    while (end < nums_size) {
        while (end < nums_size && sum < s) {
            sum += nums[end++];
        }
        if (sum < s) {
            break;
        }

        while(sum >= s) {
            sum -= nums[begin++];
        }
        min_len = algo_min(min_len, end - begin + 1);
    }

    return min_len == INT_MAX ? 0 : min_len;
}

