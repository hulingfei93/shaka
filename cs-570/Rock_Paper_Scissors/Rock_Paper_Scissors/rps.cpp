#include <iostream>

using namespace std;

class Tool 
{
public:
    Tool(int strength, char type)
    {
        this->strength = strength;
        this->type = type;
    }
    void setStrength(int strength)
    {
        this->strength = strength;
    }
    int getStrength() const 
    {
        return this->strength;
    }
    char getType() const
    {
        return this->type;
    }

private:
    int strength;
    char type;
};

class Rock : public Tool
{
public:
    Rock(int strength) : Tool(strength, 'r'){}
    bool fight(Tool tool) const
    {
        int temp_strength = getStrength();
        if (tool.getType() == 's')
        {
            temp_strength *= 2;
        }
        else if (tool.getType() == 'p')
        {
            temp_strength /= 2;
        }
        return temp_strength >= tool.getStrength();
    }
};

class Paper : public Tool
{
public:
    Paper(int strength) : Tool(strength, 'p'){}
    bool fight(Tool tool) const
    {
        int temp_strength = getStrength();
        if (tool.getType() == 'r')
        {
            temp_strength *= 2;
        }
        else if (tool.getType() == 's')
        {
            temp_strength /= 2;
        }
        return temp_strength >= tool.getStrength();
    }
};

class Scissors : public Tool
{
public:
    Scissors(int strength) : Tool(strength, 's'){}
    bool fight(Tool tool) const
    {
        int temp_strength = getStrength();
        if (tool.getType() == 'p')
        {
            temp_strength *= 2;
        }
        else if (tool.getType() == 'r')
        {
            temp_strength /= 2;
        }
        return temp_strength >= tool.getStrength();
    }
};

int main() {
    // Example main function
    // You may add your own testing code if you like

    Scissors s1(5);
    Paper p1(7);
    Rock r1(15);
    cout << s1.fight(p1) << p1.fight(s1) << endl;
    cout << p1.fight(r1) << r1.fight(p1) << endl;
    cout << r1.fight(s1) << s1.fight(r1) << endl;

    return 0;
}
