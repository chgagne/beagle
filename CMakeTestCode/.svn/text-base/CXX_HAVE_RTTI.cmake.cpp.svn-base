#include <typeinfo>

class Base { 
	public :
        Base () {}
        virtual int f () { return 0; }
        };

class Derived : public Base { 
	public :
        Derived () {}
        virtual int f () { return 1; }
        };

int main(int,char**){
	Derived d;
	Base *ptr = &d;
	return typeid (*ptr) == typeid (Derived);
	}
