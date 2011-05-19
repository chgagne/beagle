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
 *  \file   Beagle/EC/MigrationGridOp.cpp
 *  \brief  Source code of class MigrationGridOp.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:14 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a grid migration operator,
 *  \param inName Name of the operator
 */
MigrationGridOp::MigrationGridOp(std::string inName) :
	MigrationMapOp(inName),
	mGridWidth(NULL),
	mToroidal(NULL)
{ }

/*!
 *  \brief Construct a grid migration operator,
 *  \param inSelectionOp Operator to select emigrants.
 *  \param inReplacementOp Selection operator to choose individuals to
 *    be replaced by immigrants (if NULL, individuals selected for migration will be replaced).
 *  \param inName Name of the grid migration operator,
 */
MigrationGridOp::MigrationGridOp(SelectionOp::Handle inSelectionOp,
                                 SelectionOp::Handle inReplacementOp,
                                 std::string inName) :
	MigrationMapOp(inSelectionOp, inReplacementOp, inName),
	mGridWidth(NULL),
	mToroidal(NULL)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Evolutionary system.
 *
 *  This operator registers two parameters: the width of the grid
 *  (ec.mig.gridwidth) and whether the grid will be toroidal
 *  (ec.mig.toroidal).
 */
void MigrationGridOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	MigrationMapOp::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Size of each migration",
		    "UInt",
		    "5",
		    "Number of individuals migrating between each deme, at a each migration."
		);
		mNumberMigrants = castHandleT<UInt>(
		                      ioSystem.getRegister().insertEntry("ec.mig.size", new UInt(5), lDescription));
	}
	// Grid width
	{
		std::ostringstream lOSS;
		lOSS << "Width of migration grid.  If set to zero the operator will attempt to ";
		lOSS << "form an approximately square grid.";
		Register::Description lDescription(
		    "Width of migration grid",
		    "UInt",
		    "0",
		    lOSS.str()
		);
		mGridWidth = castHandleT<UInt>(
		                 ioSystem.getRegister().insertEntry("ec.mig.gridwidth",  new UInt(0), lDescription));
	}
	// Flag to make grid toroidal
	{
		std::ostringstream lOSS;
		lOSS << "Flag to make grid toroidal. If set to true then the top of the grid ";
		lOSS << "will migrate individuals to the bottom (and vice versa) and demes on the left ";
		lOSS << "of the grid will migrate to the right (and vice versa).";
		Register::Description lDescription(
		    "Flag to make grid toroidal",
		    "Bool",
		    "1",
		    lOSS.str()
		);
		mToroidal = castHandleT<Bool>(
		                ioSystem.getRegister().insertEntry("ec.mig.toroidal", new Bool(true), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize operator by setting up the migration map.
 *  ioSystem Evolutionary system
 *
 *  This operator sets up migration so that every deme is part of a
 *  grid.  The width of the grid is provided by 'ec.mig.gridwidth',
 *  but if that is set to zero the operator will attempt to form a
 *  fairly square grid.  Deme $i$ will send individuals to the demes
 *  that are above below, left and right.  If the flag
 *  'ec.mig.toroidal' is set to true then the grid will wrap at the
 *  edges (such that the grid forms a torus)
 */
void MigrationGridOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	const unsigned int lNbDemes = mPopSize->size();
	bool lToroidal = mToroidal->getWrappedValue();

	// No toroidal grid with less than 5 demes.
	if(lToroidal && lNbDemes<5) {
		Beagle_LogBasicM(
		    ioSystem.getLogger(),
		    std::string("WARNING: In MigrationGridOp: Toroidal grids are not generated for configurations ")+
		    "of fewer than five demes; value of ec.mig.toroidal will be reset to false"
		);
		lToroidal = false;
	}
	Beagle_LogVerboseM(
	    ioSystem.getLogger(),
	    std::string("The grid will ")+((lToroidal)?("be"):("not be"))+" toroidal"
	);

	// What will be the base size of the grid?
	unsigned int lGridWidth = mGridWidth->getWrappedValue();
	if(lGridWidth == 0) {
		lGridWidth = (unsigned int)(std::ceil(std::sqrt((double)lNbDemes)));
	}
	Beagle_AssertM(lGridWidth != 0);
	const unsigned int lGridHeight = lNbDemes / lGridWidth;
	const unsigned int lGridExtras = lNbDemes % lGridWidth;
	Beagle_LogVerboseM(
	    ioSystem.getLogger(),
	    std::string("The migration grid will be ")+
	    uint2str(lGridWidth)+" demes across by "+uint2str(lGridHeight)+" demes down"+
	    ((lGridExtras==0)?(""):(std::string(" (plus ")+uint2str(lGridExtras)+" demes in an extra row)"))
	);

	// Specify the migration map by specifying the demes that provide immigrants
	mMigrationMap.resize(lNbDemes);
	for(unsigned int i=0; i<lNbDemes; ++i) {
		mMigrationMap[i].clear();
		mMigrationMap[i].resize(lNbDemes, 0);
	}
	const unsigned int lNbMigrants = mNumberMigrants->getWrappedValue();
	unsigned int lDemeIndex = 0;
	for(unsigned int i=0; (i<(lGridHeight+1)) && (lDemeIndex < lNbDemes); ++i) {
		for(unsigned int j=0; (j<lGridWidth)  && (lDemeIndex < lNbDemes); ++j) {

			Beagle_LogDebugM(
			    ioSystem.getLogger(),
			    std::string("Configuring emigrants connections for ")+uint2ordinal(lDemeIndex+1)+" deme"
			);

			// Western deme migration
			if(j != 0) {
				mMigrationMap[lDemeIndex][lDemeIndex-1] = lNbMigrants;
			} else if(lToroidal) {
				unsigned int lWesternID = lDemeIndex + lGridWidth - 1;
				if(lWesternID >= lNbDemes) lWesternID = lNbDemes-1;
				mMigrationMap[lDemeIndex][lWesternID] = lNbMigrants;
			}

			// Eastern deme migration
			if(j != (lGridWidth-1)) {
				mMigrationMap[lDemeIndex][lDemeIndex+1] = lNbMigrants;
			} else if(lToroidal) {
				mMigrationMap[lDemeIndex][i*lGridWidth] = lNbMigrants;
			}

			// Northern deme migration
			if(i != 0) {
				mMigrationMap[lDemeIndex][lDemeIndex-lGridWidth] = lNbMigrants;
			} else if(lToroidal) {
				const unsigned int lNorthDemeIndex =
				    (j<lGridExtras) ? ((lGridWidth*lGridHeight)+j) : ((lGridWidth*(lGridHeight-1))+j);
				mMigrationMap[lDemeIndex][lNorthDemeIndex] = lNbMigrants;
			}

			// Southern deme migration
			if(i == lGridHeight) {
				// This is the 'extra' row
				if(lToroidal) {
					mMigrationMap[lDemeIndex][j] = lNbMigrants;
				}
			} else if(i == (lGridHeight-1)) {
				// This could be the last row
				if(j >= lGridExtras) {
					// This is the bottom of the grid, but not on the extra row
					if(lToroidal) {
						mMigrationMap[lDemeIndex][j] = lNbMigrants;
					}
				} else {
					// There are demes directly below this one in the 'extra' row
					mMigrationMap[lDemeIndex][lDemeIndex+lGridWidth] = lNbMigrants;
				}
			} else {
				// There are demes directly below this one.
				mMigrationMap[lDemeIndex][lDemeIndex+lGridWidth] = lNbMigrants;
			}

			// Increase lDemeIndex.
			++lDemeIndex;
		}
	}

	// Call MigrationMapOp post-initialization and exit.
	MigrationMapOp::init(ioSystem);
	Beagle_StackTraceEndM();
}
