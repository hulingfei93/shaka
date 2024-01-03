#include "reverse_polish_notation.h"

const size_t reverse_polish_notation::BUFFER_SIZE = 1000;

reverse_polish_notation::reverse_polish_notation() :_buffer(BUFFER_SIZE)
{
}

reverse_polish_notation::~reverse_polish_notation()
{
}

void reverse_polish_notation::start()
{
    /*
    char cmd;
    while (true)
    {
        cout << "**********************************************" << endl;
        cout << "****  Reverse Polish Notation Calculator  ****" << endl;
        cout << "**********************************************" << endl;

        cout << "Load input file(L or l), Exit(E or e): ";
        cin >> cmd;
        switch (tolower(cmd))
        {
        case 'l':
            execute();
            break;
        case 'e':
            return;
            break;
        default:
            break;
        }
    }*/

    while (true)
    {
        cout << "**********************************************" << endl;
        cout << "****  Reverse Polish Notation Calculator  ****" << endl;
        cout << "**********************************************" << endl;

        execute();
    }
}

void reverse_polish_notation::execute()
{
    get_data();
    compute();
    output_result();
}

void reverse_polish_notation::get_data()
{
    results.clear();
    while (true)
    {
        cout << "Please input a file path which contains Reverse Polish Notations: ";
        cin >> file_path;

        try
        {
            ifstream data_file;
            data_file.open(file_path);
            if (!data_file)
            {
                cout << "The data file '" << file_path << "' does not exist." << endl;
                continue;
            }

            string line;
            while (getline(data_file, line))
            {
                result_item item;
                item.RPN = line;
                item.value = 0.0;
                item.is_valid = false;
                results.push_back(item);
            }
            return;
        }
        catch (exception ex)
        {
            cout << "Read data file '" << file_path << "' failure." << endl;
        }
        
    }
}

void reverse_polish_notation::compute()
{
    for (vector<result_item>::iterator iter = results.begin(); iter != results.end(); ++iter)
    {
        try
        {
            iter->value = evaluate(iter->RPN);
            iter->is_valid = true;
        }
        catch (exception ex)
        {
            iter->is_valid = false;
        }
    }
}

void reverse_polish_notation::output_result()
{
    if (results.size() == 0)
        cout << "No results: there are no Reverse Polish Notations in file '" << file_path <<"'." << endl;
    else
    {
        for (vector<result_item>::iterator iter = results.begin(); iter != results.end(); ++iter)
        {
            if (iter->is_valid)
                cout << iter->value << endl;
            else
                cout << "SYNTAX ERROR" << endl;
        }
    }
}

double reverse_polish_notation::evaluate(string& RPN)
{
    stack<double> eval;
    deque<string> postfix_q;
    string str;
    double second_operand, first_operand, answer;
    
    
    if (!parse(RPN, postfix_q))
        throw exception("SYNTAX ERROR");

    while (!postfix_q.empty())
    {
        str = postfix_q.front();
        postfix_q.pop_front();

        if (is_decimal_char(str[0]))    // operand
            eval.push(atof(str.c_str()));
        else                            // operator
        {
            if (eval.empty())
                throw exception("SYNTAX ERROR");
            second_operand = eval.top();
            eval.pop();

            if (eval.empty())
                throw exception("SYNTAX ERROR");
            first_operand = eval.top();
            eval.pop();

            if (str == "+")
            {
                answer = first_operand + second_operand;
            }
            else if (str == "-")
            {
                answer = first_operand - second_operand;
            }
            else if (str == "*")
            {
                answer = first_operand * second_operand;
            }
            else if (str == "/")
            {
                if (second_operand == 0.0f)
                    throw exception("ERROR: zero divisor.");
                answer = first_operand / second_operand;
            }
            else if (str == "%")
            {
                if (second_operand == 0.0f)
                    throw exception("ERROR: zero divisor.");
                answer = fmod(first_operand, second_operand);
            }
            else if (str == "pow")
            {
                answer = pow(first_operand, second_operand);
            }
            eval.push(answer);
        }
    }

    if (eval.empty() || eval.size() > 1)
        throw exception("SYNTAX ERROR");

    return eval.top();
}

bool reverse_polish_notation::parse(string& RPN, deque<string>& postfix_q)
{
    _buffer.clear();
    int length = RPN.length();
    int i = 0;
    while(i < length)
    {
        char c = RPN[i];
        if (is_whitespace(c))
        {
            if (!_buffer.is_empty())
                postfix_q.push_back(_buffer.get());
            ++i;
        }
        else if (is_decimal_char(c))
        {
            _buffer.unget(c);
            ++i;
        }
        else if (is_operator(c))
        {
            if (!_buffer.is_empty())
                postfix_q.push_back(_buffer.get());

            if (c == 'p')   // special process for "pow" operator
            {
                if (i + 2 >= length || RPN.substr(i, 3) != "pow")
                    return false;
                else
                {
                    postfix_q.push_back("pow");
                    i += 3;
                }
            }
            else
            {
                string op(1, c);
                postfix_q.push_back(op);
                ++i;
            }
        }
        else
        {
            return false;
        }
    }

    if (!_buffer.is_empty())
        postfix_q.push_back(_buffer.get());

    return true;
}

bool reverse_polish_notation::is_decimal_char(char c)
{
    return (c >= '0' && c <= '9') || c == '.';
}

bool reverse_polish_notation::is_whitespace(char c)
{
    return c == ' ' || c == '\t';
}

bool reverse_polish_notation::is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c== 'p';
}