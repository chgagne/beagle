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
 *  \file   Beagle/EC/MigrationOp.cpp
 *  \brief  Source code of class MigrationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.24 $
 *  $Date: 2007/08/17 18:09:14 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a migration operator.
 *  \param inName Name of the migration operator.
 */
MigrationOp::MigrationOp(std::string inName) :
	Beagle::Operator(inName),
	mMigrationInterval(NULL),
	mPopSize(NULL)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void MigrationOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::Operator::registerParams(ioSystem);

	{
		std::ostringstream lOSS;
		lOSS << "Interval between each migration, in number of generations. ";
		lOSS <<"An interval of 0 disables migration.";
		Register::Description lDescription(
		    "Interval between migrations",
		    "UInt",
		    "1",
		    lOSS.str()
		);
		mMigrationInterval = castHandleT<UInt>(
		                         ioSystem.getRegister().insertEntry("ec.mig.interval", new UInt(1), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Number of demes and size of each deme of the population. ";
		lOSS << "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
		lOSS << "The size of the UIntArray is the number of demes present in the ";
		lOSS << "vivarium, while each value of the vector is the size of the corresponding ";
		lOSS << "deme.";
		Register::Description lDescription(
		    "Vivarium and demes sizes",
		    "UIntArray",
		    "100",
		    lOSS.str()
		);
		mPopSize = castHandleT<UIntArray>(
		               ioSystem.getRegister().insertEntry("ec.pop.size", new UIntArray(1,100), lDescription));
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the migration operator on a deme.
 *  \param ioDeme Deme to migrate.
 *  \param ioContext Evolutionary context.
 */
void MigrationOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	unsigned int lMigrationInterval = mMigrationInterval->getWrappedValue();
	if((lMigrationInterval == 0) || (ioContext.getGeneration() == 0)) return;
	if((ioContext.getGeneration() % lMigrationInterval) != 0) return;
	if(mPopSize->size() < 2) return;
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("Migrating individuals from the ")+uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	migrate(ioDeme, ioContext);

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void MigrationOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if(mPopSize->size() < 2) {
		Beagle_LogBasicM(
		    ioSystem.getLogger(),
		    std::string("WARNING: Migration can't occur because there are fewer than two demes. Consider")+
		    std::string(" either, removing the migration operator by editing your configuration file, or")+
		    std::string(" adding demes to the vivarium through the register variable 'ec.pop.size'.")
		);
	}

	Beagle_StackTraceEndM();
}
