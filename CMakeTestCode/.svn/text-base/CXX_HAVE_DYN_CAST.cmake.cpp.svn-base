#include <typeinfo>

class Base { 
    public : 
    Base () {} 
    virtual void f () = 0;
 }; 

class Derived : public Base { 
    public : Derived () {} 
    virtual void f () {} 
};

int main(int,char**) {
Derived d; 
Base& b=d; 
return dynamic_cast<Derived*>(&b) ? 0 : 1;
}
