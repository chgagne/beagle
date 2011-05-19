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
 *  \file   beagle/Distrib/Master/MainLoopEndOp.hpp
 *  \brief  Definition of class MainLoopEndOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Dubreuil <mdubreuil@gmail.com>
 *  $Revision: 1.5 $
 *  $Date: 2007/09/24 20:33:01 $
 */

#ifndef Beagle_Distrib_Master_MainLoopEndOp_hpp
#define Beagle_Distrib_Master_MainLoopEndOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Distrib/Master/EvolClient.hpp"


namespace Beagle
{
namespace Distrib
{
namespace Master
{

/*!
 *  \class MainLoopEndOp beagle/Distrib/MainLoopEndOp.hpp "beagle/Distrib/MainLoopEndOp.hpp"
 *  \brief Operator of MainLoopEnd.
 *  \ingroup DBMS
 */
class MainLoopEndOp : public Beagle::Operator
{

public:

	//! MainLoopEndOp allocator type.
	typedef Beagle::AllocatorT<MainLoopEndOp,Beagle::Operator::Alloc>
	Alloc;
	//! MainLoopEndOp handle type.
	typedef Beagle::PointerT<MainLoopEndOp,Beagle::Operator::Handle>
	Handle;
	//! MainLoopEndOp bag type.
	typedef Beagle::ContainerT<MainLoopEndOp,Beagle::Operator::Bag>
	Bag;

	explicit MainLoopEndOp(std::string inName="DBMS-MainLoopEndOp",
	                       std::string inDefaultAppName="dbeagle-app",
	                       std::string inDefaultVersion="");
	virtual ~MainLoopEndOp()
	{ }

	virtual void init(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void registerParams(Beagle::System& ioSystem);

protected:

	std::string             mDefaultAppName;   //!< Default application's name.
	Beagle::String::Handle  mAppName;          //!< Handle to the application's name.
	std::string             mDefaultVersion;   //!< Default version value of the client
	Beagle::String::Handle  mVersion;          //!< Handle to the version value of the client
	Beagle::String::Handle  mServerIP;         //!< Handle to the DAGS server's IP address
	Beagle::UInt::Handle    mServerPort;       //!< Handle to the DAGS server's port number
	Beagle::UInt::Handle    mNumberRetry;      //!< Handle to the number of retry before cancelling connections
	Beagle::UInt::Handle    mCompressionLevel; //!< Handle to the to compression level used for send/receive messages
	Beagle::Bool::Handle    mRestart;          //!< Handle of the application's restart mode

};

}
}
}


#endif // Beagle_Distrib_Master_MainLoopEndOp_hpp



