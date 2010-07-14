/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
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

	Beagle_StackTraceEndM("void MigrationOp::registerParams(System&)");
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
	    "migration", "Beagle::MigrationOp",
	    std::string("Migrating individuals from the ")+uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
	);
	migrate(ioDeme, ioContext);

	Beagle_StackTraceEndM("void MigrationOp::operate(Deme&, Context&)");
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
		    "migration", "Beagle::MigrationOp",
		    std::string("WARNING: Migration can't occur because there are fewer than two demes. Consider")+
		    std::string(" either, removing the migration operator by editing your configuration file, or")+
		    std::string(" adding demes to the vivarium through the register variable 'ec.pop.size'.")
		);
	}

	Beagle_StackTraceEndM("void MigrationOp::init(System&)");
}
