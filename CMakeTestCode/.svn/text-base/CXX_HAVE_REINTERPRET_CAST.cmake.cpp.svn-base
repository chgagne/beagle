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

class Unrelated { 
	public : Unrelated () {} 
	};

int g (Unrelated&) { 
	return 0; 
	}

int main(int,char**){
	Derived d;
	Base& b=d;
	Unrelated& e=reinterpret_cast<Unrelated&>(b);
	return g(e);
	}