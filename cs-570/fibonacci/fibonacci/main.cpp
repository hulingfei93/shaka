#include <iostream>

using namespace std;

int fibonacci(int term);

int main(int argc, char **argv)
{
    int term;
    while (true)
    {
        cout << "Please input a number to compute fibonacci: ";
        cin >> term;
        if (cin.fail())
        {
            cout << "Invalid number, please input again." << endl;
            cin.clear();
            cin.sync();
            continue;
        }
        try
        {
            int result = fibonacci(term);
            cout << "fibonacci(" << term << ") = > " << result << endl;
        }
        catch (exception ex)
        {
            const char *ex_str = ex.what();
            cout << "fibonacci(" << term << ") = > " << ex_str << endl;
        }
        
    }
    return 0;
}

int fibonacci(int term)
{
    if (term < 0)
        throw exception("Exception: the number must be equal or great than zero for computing fibonacci number.");
    if (term == 0)
        return 0;
    if (term == 1)
        return 1;

    return fibonacci(term - 1) + fibonacci(term - 2);
}