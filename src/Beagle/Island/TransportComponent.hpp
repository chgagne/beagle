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
 *  \file   beagle/TransportComponent.hpp
 *  \brief  Definition of the class TransportComponent.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/10 20:32:20 $
 */

#ifndef Beagle_Distrib_Island_TransportComponent_hpp
#define Beagle_Distrib_Island_TransportComponent_hpp

#include <vector>
#include <deque>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Float.hpp"
#include "beagle/Distrib/Island/SendOp.hpp"



namespace Beagle
{
namespace Distrib
{
namespace Island
{

/*!
 *  \class TransportComponent beagle/TransportComponent.hpp "beagle/TransportComponent.hpp"
 *  \brief System component to allow the sharing of selected
 *    individuals between TransportSendOp and TransportReceiveOp.
 *  \ingroup DBIM
 */
class TransportComponent : public Component
{

public:

	//! TransportComponent allocator type.
	typedef AllocatorT<TransportComponent,Component::Alloc>
	Alloc;
	//! TransportComponent handle type.
	typedef PointerT<TransportComponent,Component::Handle>
	Handle;
	//! TransportComponent bag type.
	typedef ContainerT<TransportComponent,Component::Bag>
	Bag;

	explicit TransportComponent(const std::string& inName="DBIM-TransportComponent");

	void getIndices(const unsigned int inNumberToGet,
	                std::vector< unsigned int >& ioIndices,
	                const unsigned int inDemeIndex);
	void insertIndices(const std::vector< unsigned int >& inIndices,
	                   const unsigned int inDemeIndex);
	virtual void init(System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	inline unsigned int getNumberIndices(const unsigned int inDemeIndex) const
	{
		return mIndices[inDemeIndex].size();
	}

protected:

	std::vector< std::deque< unsigned int > > mIndices;

};

} // End of namespace Beagle::Distrib::Island
} // End of namespace Beagle::Distrib
} // End of namespace Beagle


#endif // Beagle_Distrib_Island_TransportComponent_hpp
