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
 *  \file   beagle/GP/TermMaxHitsOp.hpp
 *  \brief  Definition of the type TermMaxHitsOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/08 19:26:36 $
 */

#ifndef Beagle_GP_TermMaxHitsOp_hpp
#define Beagle_GP_TermMaxHitsOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/TerminationOp.hpp"
#include "beagle/UInt.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class TermMaxHitsOp beagle/GP/TermMaxHitsOp.hpp "beagle/GP/TermMaxHitsOp.hpp"
 *  \brief Maximum number of hits termination operator.
 *  \ingroup GPF
 *  \ingroup GPOp
 *  \ingroup GPFit
 */
class TermMaxHitsOp : public TerminationOp
{

public:

	//! TermMaxHitsOp allocator type.
	typedef AllocatorT<TermMaxHitsOp,TerminationOp::Alloc>
	Alloc;
	//!< TermMaxHitsOp handle type.
	typedef PointerT<TermMaxHitsOp,TerminationOp::Handle>
	Handle;
	//!< TermMaxHitsOp bag type.
	typedef ContainerT<TermMaxHitsOp,TerminationOp::Bag>
	Bag;

	explicit TermMaxHitsOp(unsigned int inMaxHits=0, std::string inName="GP-TermMaxHitsOp");
	virtual ~TermMaxHitsOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual bool terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	UInt::Handle mMaxHits;          //!< Number of hits needed to stop an evolution.
	unsigned int mMaxHitsDefault;   //!< Default value of max. hits parameter.

};

}
}


#endif // Beagle_GP_TermMaxHitsOp_hpp
