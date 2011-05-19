
#include <vector>
#include <cmath>
#include "PACC/Math/QRandSequencer.hpp"


int main(int, char**){
	PACC::QRandSequencer RandSeqTest;
	std::vector<long> vecteurIn(5, 1.4142);
	RandSeqTest.getIntegerSequence(vecteurIn, 0.5, 1.5);
	return 0;
} 
