#ifndef _BINARY_SEARCH_H_INCLUDED_
#define _BINARY_SEARCH_H_INCLUDED_

#include <core.h>

int binary_search(int *nums, int nums_size, int target);
int binary_search_v2(int *nums, int nums_size, int target);
int search_insert(int *nums, int nums_size, int target);
int search_insert_v2(int *nums, int nums_size, int target);
int *search_range(int *nums, int nums_size, int target, int *return_size);
int *search_range_v2(int *nums, int nums_size, int target, int *return_size);

int my_sqrt(int x);


bool search_matrix(Int **matrix, int matrix_row_size, int matrix_col_size, int target);
bool search_matrix_2(int **matrix, int matrix_row_size, int matrix_col_size, int target);

int find_peak_element(int *nums, int nums_size);

int find_min(int *nums, int nums_size);
int find_min_v2(int *nums, int nums_size);
int find_min_2(int *nums, int nums_size);
int find_min_2_v2(int *nums, int nums_size);

int search_rotated_sorted(int *nums, int nums_size, int target);
bool search_rotated_sorted_2(int *nums, int nums_size, int target);
bool search_rotated_sorted_2_v2(int *nums, int nums_size, int target);

int h_index(int *citations, int citations_size);
int h_index_2(int *citations, int citations_size);
h_index_2_v2(int *citations, int citations_size);

double find_median_sorted_arrays(int *nums1, int nums1_size, int *nums2, int nums2_size);

#endif /* _BINARY_SEARCH_H_INCLUDED_ */
