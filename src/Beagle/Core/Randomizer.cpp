/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/Core/Randomizer.cpp
 *  \brief  Source code of class Randomizer.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2007/09/13 13:17:43 $
 */

#include "Beagle/Core.hpp"

#include <cmath>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a randomizer.
 *  \param inSeed Random number generator seed, can be overwritten by the registered seed.
 */
Randomizer::Randomizer(unsigned long inSeed) :
	Component("Randomizer"),
	PACC::Randomizer(inSeed),
	mRegisteredSeed(NULL),
	mSeed(*state)
{ }


/*!
 *  \brief Initialize this randomizer.
 *  \param ioSystem Evolutionary system.
 */
void Randomizer::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
	OpenMP::Handle lOpenMP = castHandleT<OpenMP>(ioSystem.getComponent("OpenMP"));
	if((*mRegisteredSeed)[lOpenMP->getThreadNum()] == 0) {
#else
	if(mRegisteredSeed->getWrappedValue() == 0) {
#endif
#ifdef BEAGLE_HAVE_DEV_URANDOM
		std::ifstream lURandom("/dev/urandom", std::ios::binary);
		lURandom.read((char*)&mSeed, sizeof(unsigned long));
		lURandom.close();
#else // BEAGLE_HAVE_DEV_URANDOM
		mSeed = MTRand::hash(time(NULL), clock());
#endif // BEAGLE_HAVE_DEV_URANDOM
		MTRand::seed(mSeed);
#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
		(*mRegisteredSeed)[lOpenMP->getThreadNum()] = mSeed;
	} else if((*mRegisteredSeed)[lOpenMP->getThreadNum()] != mSeed) {
		seed((*mRegisteredSeed)[lOpenMP->getThreadNum()]);
		mSeed = (*mRegisteredSeed)[lOpenMP->getThreadNum()];
	}
	Beagle_LogBasicM(
	    ioSystem.getLogger(),
	    std::string("Randomizer (")+uint2str(lOpenMP->getThreadNum())+std::string(") seed used: ")+uint2str(mSeed)
	);
#else
		mRegisteredSeed->getWrappedValue() = mSeed;
	} else if(mRegisteredSeed->getWrappedValue() != mSeed)
	{
		seed(mRegisteredSeed->getWrappedValue());
		mSeed = mRegisteredSeed->getWrappedValue();
	}
	Beagle_LogBasicM(
	    ioSystem.getLogger(),
	    std::string("Randomizer seed used: ")+uint2str(mSeed)
	);
#endif

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read the state of the randomizer from a XML subtree.
 *  \param inIter XML iterator to read the mersenne twister randomizer from.
 *  \param ioSystem
 */
void Randomizer::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Randomizer"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Randomizer> expected!");

	string lSeedStr = inIter->getAttribute("seed");
	if(lSeedStr.empty())
		throw Beagle_IOExceptionNodeM(*inIter, "expected randomizer seed!");
	mSeed = str2uint(lSeedStr);

	if(mSeed!=0) {
		PACC::XML::ConstIterator lChild = inIter->getFirstChild();
		if((!lChild) || (lChild->getType()!=PACC::XML::eString))
			throw Beagle_IOExceptionNodeM(*lChild, "expected randomizer state!");
		setState(lChild->getValue());
	}
	Beagle_StackTraceEndM();
}


/*!
*  \brief Register the randomizer seed parameter. The initial seed value is the timer.
 *  \param ioSystem Reference to the system.
 */
void Randomizer::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	std::ostringstream lStream;
	lStream << "Randomizer seed. A zero value means that ";
#ifdef BEAGLE_HAVE_DEV_URANDOM
	lStream << "the seed will be initialized using the /dev/urandom device.";
#else // BEAGLE_HAVE_DEV_URANDOM
	lStream << "the seed will be initialized using the current system time.";
#endif // BEAGLE_HAVE_DEV_URANDOM

#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
	Register::Description lDescription(
	    "Randomizer seed",
	    "ULongArray",
	    "0",
	    lStream.str()
	);
	mRegisteredSeed = castHandleT<ULongArray>(
	                      ioSystem.getRegister().insertEntry("ec.rand.seed", new ULongArray(1,0), lDescription));
#else
	Register::Description lDescription(
	    "Randomizer seed",
	    "ULong",
	    "0",
	    lStream.str()
	);
	mRegisteredSeed = castHandleT<ULong>(
	                      ioSystem.getRegister().insertEntry("ec.rand.seed", new ULong(0), lDescription));
#endif
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the state of the randomizer into a XML streamer.
 *  \param ioStreamer XML streamer to write the randomizer into.
 *  \param inIndent Whether output should be indented.
 */
void Randomizer::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertAttribute("seed", uint2str(mSeed));
	if(mSeed!=0) ioStreamer.insertStringContent(getState());
	Beagle_StackTraceEndM();
}
