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

int g (Derived&) { 
	return 0; 
	}

int main(int,char**){
	Derived d; 
	Base& b = d; 
	Derived& s = static_cast<Derived&> (b); 
	return g (s);
	}