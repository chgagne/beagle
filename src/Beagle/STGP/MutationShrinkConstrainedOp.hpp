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
 *  \file   beagle/GP/MutationShrinkConstrainedOp.hpp
 *  \brief  Definition of the class GP::MutationShrinkConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_STGP_MutationShrinkConstrainedOp_hpp
#define Beagle_STGP_MutationShrinkConstrainedOp_hpp

#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/EC/MutationOp.hpp"
#include "Beagle/Core/UInt.hpp"
#include "Beagle/Core/Float.hpp"
#include "Beagle/Core/Bool.hpp"
#include "Beagle/GP/Context.hpp"
#include "Beagle/GP/Individual.hpp"
#include "Beagle/GP/MutationShrinkOp.hpp"

namespace Beagle
{
namespace STGP
{

/*!
 *  \class MutationShrinkConstrainedOp
 *    beagle/GP/MutationShrinkConstrainedOp.hpp "beagle/GP/MutationShrinkConstrainedOp.hpp"
 *  \brief Constrained GP tree shrink mutation operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class MutationShrinkConstrainedOp : public GP::MutationShrinkOp
{

public:

	//! GP::MutationShrinkConstrainedOp allocator type.
	typedef AllocatorT<MutationShrinkConstrainedOp,GP::MutationShrinkOp::Alloc>
	Alloc;
	//! GP::MutationShrinkConstrainedOp handle type.
	typedef PointerT<MutationShrinkConstrainedOp,GP::MutationShrinkOp::Handle>
	Handle;
	//! GP::MutationShrinkConstrainedOp bag type.
	typedef ContainerT<MutationShrinkConstrainedOp,GP::MutationShrinkOp::Bag>
	Bag;

	explicit MutationShrinkConstrainedOp(std::string inMutationPbName="gp.mutshrink.indpb",
	                                     std::string inName="GP-MutationShrinkConstrainedOp");
	virtual ~MutationShrinkConstrainedOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);

protected:

	UInt::Handle  mNumberAttempts;   //!< Number of attempts to shrink mutation an individual.

};

}
}

#endif // Beagle_GP_MutationShrinkConstrainedOp_hpp
