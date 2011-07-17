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
 *  \file   beagle/ES/RecombinationOp.hpp
 *  \brief  Definition of the class ES::RecombinationOp.
 *  \author Christian Gagne
 *  $Revision: 1.4 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_ES_RecombinationOp_hpp
#define Beagle_ES_RecombinationOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"
#include "Beagle/FltVec.hpp"


namespace Beagle
{
	
namespace ES
{

/*!
 *  \class RecombinationOp Beagle/ES/RecombinationOp.hpp
 *    "Beagle/ES/RecombinationOp.hpp"
 *  \brief Real-valued recombination by averaging operator class.
 *  \ingroup ESF
 */
class RecombinationOp : public EC::RecombinationOp
{

public:

	//! ES::RecombinationOp allocator type.
	typedef AllocatorT<RecombinationOp,EC::RecombinationOp::Alloc>
	Alloc;
	//! ES::RecombinationOp handle type.
	typedef PointerT<RecombinationOp,EC::RecombinationOp::Handle>
	Handle;
	//! ES::RecombinationOp bag type.
	typedef ContainerT<RecombinationOp,EC::RecombinationOp::Bag>
	Bag;

	explicit RecombinationOp(std::string inRecombPbName="es.recomb.prob",
	                         std::string inNumberRecombName="es.recomb.nbind",
	                         std::string inName="ES-RecombinationOp");
	virtual ~RecombinationOp()
	{ }

	virtual Individual::Handle recombine(Individual::Bag& inIndivPool, Context& ioContext);

};

}
}

#endif // Beagle_ES_RecombinationOp_hpp
