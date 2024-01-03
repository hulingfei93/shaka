#include "algebraic_expression.h"

void algebraic_expression::execute(const string& file_path)
{
	clear();
	read_data_file(file_path);
	convert_infix_to_postfix();
	cout << "answer: " << evaluate() << endl;
}

void algebraic_expression::read_data_file(const string& file_path)
{
	ifstream input_file(file_path.c_str());
	char letter;

	if (input_file.fail())
	{
		string err_msg = "error: The input file " + file_path + " opening failed";
		throw exception(err_msg.c_str());
	}

	cout << "read expression from file '" + file_path + "'" << endl;
	while (input_file.peek() != EOF)
	{
		input_file.get(letter);
		if (isdigit(letter)
			|| letter == '+'
			|| letter == '-'
			|| letter == '*'
			|| letter == '/'
			|| letter == '('
			|| letter == ')')
		{
			infixQ.EnQueue(letter);
		}
	}
	cout << "infix expression: ";
	infixQ.Print_v2();

	input_file.close();
}

void algebraic_expression::convert_infix_to_postfix()
{
	char c;
	while (!infixQ.IsEmpty())
	{
		c = infixQ.Front();
		infixQ.DeQueue();

		if (isdigit(c))
			postQ.EnQueue(c);
		else if (opStack.IsEmpty())
			opStack.Push(c);
		else if (c == '(')
			opStack.Push(c);
		else if (c == ')')
		{
			while (opStack.Top() != '(')
			{
				postQ.EnQueue(opStack.Top());
				opStack.Pop();
			}
			opStack.Pop(); // discard a left parenthese from stack
		}
		else
		{
			while (!opStack.IsEmpty() 
				&& opStack.Top() != '(' 
				&& get_precedence(c) <= get_precedence(opStack.Top()))
			{
				postQ.EnQueue(opStack.Top());
				opStack.Pop();
			}
			opStack.Push(c);
		}
	}
	// Now there are no tokens left in infixQ, so transfer remaining operators
	while (!opStack.IsEmpty())
	{
		postQ.EnQueue(opStack.Top());
		opStack.Pop();
	}

	cout << "postfix expression: ";
	postQ.Print_v2();
}

int algebraic_expression::evaluate()
{
	char c;
	int topNum, nextNum, answer;

	while (!postQ.IsEmpty())
	{
		c = postQ.Front();
		postQ.DeQueue();
		if (isdigit(c))
			eval.Push(ctoi(c));
		else
		{
			topNum = eval.Top();
			eval.Pop();
			nextNum = eval.Top();
			eval.Pop();

			switch (c)
			{
			case '+':
				answer = nextNum + topNum;
				break;
			case '-':
				answer = nextNum - topNum;
				break;
			case '*':
				answer = nextNum * topNum;
				break;
			case '/':
				if (topNum == 0)
					throw exception("error: zero divisor.");
				answer = nextNum / topNum;
				break;
			default:
				throw exception("error: unknown operator: " + c);
				break;
			}
			eval.Push(answer);
		}
	}

	if (eval.IsEmpty())
		throw exception("error: invalid expression");

	return eval.Top();
}

int algebraic_expression::get_precedence(char c) const
{
	int precedence;
	switch (c)
	{
	case '+':
	case '-':
		precedence = 0;
		break;
	case '*':
	case '/':
		precedence = 1;
		break;
	default:
		throw exception("error: unknown operator: " + c);
		break;
	}

	return precedence;
}

int algebraic_expression::ctoi(char c) const
{
	return c - '0';
}

bool algebraic_expression::isdigit(char c) const
{
	return c >= '0' && c <= '9';
}

void algebraic_expression::clear()
{
	while (!infixQ.IsEmpty())
		infixQ.DeQueue();

	while (!postQ.IsEmpty())
		postQ.DeQueue();

	while (!eval.IsEmpty())
		eval.Pop();

	while (!opStack.IsEmpty())
		opStack.Pop();
}
