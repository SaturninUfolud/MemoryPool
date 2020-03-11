#include "Tester.h"


int Tester::counter = 0; //Objects counter
int Tester::counter2 = 0; //ID counter

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
    source.id=-1;
    counter++;
}

Tester::~Tester()
{
    std::cout<<"Tester d-tor"<<std::endl;
    id=-2;
    counter--;
}

Tester& Tester::operator=(const Tester &source)
{
    std::cout<<"Tester ca-op="<<std::endl;

    if(this!=&source)
    {
        this->id=source.id;
    }
    else
    {
        std::cout<<"Stupid ca-op= :-("<<std::endl;
    }
    return *this;
}

Tester& Tester::operator=(Tester&&source)
{
    std::cout<<"Tester ma-op="<<std::endl;

    if(this!=&source)
    {
        this->id=source.id;
        source.id=-1;
    }
    else
    {
        std::cout<<"Stupid ma-op= :-("<<std::endl;
    }
    return *this;
}


std::ostream&operator<<(std::ostream&out,const Tester&tester)
{
    out<<"Tester id:"<<tester.getID();
    return out;
}
