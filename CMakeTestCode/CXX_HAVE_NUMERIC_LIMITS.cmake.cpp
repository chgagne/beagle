#include <limits>
#ifdef HAVE_NAMESPACES

using namespace std;
#endif

int main(int,char**){
	double e = std::numeric_limits<double>::epsilon(); 
	return 0;
	}
