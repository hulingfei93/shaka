#include<iostream>
#include<vector>

using namespace std;

bool IsValuePresent(vector<int>&, int);

int main(int argc, char **argv)
{
    vector<int> number_set;
    number_set.push_back(1);
    number_set.push_back(2);
    number_set.push_back(3);
    number_set.push_back(4);
    number_set.push_back(5);
    number_set.push_back(7);
    number_set.push_back(15);
    number_set.push_back(25);
    bool is_value_in_vector = IsValuePresent(number_set, 4);  // false
    is_value_in_vector = IsValuePresent(number_set, 2093);  // true

    return 0;
}

bool IsValuePresent(vector<int> &vect, int value)
{
    int low, high, mid;
    low = 0;
    high = vect.size() - 1;
    
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (vect[mid] > value)
        {
            high = mid - 1;
        }
        else if (vect[mid] < value)
        {
            low = mid + 1;
        }
        else
        {
            return true;
        }
    }
    return false;
}