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
 *  \file   beagle/Distrib/Master/EnvironmentManager.hpp
 *  \brief  Definition of class EnvironmentManager.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Dubreuil <mdubreuil@gmail.com>
 *  $Revision: 1.3 $
 *  $Date: 2007/08/08 19:26:33 $
 */

#ifndef Beagle_Distrib_Master_EnvironmentManager_hpp
#define Beagle_Distrib_Master_EnvironmentManager_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Deme.hpp"
#include "beagle/Context.hpp"


namespace Beagle
{
namespace Distrib
{
namespace Master
{

/*!
 *  \class EnvironmentManager beagle/Distrib/EnvironmentManager.hpp "beagle/Distrib/EnvironmentManager.hpp"
 *  \brief Deme's environment manager for writing/reading XML environment.
 *  \ingroup DBMS
 */
class EnvironmentManager : public Beagle::Object
{

public:

	//! EnvironmentManager allocator type.
	typedef Beagle::AllocatorT<EnvironmentManager,Beagle::Object::Alloc>
	Alloc;
	//! EnvironmentManager handle type.
	typedef Beagle::PointerT<EnvironmentManager,Beagle::Object::Handle>
	Handle;
	//! EnvironmentManager bag type.
	typedef Beagle::ContainerT<EnvironmentManager,Beagle::Object::Bag>
	Bag;

	EnvironmentManager()
	{ }
	virtual ~EnvironmentManager()
	{ }

	virtual void readEnvironment(PACC::XML::ConstIterator inRootNode,
	                             Beagle::Deme& ioDeme,
	                             Beagle::Context& ioContext);
	virtual void writeEnvironment(PACC::XML::Streamer& ioStreamer,
	                              Beagle::Deme& ioDeme,
	                              Beagle::Context& ioContext);

	void setAsEnvManager() const;
};

}
}
}


#endif // Beagle_Distrib_Master_EnvironmentManager_hpp

