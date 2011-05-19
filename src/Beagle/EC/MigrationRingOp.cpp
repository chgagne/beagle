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
 *  \file   Beagle/EC/MigrationRingOp.cpp
 *  \brief  Source code of class MigrationRingOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.13 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/EC.hpp"

using namespace std;
using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a ring migration operator,
 *  \param inName Name of the operator
 */
MigrationRingOp::MigrationRingOp(std::string inName) :
	Beagle::EC::MigrationMapOp(new Beagle::EC::SelectRandomOp, NULL, inName),
	mNumberMigrants(NULL)
{ }


/*!
 *  \brief Construct a ring migration operator,
 *  \param inSelectionOp Operator to select emigrants.
 *  \param inReplacementOp Selection operator to choose individuals to
 *    be replaced by immigrants (if NULL, individuals selected for migration will be replaced).
 *  \param inName Name of the ring migration operator,
 */
MigrationRingOp::MigrationRingOp(SelectionOp::Handle inSelectionOp,
                                 SelectionOp::Handle inReplacementOp,
                                 std::string inName) :
	Beagle::EC::MigrationMapOp(inSelectionOp, inReplacementOp, inName),
	mNumberMigrants(NULL)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void MigrationRingOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle::EC::MigrationMapOp::registerParams(ioSystem);
	Register::Description lDescription(
	    "Size of each migration",
	    "UInt",
	    "5",
	    "Number of individuals migrating between each deme, at a each migration."
	);
	mNumberMigrants = castHandleT<UInt>(
	                      ioSystem.getRegister().insertEntry("ec.mig.size", new UInt(5), lDescription));
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this operator by setting up the migration map.
 *  \param ioSystem Evolutionary system
 *
 *  This operator sets up migration so that every deme is part of a
 *  ring.  Deme $i$ will send individuals to deme $i+1$. The last
 *  deme will send to the first.
 */
void MigrationRingOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// Create migration map for a ring migration with given number of migrants.
	const unsigned int lNbDeme = mPopSize->size();
	const unsigned int lNbMigrants = mNumberMigrants->getWrappedValue();
	mMigrationMap.resize(lNbDeme);
	for(unsigned int i=0; i<lNbDeme; ++i) {
		mMigrationMap[i].clear();
		mMigrationMap[i].resize(lNbDeme, 0);
		const unsigned int lNextDemeIndex = (i==(lNbDeme-1)) ? 0 : i+1;
		mMigrationMap[i][lNextDemeIndex] = lNbMigrants;
	}

	MigrationMapOp::init(ioSystem);
	Beagle_StackTraceEndM();
}
