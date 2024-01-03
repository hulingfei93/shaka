#ifndef _MATH_H_INCLUDED_
#define _MATH_H_INCLUDED_

#include <core.h>

char *add_binary(char *a, char *b);
int add_digits(int num);
int title_to_number(char *s);
char *convert_to_title(int n);
int trailing_zeroes(int n);
unsigned char is_happy(int n);
int missing_number(int *nums, int nums_size);
int *plus_one(int *digits, int digits_size, int *return_size);
unsigned char is_power_of_two(int n);
unsigned char is_ugly(int num);
int my_atoi(char *str);
int roman_to_int(char *s);
unsigned char is_palindrome(int x);
int reverse(int x);
int count_primes(int n); 
struct ListNode *addTwoNumbers(struct ListNode *l1, struct ListNode *l2);
int num_squares(int n);

#endif /* _MATH_H_INCLUDED_ */
