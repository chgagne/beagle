
template<class X> class A { 
	public : 
	typedef X W; 
	};

template<class Y> class B {};

template<class Y> void operator+(B<Y> d1, typename Y::W d2) {}

int main(int,char**){
	B<A<float> > z; 
	z + 0.5f; 
	return 0;
	}
