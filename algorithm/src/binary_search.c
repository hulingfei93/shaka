#include <core.h>
/*****************************************************
 * Binary Search
*****************************************************/
int
binary_search(int *nums, int nums_size, int target) {
    if (nums_size < 1) {
        return -1;
    }

    int low = 0, high = nums_size - 1, mid;
    
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (nums[mid] == target) {
            return mid;
        }
        else if (nums[mid] < target) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return -1;
}

int
binary_search_v2(int *nums, int nums_size, int target) {
    if (nums_size < 1) {
        return -1;
    }

    int low = 0, high = nums_size - 1, mid;
    
    while (low + 1 < high) {
        mid = low + (high - low) / 2;
        if (nums[mid] == target) {
            return mid;
        }
        else if (nums[mid] < target) {
            low = mid;
        }
        else {
            high = mid;
        }
    }

    if (nums[low] == target) {
        return low;
    }
    if (nums[high] == target) {
        return high;
    }

    return -1;
}

/*****************************************************
 *
 * Search Insert Position
 *
 * Given a sorted array and a target value, return the 
 * index if the target is found. If not, return the index 
 * where it would be if it were inserted in order.
 *
 * You may assume no duplicates in the array.
 *
 * Here are few examples.
 * [1,3,5,6], 5 -> 2
 * [1,3,5,6], 2 -> 1
 * [1,3,5,6], 7 -> 4
 * [1,3,5,6], 0 -> 0
 *
*****************************************************/
int search_insert(int *nums, int nums_size, int target) {
    if (nums_size < 1) {
        return 0;
    }

    int low = 0, high = nums_size - 1, mid;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (nums[mid] == target) {
            return mid;
        }
        else if (nums[mid] < target) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return low;
}

int search_insert_v2(int *nums, int nums_size, int target) {
    if (nums_size < 1) {
        return 0;
    }

    int low = 0, high = nums_size - 1, mid;
    while (low + 1 < high) {
        mid = low + (high - low) / 2;
        if (nums[mid] == target) {
            return mid;
        }
        else if (nums[mid] < target) {
            low = mid;
        }
        else {
            high = mid;
        }
    }

    if (nums[low] >= target) {
        return low;
    }
    if (nums[high] >= target) {
        return high;
    }
    return high + 1;
}

/*****************************************************
 *
 * Search for a Range
 *
 * Given a sorted array of integers, find the starting and ending position of a given target value.
 * If the target is not found in the array, return [-1, -1].
 *
 * For example,
 * Given [5, 7, 7, 8, 8, 10] and target value 8,
 * return [3, 4].
 *
*****************************************************/
int *
search_range(int *nums, int nums_size, int target, int *return_size) {
    
    int low, high, mid;
    int *range;

    range = (int *)malloc(sizeof(int) * 2);
    range[0] = range[1] = -1;
    *return_size = 2;

    if (nums_size < 1) {
        return range;
    }

    low = 0;
    high = nums_size - 1;

    /* search left boundary */
    while (low < high) {
        mid = low + (high - low) / 2;
        if (nums[mid] < target) {
            low = mid + 1;
        }
        else {
            high = mid;
        }
    }

    if (nums[low] != target) {
        return range;
    }

    range[0] = low;

    low = 0;
    high = nums_size - 1;

    /* search right boundary */
    while (low < high) {
        mid = low + (high - low) / 2 + 1;
        if (nums[mid] <= target) {
            low = mid;
        }
        else {
            high = mid - 1;
        }
    } 

    range[1] = low;

    return range;
}

int *
search_range_v2(int *nums, int nums_size, int target, int *return_size) {
    
    int low, high, mid;
    int *range;

    range = (int *)malloc(sizeof(int) * 2);
    range[0] = range[1] = -1;
    *return_size = 2;

    if (nums_size < 1) {
        return range;
    }

    low = 0;
    high = nums_size - 1;

    /* search left boundary */
    while (low + 1 < high) {
        mid = low + (high - low) / 2;
        if (nums[mid] == target) {
            high = mid;
        }
        else if (nums[mid] < target) {
            low = mid;
        }
        else {
            high = mid;
        }
    }

    if (nums[low] == target) {
        range[0] = low;
    }
    else if (nums[high] == target) {
        range[0] = high;
    }
    else {
        return range;
    }

    low = 0;
    high = nums_size - 1;

    /* search right boundary */
    while (low + 1 < high) {
        mid = low + (high - low) / 2;
        if (nums[mid] == target) {
            low = mid;
        }
        else if (nums[mid] < target) {
            low = mid;
        }
        else {
            high = mid;
        }
    } 

    if (nums[high] == target) {
        range[1] = high;
    }
    else {
        range[1] = low;
    }

    return range;
}

/*****************************************************
 *
 * Sqrt(x)
 *
 * Implement int sqrt(int x).
 *
 * Compute and return the square root of x.
 *
*****************************************************/

int 
my_sqrt(int x) {
    if (x == 0) {
        return 0;
    }

    int low = 1, high = x, mid, k;

    while (low + 1 < high) {
        mid = low + (high - low) / 2;
        k = x / mid;
        if (k == mid) {
            low = mid;
        }
        else if (k < mid) {
            high = mid;
        }
        else {
            low = mid;
        }
    }

    if (x / high >= high) {
        return high;
    }

    return low;
}

/*****************************************************
 *
 * Search a 2D Matrix
 *
 * Write an efficient algorithm that searches for a value 
 * in an m x n matrix. This matrix has the following properties:
 *
 *      Integers in each row are sorted from left to right.
 *      The first integer of each row is greater than the last integer of the previous row.
 * 
 * For example,
 *
 * Consider the following matrix:
 *
 * [
 *   [1,   3,  5,  7],
 *   [10, 11, 16, 20],
 *   [23, 30, 34, 50]
 * ]
 *
 * Given target = 3, return true.
 *
*****************************************************/
bool
search_matrix(int **matrix, int matrix_row_size, int matrix_col_size, int target) {

    int m = matrix_row_size, n = matrix_col_size;

    if (m < 1 || n < 1) {
        return false;
    }

    int low, high, mid;

    low = 0;
    high = m - 1;

    while (low <= high) {
        mid = low + (high - low) / 2;

        if (matrix[mid][0] == target) {
            return true;
        }
        else if (matrix[mid][0] > target) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }

    int row = high;
    if (row < 0 || row >= m) {
        return false;
    }

    low = 0;
    high = n - 1;

    while (low <= high) {
        mid = low + (high - low) / 2;

        if (matrix[row][mid] == target) {
            return true;
        }
        else if (matrix[row][mid] > target) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }

    return false;
}

bool
search_matrix_v2(int **matrix, int matrix_row_size, int matrix_col_size, int target) {

    int m = matrix_row_size, n = matrix_col_size;

    if (m < 1 || n < 1) {
        return false;
    }

    int low, high, mid;

    low = 0;
    high = m - 1;

    while (low + 1 < high) {
        mid = low + (high - low) / 2;

        if (matrix[mid][0] == target) {
            return true;
        }
        else if (matrix[mid][0] > target) {
            high = mid;
        }
        else {
            low = mid;
        }
    }

    int row;
    if (matrix[low][0] > target) {
        row = low - 1;
    }
    else if (matrix[low][0] == target) {
        return true;
    }
    else if (matrix[high][0] > target) {
        row = low;
    }
    else if (matrix[high][0] == target) {
        return true;
    }
    else {
        row = high;
    }
    

    if (row < 0 || row >= m) {
        return false;
    }

    low = 0;
    high = n - 1;

    while (low + 1 < high) {
        mid = low + (high - low) / 2;

        if (matrix[row][mid] == target) {
            return true;
        }
        else if (matrix[row][mid] > target) {
            high = mid;
        }
        else {
            low = mid;
        }
    }
    
    if (matrix[row][low] == target) {
        return true;
    }
    if (matrix[row][high] == target) {
        return true;
    }

    return false;
}
/*****************************************************
 *
 * Search a 2D Matrix II
 *
 * Write an efficient algorithm that searches for a value 
 * in an m x n matrix. This matrix has the following properties:
 *
 * Integers in each row are sorted in ascending from left to right.
 * Integers in each column are sorted in ascending from top to bottom.
 * For example,
 *
 * Consider the following matrix:
 *
 * [
 *   [1,   4,  7, 11, 15],
 *   [2,   5,  8, 12, 19],
 *   [3,   6,  9, 16, 22],
 *   [10, 13, 14, 17, 24],
 *   [18, 21, 23, 26, 30] 
 * ]
 *
 * Given target = 5, return true.
 * Given target = 20, return false.
 *
*****************************************************/
bool 
search_matrix_2(int **matrix, int matrix_row_size, int matrix_col_size, int target) {
    
    int m = matrix_row_size, n = matrix_col_size;

    if (m < 1 || n < 1) {
        return false;
    }

    int i = m - 1, j = 0;

    while (i >= 0 && j < n) {
        if (matrix[i][j] == target) {
            return true;
        }
        else if (matrix[i][j] > target) {
            --i;
        }
        else {
            ++j;
        }
    }

    return false;
}

/*****************************************************
 *
 * Find Peak Element
 *
 * A peak element is an element that is greater than its neighbors.
 *
 * Given an input array where num[i] . num[i+1], find a peak element and return its index.
 *
 * The array may contain multiple peaks, in that case return the index to any one of the peaks is fine.
 *
 * You may imagine that num[-1] = num[n] = -..
 *
 * For example, in array [1, 2, 3, 1], 3 is a peak element and your function should return the index number 2.
 *
 * Note:
 * Your solution should be in logarithmic complexity.
 *
*****************************************************/
int find_peak_element(int *nums, int nums_size) {
    if (nums_size < 1) {
        return -1;
    }

    int low = 0, high = nums_size - 1, mid1, mid2;

    while (low < high) {
        mid1 = low + (high - low) / 2;
        mid2 = mid1 + 1;
        if (nums[mid1] < nums[mid2]) {
            low = mid2;
        }
        else {
            high = mid1;
        }
    }

    return low;
}

/*****************************************************
 *
 * Find Minimum in Rotated Sorted Array
 *
 * Suppose a sorted array is rotated at some pivot unknown to you beforehand.
 *
 * (i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).
 *
 * Find the minimum element.
 *
 * You may assume no duplicate exists in the array.
 *
*****************************************************/
int 
find_min(int *nums, int nums_size) {
    
    int low = 0, high = nums_size - 1, mid;

    while (low < high) {
        mid = low + (high - low) / 2;
        if (nums[mid] < nums[high]) {
            high = mid;
        }
        else {
            low = mid + 1;
        }
    }

    return nums[low];
}

int 
find_min_v2(int *nums, int nums_size) {
    
    int low = 0, high = nums_size - 1, mid;

    while (low + 1 < high) {
        mid = low + (high - low) / 2;
        if (nums[mid] < nums[high]) {
            high = mid;
        }
        else {
            low = mid;
        }
    }

    if (nums[low] < nums[high]) {
        return nums[low];
    }
    return nums[high];
}

/*****************************************************
 *
 * Find Minimum in Rotated Sorted Array II
 *
 * Suppose a sorted array is rotated at some pivot unknown to you beforehand.
 *
 * (i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).
 *
 * Find the minimum element.
 *
 * The array may contain duplicates.
 *
*****************************************************/
int 
find_min_2(int *nums, int nums_size) {
    
    int low = 0, high = nums_size - 1, mid;

    while (low < high) {
        mid = low + (high - low) / 2;
        if (nums[mid] < nums[high]) {
            high = mid;
        }
        else if (nums[mid] > nums[high]) {
            low = mid + 1;
        }
        else {
            --high;
        }
    }

    return nums[low];
}

int 
find_min_2_v2(int *nums, int nums_size) {
    
    int low = 0, high = nums_size - 1, mid;

    while (low + 1 < high) {
        mid = low + (high - low) / 2;
        if (nums[mid] < nums[high]) {
            high = mid;
        }
        else if (nums[mid] > nums[high]) {
            low = mid;
        }
        else {
            --high;
        }
    }
    
    if (nums[low] < nums[high]) {
        return nums[low];
    }
    return nums[high];
}

/*****************************************************
 *
 * Search in Rotated Sorted Array
 *
 * Suppose a sorted array is rotated at some pivot unknown to you beforehand.
 *
 * (i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).
 *
 * You are given a target value to search. If found in the array return its index, otherwise return -1.
 *
 * You may assume no duplicate exists in the array.
 *
*****************************************************/
int 
search_rotated_sorted(int *nums, int nums_size, int target) {

    int low = 0, high = nums_size - 1, mid;

    /* find the index of min */
    while (low < high) {
        mid = low + (high - low) / 2;

        if (nums[mid] >= nums[high]) {
            low = mid + 1;
        }
        else {
            high = mid;
        }
    }

    int base = low, index;

    low = 0;
    high = nums_size - 1;
    while (low <= high) {
        mid = low + (high - low) / 2;
        index = (base + mid) % nums_size;

        if (nums[index] == target) {
            return index;
        }
        else if (nums[index] > target) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }

    return -1;
}

/*****************************************************
 *
 * Search in Rotated Sorted Array II
 *
 * Follow up for "Search in Rotated Sorted Array":
 * What if duplicates are allowed?
 *
 * Would this affect the run-time complexity? How and why?
 *
 * Write a function to determine if a given target is in the array.
 *
*****************************************************/
bool 
search_rotated_sorted_2(int *nums, int nums_size, int target) {

    int low = 0, high = nums_size - 1, mid;

    while (low < high) {
        mid = low + (high - low) / 2;

        if (nums[mid] == target) {
            return true;
        }
        
        if (nums[mid] > nums[high]) {
            if (nums[low] <= target && nums[mid] > target) {
                high = mid;
            }
            else {
                low = mid + 1;
            }
        }
        else if (nums[mid] < nums[high]) {
            if (nums[mid] < target && nums[high] >= target) {
                low = mid + 1;
            }
            else {
                high = mid;
            }
        }
        else {
            --high;
        }
    }

    return nums[low] == target ? true : false;
}

bool 
search_rotated_sorted_2_v2(int *nums, int nums_size, int target) {

    int low = 0, high = nums_size - 1, mid;

    while (low + 1 < high) {
        mid = low + (high - low) / 2;

        if (nums[mid] == target) {
            return true;
        }
        
        if (nums[mid] > nums[high]) {
            if (nums[low] <= target && nums[mid] > target) {
                high = mid;
            }
            else {
                low = mid;
            }
        }
        else if (nums[mid] < nums[high]) {
            if (nums[mid] < target && nums[high] >= target) {
                low = mid;
            }
            else {
                high = mid;
            }
        }
        else {
            --high;
        }
    }

    if (nums[low] == target) {
        return true;
    }
    if (nums[high] == target) {
        return true;
    }
    return false;
}

/*****************************************************
 *
 * H-Index
 *
 * Given an array of citations (each citation is a non-negative 
 * integer) of a researcher, write a function to compute the researcher's h-index.
 *
 * According to the definition of h-index on Wikipedia: 
 * "A scientist has index h if h of his/her N papers have 
 * at least h citations each, and the other N . h papers 
 * have no more than h citations each."
 *
 * For example, given citations = [3, 0, 6, 1, 5], which means 
 * the researcher has 5 papers in total and each of them had 
 * received 3, 0, 6, 1, 5 citations respectively. Since the 
 * researcher has 3 papers with at least 3 citations each and 
 * the remaining two with no more than 3 citations each, his h-index is 3.
 *
 * Note: If there are several possible values for h, 
 * the maximum one is taken as the h-index.
 *
 * Hint:
 * 1. An easy approach is to sort the array first.
 * 2. What are the possible values of h-index?
 * 3. A faster approach is to use extra space.
 *
*****************************************************/
int 
h_index(int *citations, int citations_size) {

    int len = citations_size, i;

    if (len < 1) {
        return 0;
    }

    int *array = (int *)malloc(sizeof(int) * (len + 1));
    for (i = 0; i <= len; ++i) {
        array[i] = 0;
    }

    for (i = 0; i < len; ++i) {
        if (citations[i] >= len) {
            array[len] += 1;
        }
        else {
            array[citations[i]] += 1;
        }
    }

    int tmp = 0;
    for (i = len; i >= 0; --i) {
        tmp += array[i];
        if (tmp >= i) {
            return i;
        }
    }

    return 0;
}

/*****************************************************
 *
 * H-Index II
 *
 * Follow up for H-Index: What if the citations array 
 * is sorted in ascending order? Could you optimize your algorithm?
 *
 * Hint:
 * Expected runtime complexity is in O(log n) and the input is sorted.
 *
*****************************************************/
int 
h_index_2(int *citations, int citations_size) {

    int low = 0, high = citations_size - 1, mid, count;

    while (low <= high) {
        mid = low + (high - low) / 2;
        count = citations_size - mid;
        
        if (citations[mid] == count) {
            return count;
        }
        else if (citations[mid] < count) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return citations_size - low;
}


int 
h_index_2_v2(int *citations, int citations_size) {

    int low = 0, high = citations_size - 1, mid, count;

    while (low + 1 < high) {
        mid = low + (high - low) / 2;
        count = citations_size - mid;
        
        if (citations[mid] == count) {
            return count;
        }
        if (citations[mid] > count) {
            high = mid;
        }
        else {
            low = mid;
        }
    }

    count = citations_size - low;
    if (citations[low] >= count) {
        return count;
    }

    count = citations_size - high;
    if (citations[high] >= count) {
        return count;
    }

    return 0;
}

/*****************************************************
 *
 * Median of Two Sorted Arrays
 *
 * There are two sorted arrays nums1 and nums2 of size 
 * m and n respectively. Find the median of the two 
 * sorted arrays. The overall run time complexity
 * should be O(log (m+n)).
 *
*****************************************************/
int
find_median_sorted_arrays_helper(int *a, int a_len, int a_start, int *b, int b_len, int b_start, int k)
{
    if (a_start >= a_len) {
        return b[b_start + k - 1];
    }
    if (b_start >= b_len) {
        return a[a_start + k - 1];
    }

    if (k == 1) {
        return algo_min(a[a_start], b[b_start]);
    }

    int a_key, b_key;

    a_key = a_start + k / 2 - 1 < a_len ? a[a_start + k / 2 - 1] : INT_MAX;
    b_key = b_start + k / 2 - 1 < b_len ? a[b_start + k / 2 - 1] : INT_MAX;

    if (a_key < b_key) {
        return find_median_sorted_arrays_helper(
                    a, a_len, a_start + k / 2,
                    b, b_len, b_start,
                    k - k / 2);
    }
    else {
        return find_median_sorted_arrays_helper(
                    a, a_len, a_start,
                    b, b_len, b_start + k / 2,
                    k - k / 2);
    }
}

double 
find_median_sorted_arrays(int *nums1, int nums1_size, int *nums2, int nums2_size)
{
    int len;

    len = nums1_size + nums2_size;

    if (len % 2 == 1) {
        return find_median_sorted_arrays_helper(
                    nums1, nums1_size, 0, 
                    nums2, nums2_size, 0, 
                    len / 2 + 1);
    }
    else {
        return (find_median_sorted_arrays_helper(
                    nums1, nums1_size, 0, 
                    nums2, nums2_size, 0, 
                    len / 2)
                + find_median_sorted_arrays_helper(
                    nums1, nums1_size, 0, 
                    nums2, nums2_size, 0, 
                    len / 2 + 1)
                ) / 2.0;
    }
}

