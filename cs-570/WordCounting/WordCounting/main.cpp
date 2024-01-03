/*
Author: Lingfei Hu
CWID: 10394753
Date: 09/17/2014
Email: lhu4@stevens.edu
*/

#include<iostream>
#include<fstream>

using namespace std;

struct count_result
{
	int one_letter_word_count;
	int two_letter_word_count;
	int three_letter_word_count;
};

count_result count_word(const char *);
void output_table(count_result);


int main(int argc, char* argv[])
{
	count_result result = count_word("input.txt");
	output_table(result);
	return 0;
}

count_result count_word(const char *filePath)
{
	ifstream input_file(filePath);
	char letter;
	struct count_result result = { 0, 0, 0 };
	int currentWordCount = 0;
	//enum state { Out = 0, In = 1 };
	//state s = state::Out;

	if (input_file.fail())
	{
		cout << "The input file \"input.txt\" opening failed" << endl;
	}

	while (input_file.peek()!=EOF)
	{
		input_file.get(letter);
		if (letter != ' ' && letter != '\t' && letter != '\n')
		{
			//s = state::In;
			++currentWordCount;
		}
		else
		{
			//s = state::Out;
			if (currentWordCount == 1)
			{
				++result.one_letter_word_count;
			}
			else if (currentWordCount == 2)
			{
				++result.two_letter_word_count;
			}
			else if (currentWordCount == 3 || currentWordCount > 3)
			{
				++result.three_letter_word_count;
			}
			else // currentWordCount == 0
			{

			}
			currentWordCount = 0;
		}
	}
	if (currentWordCount > 0) // in case there is no space-like letter in the end of the file
	{
		if (currentWordCount == 1)
		{
			++result.one_letter_word_count;
		}
		else if (currentWordCount == 2)
		{
			++result.two_letter_word_count;
		}
		else
		{
			++result.three_letter_word_count;
		}
	}

	input_file.close();

	return result;
}

void output_table(count_result result)
{
	cout << "The count of words with 1 letter: " << result.one_letter_word_count << endl;
	cout << "The count of words with 2 letters: " << result.two_letter_word_count << endl;
	cout << "The count of words with 3 or more letters: " << result.three_letter_word_count << endl;
}

