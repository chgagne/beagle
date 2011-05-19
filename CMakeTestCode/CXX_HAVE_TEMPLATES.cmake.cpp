
template<class T> class A {
	public
	:A(){}
	};

template<class T> void f(const A<T>& ){}

int main(int,char**){
	A<double> d; 
	A<int> i; 
	f(d); 
	f(i); 
	return 0;
	}
