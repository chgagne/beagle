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
 *  \file   Beagle/EC/RegisterReadOp.cpp
 *  \brief  Source code of class RegisterReadOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15 $
 *  $Date: 2007/09/17 17:25:49 $
 */

#include "Beagle/EC.hpp"

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#else
#include <fstream>
#endif // BEAGLE_HAVE_LIBZ

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a parameters reading operator.
 *  \param inName Name of the read parameter operator.
 */
RegisterReadOp::RegisterReadOp(std::string inName) :
	Beagle::Operator(inName),
	mReadingInterval(NULL)
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void RegisterReadOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::Operator::registerParams(ioSystem);

	{
		std::ostringstream lOSS;
		lOSS << "Interval between each register read, in number of generations. ";
		lOSS << "The name of the register file is specified by parameter ";
		lOSS << "'rg.read.filename'. When zero, the register is read only once, ";
		lOSS << "during initialization.";
		Register::Description lDescription(
		    "Register reading interval",
		    "UInt",
		    "0",
		    lOSS.str()
		);
		mReadingInterval = castHandleT<UInt>(
		                       ioSystem.getRegister().insertEntry("ec.conf.readinterval", new UInt(0), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "The name of a configuration file containing ";
		lOSS << "evolver and parameter values. A typical configuration file can ";
		lOSS << "be created with parameter 'ec.conf.dump'.";
		Register::Description lDescription(
		    "Configuration filename",
		    "String",
		    "''",
		    lOSS.str()
		);
		mFileName = castHandleT<String>(
		                ioSystem.getRegister().insertEntry("ec.conf.file", new String(""), lDescription));
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply operation.
 *  \param ioDeme Current deme (must be 0 to operate).
 *  \param ioContext Current context.
 */
void RegisterReadOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	int lInterval = mReadingInterval->getWrappedValue();
	int lGeneration = ioContext.getGeneration();
	std::string lFileName = mFileName->getWrappedValue();
	if(lInterval == 0 || lGeneration == 0 || lGeneration%lInterval != 0) return;
	if(ioContext.getDemeIndex() != 0 || lFileName == "") return;

#ifdef BEAGLE_HAVE_LIBZ
	igzstream lStream(lFileName.c_str());
#else // BEAGLE_HAVE_LIBZ
	std::ifstream lStream(lFileName.c_str());
#endif // BEAGLE_HAVE_LIBZ
	if(lStream.good() == false) {
		throw Beagle_RunTimeExceptionM(std::string("Could not open file '")+lFileName+"'");
	}
	Beagle_LogInfoM(
	    ioContext.getSystem().getLogger(),
	    std::string("Reading system in file '")+lFileName+"'"
	);
	PACC::XML::Document lDocument(lStream, lFileName);
	lStream.close();
	PACC::XML::ConstFinder lFinder(lDocument.getFirstDataTag());
	PACC::XML::ConstIterator lPos = lFinder.find("Beagle//Register");
	while(lPos) ioContext.getSystem().getRegister().readWithSystem(lPos, ioContext.getSystem());

	Beagle_StackTraceEndM();
}
