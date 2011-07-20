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
 *  \file   Beagle/SAES/MutationQROp.hpp
 *  \brief  Definition of the class SAES::MutationQROp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_SAES_MutationQROp_hpp
#define Beagle_SAES_MutationQROp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"
#include "Beagle/SAES/MutationOp.hpp"


namespace Beagle
{
	
namespace SAES
{

/*!
 *  \class MutationQROp Beagle/SAES/MutationQROp.hpp "Beagle/SAES/MutationQROp.hpp"
 *  \brief SA-ES derandomized mutation operator class.
 *  \ingroup SAESF
 */
class MutationQROp : public SAES::MutationOp
{

public:

	//! SAES::MutationQROp allocator type.
	typedef AllocatorT<MutationQROp,SAES::MutationOp::Alloc>
	Alloc;
	//! SAES::MutationQROp handle type.
	typedef PointerT<MutationQROp,SAES::MutationOp::Handle>
	Handle;
	//! SAES::MutationQROp bag type.
	typedef ContainerT<MutationQROp,SAES::MutationOp::Bag>
	Bag;

	explicit MutationQROp(std::string inMutationPbName="saes.mut.prob",
	                      std::string inMinStrategyName="saes.mut.minstrategy",
	                      std::string inName="SAES-MutationQROp");
	virtual ~MutationQROp()
	{ }

	virtual void init(System& ioSystem);
	virtual bool mutate(Individual& ioIndividual, Context& ioContext);
	virtual void registerParams(System& ioSystem);

};

}
}

#endif // Beagle_SAES_MutationQROp_hpp
