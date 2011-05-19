
#include <tr1/unordered_map>

using std::tr1::unordered_map;

class TestTr1UnorderedMapClass{
public:
	TestTr1UnorderedMapClass(){}
	inline void const_Find_Test() const{
		mTestUnorderedMap.find(0);
	}
private:
	std::tr1::unordered_map<int, int> mTestUnorderedMap;
};
 
int main(int,char**) {}
