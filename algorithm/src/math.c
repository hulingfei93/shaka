#include <core.h>

/*****************************************************
 *
 * Given two binary strings, return their sum(also a binary string)
 *
 * For example:
 * a = "11"
 * b = "1"
 * Return "100"
 *
*****************************************************/
char *add_binary(char *a, char *b) {
	if (a == NULL || b == NULL)
	  return NULL;

	int len_a, len_b, len_max, len_sum;
	char *sum;

	len_a = strlen(a);
	len_b = strlen(b);

	len_max = len_a < len_b ? len_b : len_a;
	len_sum = len_max + 1;
	sum = (char *)malloc(len_sum + 1);
	sum[len_sum] = '\0';

	int i, j, s, bit_sum, add;

	add = 0;
	for (i = len_a - 1, j = len_b - 1, s = len_sum - 1; i >= 0 && j >= 0; --i, --j) {
		bit_sum = 0;
		if (a[i] == '1') {
			++bit_sum;
		}
		if (b[j] == '1') {
			++bit_sum;
		}
		bit_sum += add;

		if (bit_sum == 0) {
			sum[s] = '0';
			add = 0;
		}
		else if (bit_sum == 1) {
			sum[s] = '1';
			add = 0;
		}
		else if (bit_sum == 2) {
			sum[s] = '0';
			add = 1;
		}
		else {
			sum[s] = '1';
			add = 1;
		}

		--s;
	}

	while (i >= 0 ) {
		bit_sum = 0;
		if (a[i] == '1') {
			++bit_sum;
		}
		bit_sum += add;

		if (bit_sum == 0) {
			sum[s] = '0';
			add = 0;
		}
		else if (bit_sum == 1) {
			sum[s] = '1';
			add = 0;
		}
		else if (bit_sum == 2) {
			sum[s] = '0';
			add = 1;
		}
		else {
			sum[s] = '1';
			add = 1;
		}

		--s;
		--i;
	}

	while (j >= 0 ) {
		bit_sum = 0;
		if (b[j] == '1') {
			++bit_sum;
		}
		bit_sum += add;

		if (bit_sum == 0) {
			sum[s] = '0';
			add = 0;
		}
		else if (bit_sum == 1) {
			sum[s] = '1';
			add = 0;
		}
		else if (bit_sum == 2) {
			sum[s] = '0';
			add = 1;
		}
		else {
			sum[s] = '1';
			add = 1;
		}

		--s;
		--j;
	}

	if (add > 0) {
		sum[s] = '1';
		return sum;
	}
	else {
		return sum + 1;
	}
}


/*****************************************************
 *
 * Given a non-negative ingeter num, repeatedly add 
 * its digits until the result has only one digit.
 *
 * For example:
 * Given num = 38, the process is like: 3 + 8 = 11, 
 * 1 + 1 = 2. Since 2 has only one digit, return it.
 *
*****************************************************/
int add_digits(int num) {
	return (num - 1) % 9 + 1;
}

/*****************************************************
 *
 * Given a column title as appear in an Excel sheet, 
 * return its corresponding column number.
 *
 * For example:
 * A -> 1
 * B -> 2
 * C -> 3
 * ...
 * Z -> 26
 * AA -> 27
 * AB -> 28
 *
*****************************************************/
int title_to_number(char *s) {

	if (s == NULL) {
		return 0;
	}

	int i, num, base;

	i = strlen(s) - 1;
	num = 0;
	base = 1;

	while(i >= 0) {
		num += (s[i] - 'A' + 1) * base;

		base *= 26;
		--i;
	}

	return num;
}

/*****************************************************
 *
 * Given a positive integer, return its corresponding
 * column title as appear in an Excel sheet.
 *
 * For example:
 * 1 -> A
 * 2 -> B
 * 3 -> C
 * ...
 * 26 -> Z
 * 27 -> AA
 * 28 -> AB
 *
*****************************************************/
char *convert_to_title(int n) {

	int i;
	char *s;

	s = (char *)malloc(100);
	s[99] = '\0';
	i = 98;

	while (n > 0 && i >= 0) {
		s[i] = (n - 1) % 26 + 'A';
		n = (n - 1) / 26;
		--i;
	}

	return s + i + 1;
}

/*****************************************************
 *
 * Given an integer n, return the number of trailing
 * zeroes in n!.
 *
 * Note: Your solution should be in logarithmic time 
 * complexity.
 *
*****************************************************/
int trailing_zeroes(int n) {
	int count;

	count = 0;
	while (n) {
		n /= 5;
		count += n;
	}

	return count;
}

/*****************************************************
 *
 * Write an algorithm to determine if a number is 
 * "happy".
 *
 * A happy number is a number defined by the following
 * process: Starting with any positive integer,
 * replace the number by the sum of the squares of its
 * digits, and repeat the process until the number
 * equals 1 (where it will stay), or it loops endlessly
 * in a cycle which does not include 1. Those numbers
 * for which this process ends in 1 are happy number.
 *
 * For example: 19 is a happy number
 * 1^2 + 9^2 = 82
 * 8^2 + 2^2 = 68
 * 6^2 + 8^2 = 100
 * 1^2 + 0^2 + 0^2 = 1
 *
*****************************************************/
int is_happy_cal(int n) {
	int sum, tmp;

	sum = 0;
	while (n) {
		tmp = n % 10;
		sum += tmp * tmp;
		n /= 10;
	}

	return sum;
}

unsigned char is_happy(int n) {
	if (n <= 0) {
		return 0;
	}

	int sum1, sum2, n1, n2;

	n1 = n2 = n;
	while (1) {
		sum1 = is_happy_cal(n1);
		sum2 = is_happy_cal(is_happy_cal(n2));

		if (sum1 == 1 || sum2 == 1) {
			break;
		}
		
		if (sum1 == sum2) {
			return 0;
		}

		n1 = sum1;
		n2 = sum2;
	}

	return 1;
}


/*****************************************************
 *
 * Given an array containing n distinct numbers taken
 * from 0, 1, 2, ..., n, find the one that is missing
 * from the array.
 *
 * For example:
 * Given nums = [0, 1, 3], return 2.
 *
 * Note:
 * Your algorithm should run in linear runtime complexity.
 * Could you implement it using only constant extra
 * space complexity?
 *
*****************************************************/
int missing_number(int *nums, int nums_size) {
	int sum, i;

	sum = nums_size * (nums_size + 1) / 2;
	
	for (i = 0; i < nums_size; ++i) {
		sum -= nums[i];
	}

	return sum;
}	

/*****************************************************
 *
 * Given a non-negative number represented as an array
 * of digits, plus one to the number.
 *
 * The digits are stored such that the most significant
 * digit is at the head of the list.
 *
*****************************************************/
int *plus_one(int *digits, int digits_size, int *return_size) {
	
	if (digits == NULL ||  return_size < digits_size)
	  return NULL;

	int *results;
	int i, s, sum, add;

	*return_size = digits_size + 1;
	results = (int *)malloc(*return_size * sizeof(int));
	memset(results, 0, *return_size * sizeof(int));

	sum = digits[digits_size - 1] + 1;
	if (sum < 10) {
		memcpy(results, digits, digits_size * sizeof(int));
		results[digits_size - 1] = sum;

		*return_size = digits_size;
		return results;
	}
	
	add = 1;
	results[*return_size - 1] = 0;
	for (i = digits_size - 2, s = *return_size - 2; i >= 0; --i, --s) {
		sum = digits[i] + add;
		results[s] = sum % 10;
		add = sum / 10;
	}

	if (add > 0) {
		results[s] = add;
	}

	return results;

}

/*****************************************************
 *
 * Given an integer, write a function to determine
 * if it is a power of two.
 *
*****************************************************/
unsigned char is_power_of_two(int n) {
	if (n <= 0)
	  return 0;

	int m;

	m = n & (n - 1);

	return m == 0 ? 1 : 0;
}

/*****************************************************
 *
 * Write a program to check whether a given number is 
 * an ugly number.
 *
 * Ugly numbers are positive numbers whose prime factors 
 * only include 2, 3, 5. For example, 6, 8 are ugly 
 * while 14 is not ugly since it includes another prime factor 7.
 *
 * Note that 1 is typically treated as an ugly number
 *
*****************************************************/
unsigned char is_ugly(int num) {
	if (num <= 0)
	  return 0;

	if (num == 1)
	  return 1;

	while (num % 2 == 0) {
		num = num >> 1;
	}

	while (num % 3 == 0) {
		num = num / 3;
	}

	while (num % 5 == 0) {
		num = num / 5;
	}

	return num == 1 ? 1 : 0;
}

/*****************************************************
 *
 * String to Integer (atoi)
 *
 * Implement atoi to convert a string to an integer.
 *
 * Hint: Carefully consider all possible input cases. 
 * If you want a challenge, please do not see below 
 * and ask yourself what are the possible input cases.
 *
 * Notes: It is intended for this problem to be specified 
 * vaguely (ie, no given input specs). You are responsible 
 * to gather all the input requirements up front.
 *
*****************************************************/
int my_atoi(char *str) {
	if (!str) {
		return 0;
	}

	long long num = 0, sign = 1;

	while (isspace(*str)) {
		++str;
	}

	if (*str == '+') {
		++str;
	}
	else if (*str == '-') {
		sign = -1;
		++str;
	}

	while (*str >= '0' && *str <= '9') {
		num = num * 10 + (*str - '0');
		if (num > 2147483648) {
			break;
		}
		++str;
	} 

	num *= sign;

	if (num > INT_MAX) {
		return INT_MAX;
	}
	
	if (num < INT_MIN) {
		return INT_MIN;
	}
	
	return (int)num;


}	


/*****************************************************
 *
 * Roman to Integer
 *
 * Given a roman numeral, convert it to an integer.
 *
 * Input is guaranteed to be within the range from 1 to 3999.
 *
*****************************************************/
int roman_to_int(char *s) {
	if (!s) {
		return 0;
	}

	int num = 0, len, i, temp, last = 0;

	len = strlen(s);

	for (i = len - 1; i >= 0; --i) {
		switch (s[i]) {
			case 'I': temp = 1; break;
			case 'V': temp = 5; break;
			case 'X': temp = 10; break;
			case 'L': temp = 50; break;
			case 'C': temp = 100; break;
			case 'D': temp = 500; break;
			case 'M': temp = 1000; break;
			default: temp = 0; break;
		}
		num += (temp >= last ? temp : -1 * temp);
		last = temp;
	}

	return num;
}

/*****************************************************
 *
 * Palindrome Number
 *
 * Determine whether an integer is a palindrome. 
 * Do this without extra space.
 *
*****************************************************/
unsigned char is_palindrome(int x) {
	if (x < 0) {
		return 0;
	}

	long long num = 0;
	int temp = x;

	while (temp) {
		num = num * 10 + temp % 10;
		if (num > INT_MAX) {
			return 0;
		}
		temp /= 10;
	}

	return num == x ? 1 : 0;
}

/*****************************************************
 *
 * Reverse Integer 
 *
 * Reverse digits of an integer.
 *
 * Example1: x = 123, return 321
 * Example2: x = -123, return -321
 *
*****************************************************/
int reverse(int x) {
	long long num = 0, n = x; 
	int sign = 1;

	if (x < 0) {
		sign = -1;
		n *= -1;
	}

	while (n) {
		num = num * 10 + n % 10;
		if (sign == 1 && num > INT_MAX) {
			return 0;
		}
		else if (sign == -1 && num * sign < INT_MIN) {
			return 0;
		}
		n /= 10;
	}

	return (int)num * sign;
}

/*****************************************************
 *
 * Count Primes
 * Count the number of prime numbers less than a non-negative number, n.
 *
*****************************************************/
bool is_prime(int n) {
	if (n <= 1) {
		return false;
	}

	int i;

	for (i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			return false;
		}
	}

	return true;
}

int count_primes(int n) {
	if (n <= 2) {
		return 0;
	}

	bool *prime_mask;
	int i, j, count = 0;

	prime_mask = (bool *)malloc(sizeof(bool) * n);
	memset(prime_mask, true, n);

	for (i = 2; i * i < n; ++i) {
		if (!prime_mask[i]) {
			continue;
		}
		for (j = i * i; j < n; j += i) {
			prime_mask[j] = false;
		}
	}

	for (i = 2; i < n; ++i) {
		if (prime_mask[i]) {
			++count;
		}
	}

	return count;
}


/*****************************************************
 *
 * Add Two Numbers
 *
 * You are given two linked lists representing two non-negative numbers. 
 * The digits are stored in reverse order and each of their nodes contain a single digit. 
 * Add the two numbers and return it as a linked list.
 *
 * Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
 * Output: 7 -> 0 -> 8
 *
*****************************************************/
struct ListNode {
     int val;
     struct ListNode *next;
};

struct ListNode *addTwoNumbers(struct ListNode *l1, struct ListNode *l2) {
	struct ListNode *sum, *tail, *c1, *c2, *new_node;
	int n1, n2, add = 0;

	sum = tail = NULL;

	c1 = l1;
	c2 = l2;

	while (c1 != NULL || c2 != NULL) {
		if (c1 != NULL) {
			n1 = c1->val;
			c1 = c1->next;
		}
		else {
			n1 = 0;
		}

		if (c2 != NULL) {
			n2 = c2->val;
			c2 = c2->next;
		}
		else {
			n2 = 0;
		}
		
		new_node = (struct ListNode*)malloc(sizeof(struct ListNode));
		new_node->val = (n1 + n2 + add) % 10;
		new_node->next = NULL;
		add = (n1 + n2 + add) / 10;
		
		if (sum == NULL) {
			sum = tail = new_node;
		}
		else {
			tail->next = new_node;
			tail = new_node;
		}

	}

	if (add > 0) {
		new_node = (struct ListNode*)malloc(sizeof(struct ListNode));
		new_node->val = add;
		new_node->next = NULL;
		tail->next = new_node;
		tail = new_node;
	}

	return sum;
}

/*****************************************************
 *
 * Perfect Squares
 *
 * Given a positive integer n, find the least number of perfect square numbers 
 * (for example, 1, 4, 9, 16, ...) which sum to n.
 *
 * For example, given n = 12, return 3 because 12 = 4 + 4 + 4; 
 * given n = 13, return 2 because 13 = 4 + 9.
 *
*****************************************************/
int num_squares(int n) {
	if (n <= 0) {
		return 0;
	}

	int end = (int)sqrt(n), square;
	int i, j, temp, least_num = n, current_num;

	for (i = 1; i <= end; ++i) {
		temp = n;
		current_num = 0;
		for (j = i; j >= 1; --j) {
			square = j * j;
			current_num += temp / square;
			temp %= square;

			if (temp == 0) {
				break;
			}
		}
		
		if (current_num < least_num) {
			least_num = current_num;
		}
	}

	return least_num;
}

