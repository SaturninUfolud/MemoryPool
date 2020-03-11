#ifndef TESTER_H
#define TESTER_H

#include <iostream>

class Tester
{
public:
    Tester();
    Tester(const Tester&source);
    Tester(Tester&&source);
    ~Tester();

    Tester& operator=(const Tester&source);
    Tester& operator=(Tester&&source);

    static int getCounter()
    {
        return counter;
    }

    int getID()const
    {
        return this->id;
    }

    bool operator>(const Tester&t2)const
    {
        return this->id>t2.id;
    }
    bool operator<(const Tester&t2)const
    {
        return this->id<t2.id;
    }

    bool operator==(const Tester&t2)const
    {
        return this->id==t2.id;
    }

private:
    int id;
    static int counter; //Objects counter
    static int counter2; //ID counter
    friend std::ostream&operator<<(std::ostream&out,const Tester&tester);
};

#endif // TESTER_H
