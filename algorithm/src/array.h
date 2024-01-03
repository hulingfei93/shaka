#ifndef _ARRAY_H_INCLUDED_
#define _ARRAY_H_INCLUDED_

#include <core.h>

void rotate(int *nums, int nums_size, int k);
int remove_element(int *nums, int num_size, int val);
int remove_duplicates(int *nums, int num_size);
int majority_element(int *nums, int nums_size);
int *majority_element_2(int *nums, int nums_size, int *return_size);
int first_missing_positive(int *nums, int nums_size);
void merge(int *nums1, int m, int *nums2, int n);
int *spiral_order(int **matrix, int matrix_row_size, int matrix_col_size);
int **generate_matrix(int n);
int unique_paths(int m, int n);
int unique_paths_with_obstacles(int **obstacle_grid, int obstacle_grid_row_size, int obstacle_grid_col_size);
int remove_duplicates_2(int *nums, int nums_size);
int *product_except_self(int *nums, int nums_size, int *return_size);
int min_path_sum(int **grid, int grid_row_size, int grid_col_size);
int max_product(int *nums, int nums_size);
void sort_colors(int *nums, int nums_size);
void rotate_matrix(int **matrix, int matrix_row_size, int matrix_col_size);
int max_sub_array(int *nums, int nums_size);
void set_zeroes(int **matrix, int matrix_row_size, int matrix_col_size);
int max_profit_stock(int *prices, int prices_size);
int max_profit_stock_2(int *prices, int prices_size);
int max_profit_stock_3(int *prices, int prices_size);
int most_water(int *height, int height_size);
int trap_water(int *height, int height_size);
void next_permutation(int *nums, int nums_size);
bool word_search_exist(char **board, int board_row_size, int board_col_size, char *word);
int min_subarray_len(int s, int *nums, int nums_size);

#endif /* _ARRAY_H_INCLUDED_ */
