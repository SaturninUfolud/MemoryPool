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
    static int counter;
    static int counter2;
    friend std::ostream&operator<<(std::ostream&out,const Tester&tester);
};

#endif // TESTER_H
