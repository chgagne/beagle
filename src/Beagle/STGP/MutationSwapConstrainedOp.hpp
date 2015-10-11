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
 *  \file   beagle/GP/MutationSwapConstrainedOp.hpp
 *  \brief  Definition of the class GP::MutationSwapConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_MutationSwapConstrainedOp_hpp
#define Beagle_GP_MutationSwapConstrainedOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Float.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Individual.hpp"
#include "beagle/GP/MutationSwapOp.hpp"

namespace Beagle
{
namespace GP
{

/*!
 *  \class MutationSwapConstrainedOp
 *    beagle/GP/MutationSwapConstrainedOp.hpp "beagle/GP/MutationSwapConstrainedOp.hpp"
 *  \brief Constrained GP tree swap mutation operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class MutationSwapConstrainedOp : public MutationSwapOp
{

public:

	//! GP::MutationSwapConstrainedOp allocator type.
	typedef AllocatorT<MutationSwapConstrainedOp,MutationSwapOp::Alloc>
	Alloc;
	//! GP::MutationSwapConstrainedOp handle type.
	typedef PointerT<MutationSwapConstrainedOp,MutationSwapOp::Handle>
	Handle;
	//! GP::MutationSwapConstrainedOp bag type.
	typedef ContainerT<MutationSwapConstrainedOp,MutationSwapOp::Bag>
	Bag;

	explicit MutationSwapConstrainedOp(std::string inMutationPbName="gp.mutswap.indpb",
	                                   std::string inDistribPbName="gp.mutswap.distrpb",
	                                   std::string inName="GP-MutationSwapConstrainedOp");
	virtual ~MutationSwapConstrainedOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);

protected:

	UInt::Handle  mNumberAttempts;     //!< Number of attempts to swap mutate an individual.

};

}
}

#endif // Beagle_GP_MutationSwapConstrainedOp_hpp
