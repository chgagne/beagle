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
 *  \file   beagle/Distrib/Master/src/BootStrapEndOp.cpp
 *  \brief  Source code of class BootStrapEndOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Dubreuil <mdubreuil@gmail.com>
 *  $Revision: 1.6 $
 *  $Date: 2007/09/24 20:33:01 $
 */

#include "beagle/Distrib/Master.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Build bootstraping end operator.
 *  \param inName Name of the bootstaping end operator.
 *  \param inDefaultAppName default application's name.  Usually dbeagle-app.
 *  \param inDefaultVersion default client version
 */
Master::BootStrapEndOp::BootStrapEndOp(std::string inName,
                                       std::string inDefaultAppName,
                                       std::string inDefaultVersion)  :
		Operator(inName),
		mDefaultAppName(inDefaultAppName),
		mDefaultVersion(inDefaultVersion)
{ }


/*!
 *  \brief Register parameters of the bootstraping end operator.
 *  \param ioSystem Reference to the system.
 */
void Master::BootStrapEndOp::registerParams(Beagle::System& ioSystem)
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
		    "Communication retry",
		    "UInt",
		    "5",
		    "Number of retry allowed to the server before stopping."
		);
		mNumberRetry = castHandleT<UInt>(
		                   ioSystem.getRegister().insertEntry("db.comm.retry", new UInt(5), lDescription));
	}
	{
		Register::Description lDescription(
		    "Compression level",
		    "UInt",
		    "3",
		    "Evolver communication compression."
		);
		mCompressionLevel = castHandleT<UInt>(
		                        ioSystem.getRegister().insertEntry("db.evolver.compression", new UInt(0), lDescription));
	}
	{
		Register::Description lDescription(
		    "Network restart flag",
		    "Bool",
		    "0",
		    "Network restart flag. Automatically set by operator Master::BootStrapBeginOp."
		);
		mRestart = castHandleT<Bool>(
		               ioSystem.getRegister().insertEntry("db.restart", new Bool(false), lDescription));
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
 *  \brief Apply the bootstraping end operation on the deme.
 *  \param ioDeme Current deme of individuals to end bootstraping.
 *  \param ioContext Context of the evolution.
 */
void Master::BootStrapEndOp::operate(Deme& ioDeme, Context& ioContext)
{}


/*!
 *  \brief Initialize of the bootstrap ending.
 *  \param ioSystem Evolutionary system.
 */
void Master::BootStrapEndOp::init(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();

	Beagle::Operator::init(ioSystem);

	if(EvolClient::getSingleton() == NULL) {
		EvolClient::setSingleton(new EvolClient(mServerIP->getWrappedValue(),
		                                        mServerPort->getWrappedValue(),
		                                        mCompressionLevel->getWrappedValue(),
		                                        mVersion->getWrappedValue()));
	}

	Beagle_StackTraceEndM();
}

