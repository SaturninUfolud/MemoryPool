#include "Tester.h"


int Tester::counter = 0;
int Tester::counter2 = 0;

Tester::Tester()
    :id(counter2)
{
    std::cout<<"Tester c-tor"<<std::endl;
    counter++;
    counter2++;
}

Tester::Tester(const Tester&source)
    :id(source.id)
{
    std::cout<<"Tester cc-tor"<<std::endl;
    counter++;
}

Tester::Tester(Tester&&source)
    :id(source.id)
{
    std::cout<<"Tester mc-tor"<<std::endl;
    counter++;
}

Tester::~Tester()
{
    std::cout<<"Tester d-tor"<<std::endl;
    counter--;
}

std::ostream&operator<<(std::ostream&out,const Tester&tester)
{
    out<<"Tester id:"<<tester.getID();
    return out;
}
