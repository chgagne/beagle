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
 *  \file   Beagle/Core/QuasiRandom.cpp
 *  \brief  Source code of class QuasiRandom.
 *  \author Christian Gagne
 *  $Revision: 1.4 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/Core.hpp"

#include <cmath>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a low discrepancy number generator.
 *  \param inDimensionality Dimensionality of the numbers generated.
 *  \param inRand Random number generator used to scramble the generator.
 */
QuasiRandom::QuasiRandom(unsigned int inDimensionality, PACC::Randomizer& inRand) :
	Component("QuasiRandom"),
	PACC::QRandSequencer(inDimensionality, inRand)
{ }


/*!
 *  \brief Read the state of the quasi-random generator from a XML subtree.
 *  \param inIter XML iterator to read the mersenne twister randomizer from.
 *  \param ioSystem
 */
void QuasiRandom::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="QuasiRandom"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <QuasiRandom> expected!");
	PACC::XML::ConstIterator lChild = inIter->getFirstChild();
	if(lChild) {
		if(lChild->getType()!=PACC::XML::eString)
			throw Beagle_IOExceptionNodeM(*lChild, "expected quasi-random state!");
		setState(lChild->getValue());
	} else reset(0);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the state of the quasi-random generator into a XML streamer.
 *  \param ioStreamer XML streamer to write the randomizer into.
 *  \param inIndent Whether output should be indented.
 */
void QuasiRandom::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	if(getDimensionality() != 0) ioStreamer.insertStringContent(getState());
	Beagle_StackTraceEndM();
}
