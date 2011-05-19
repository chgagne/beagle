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
 *  \file   beagle/Distrib/Master/src/FetchCruncherDataOp.cpp
 *  \brief  Source code of class FetchCruncherDataOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Dubreuil <mdubreuil@gmail.com>
 *  $Revision: 1.6 $
 *  $Date: 2007/09/24 20:33:01 $
 */

#include "beagle/Distrib/Master.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Build a cruncher that fetch individuals operator.
 *  \param inName Name of the bootstaping begin operator.
 *  \param inDefaultAppName default application's name.  Usually dbeagle-app.
 *  \param inVersion Version number of the cruncher client.
 */
Master::FetchCruncherDataOp::FetchCruncherDataOp(std::string inName,
        std::string inDefaultAppName,
        std::string inDefaultVersion) :
		Operator(inName),
		mDefaultAppName(inDefaultAppName),
		mAppName(NULL),
		mDefaultVersion(inDefaultVersion),
		mVersion(NULL),
		mServerIP(NULL),
		mServerPort(NULL),
		mCompressionLevel(NULL)
{ }


/*!
 *  \brief Register parameters of the FetchCruncher operator.
 *  \param ioSystem Reference to the system.
 */
void Master::FetchCruncherDataOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::Operator::registerParams(ioSystem);
	{
		Register::Description lDescription(
		    "Application name",
		    "String",
		    std::string("'")+mDefaultAppName+std::string("'"),
		    "Name of the application, as registered in the server."
		);
		mAppName = castHandleT<Beagle::String>(
		               ioSystem.getRegister().insertEntry("db.app.name", new Beagle::String(mDefaultAppName), lDescription));
	}
	{
		Register::Description lDescription(
		    "IP server address",
		    "String",
		    "127.0.0.1",
		    "IP address of the master-slave server."
		);
		mServerIP = castHandleT<Beagle::String>(
		                ioSystem.getRegister().insertEntry("db.server.ip", new Beagle::String("127.0.0.1"), lDescription));
	}
	{
		Register::Description lDescription(
		    "IP server port",
		    "UInt",
		    "9123",
		    "IP port of the master-slave server."
		);
		mServerPort = castHandleT<UInt>(
		                  ioSystem.getRegister().insertEntry("db.server.port", new UInt(9123), lDescription));
	}
	{
		Register::Description lDescription(
		    "Compression level",
		    "UInt",
		    "3",
		    "Server communication compression."
		);
		mCompressionLevel = castHandleT<UInt>(
		                        ioSystem.getRegister().insertEntry("db.cruncher.compression", new UInt(3), lDescription));
	}
	{
		Register::Description lDescription(
		    "Client version",
		    "String",
		    std::string("'") + mDefaultVersion + std::string("'"),
		    "Client version, to tell the server if the current version is up-to-date."
		);
		mVersion = castHandleT<Beagle::String>(
		               ioSystem.getRegister().insertEntry("db.client.version", new Beagle::String(mDefaultVersion), lDescription));
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set and Receive a group of individuals.
 *  \param ioDeme Current deme of individuals to fetch into.
 *  \param ioContext Context of the evolution.
 */
void Master::FetchCruncherDataOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();

	Beagle_AssertM(EvalClient::getSingleton() != NULL);
	Beagle_AssertM(ioContext.getVivarium().size() == 1);
	EvalClient& lClient = *EvalClient::getSingleton();
	lClient.setDeme(&ioDeme);
	lClient.setContext(&ioContext);
	lClient.sendAndReceiveDeme();
	ioContext.setGeneration(lClient.getGeneration());

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize the FetchCruncher operator.
 *  \param ioSystem Evolutionary system.
 */
void Master::FetchCruncherDataOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::Operator::init(ioSystem);

	Beagle::UIntArray::Handle lPopSize=
	    castHandleT<Beagle::UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));
	lPopSize->resize(1);

	if(EvalClient::getSingleton() == NULL) {
		EvalClient::setSingleton(new EvalClient(mServerIP->getWrappedValue(),
		                                        mServerPort->getWrappedValue(),
		                                        mCompressionLevel->getWrappedValue(),
		                                        mVersion->getWrappedValue()));
	}

	Beagle_StackTraceEndM();
}







