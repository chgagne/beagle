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
 *  \file   beagle/GP/MutationEphemeralDoubleOp.hpp
 *  \brief  Definition and implementation of class GP::MutationEphemeralDoubleOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_MutationEphemeralDoubleOp_hpp
#define Beagle_GP_MutationEphemeralDoubleOp_hpp

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
#include "beagle/GP/EphemeralT.hpp"
#include "beagle/GP/MutationEphemeralOpT.hpp"

namespace Beagle
{
namespace GP
{


/*!
 *  \class MutationEphemeralDoubleOp beagle/GP/MutationEphemeralDoubleOp.hpp
 *    "beagle/GP/MutationEphemeralDoubleOp.hpp"
 *  \brief GP ephemeral mutation operator class, change value of Double-typed
 *    ephemeral constants.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class MutationEphemeralDoubleOp : public Beagle::GP::MutationEphemeralOpT<Double>
{

public:

	//! GP::MutationEphemeralDoubleOp allocator type.
	typedef AllocatorT<MutationEphemeralDoubleOp,Beagle::GP::MutationEphemeralOpT<Double>::Alloc>
	Alloc;
	//! GP::MutationEphemeralDoubleOp handle type.
	typedef PointerT<MutationEphemeralDoubleOp,Beagle::GP::MutationEphemeralOpT<Double>::Handle>
	Handle;
	//! GP::MutationEphemeralDoubleOp bag type.
	typedef ContainerT<MutationEphemeralDoubleOp,Beagle::GP::MutationEphemeralOpT<Double>::Bag>
	Bag;

	/*!
	 *  \brief Construct a GP Double-typed ephemeral mutation operator.
	 *  \param inMutationPbName Mutation probability parameter name used in register.
	 *  \param inEphemeralNameParamName Parameter name used to state the name of the ephemeral
	 *    primitive that are mutated.
	 *  \param inName Name of the operator.
	 */
	explicit MutationEphemeralDoubleOp(std::string inMutationPbName="gp.mutephdbl.indpb",
	                                   std::string inEphemeralNameParamName="gp.mutephdbl.primit",
	                                   std::string inName="GP-MutationEphemeralDoubleOp") :
			Beagle::GP::MutationEphemeralOpT<Double>(inMutationPbName, inEphemeralNameParamName, inName)
	{ }

	virtual ~MutationEphemeralDoubleOp()
	{ }


};

}
}


#endif // Beagle_GP_MutationEphemeralDoubleOp_hpp
