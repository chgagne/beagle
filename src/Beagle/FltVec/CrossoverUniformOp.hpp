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
 *  \file   Beagle/FltVec/CrossoverUniformOp.hpp
 *  \brief  Definition of the class FltVec::CrossoverUniformOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_FltVec_CrossoverUniformOp_hpp
#define Beagle_FltVec_CrossoverUniformOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"
#include "Beagle/FltVec/FloatVector.hpp"


namespace Beagle
{

namespace FltVec
{

/*!
 *  \class CrossoverUniformOp Beagle/FltVec/CrossoverUniformOp.hpp
 *    "Beagle/FltVec/CrossoverUniformOp.hpp"
 *  \brief Real-valued uniform crossover operator class.
 *  \ingroup FltVecF
 */
class CrossoverUniformOp : public EC::CrossoverUniformOpT<FltVec::FloatVector>
{

public:

	//! FltVec::CrossoverUniformOp allocator type.
	typedef AllocatorT<CrossoverUniformOp,EC::CrossoverUniformOpT<FltVec::FloatVector>::Alloc>
	Alloc;
	//! FltVec::CrossoverUniformOp handle type.
	typedef PointerT<CrossoverUniformOp,EC::CrossoverUniformOpT<FltVec::FloatVector>::Handle>
	Handle;
	//! FltVec::CrossoverUniformOp bag type.
	typedef ContainerT<CrossoverUniformOp,EC::CrossoverUniformOpT<FltVec::FloatVector>::Bag>
	Bag;

	/*!
	 *  \brief Construct float vector uniform crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inDistribPbName Name of the distribution probability parameter used in the register.
	 *  \param inName Name of the float vector GA uniform crossover operator.
	 */
	explicit CrossoverUniformOp(std::string inMatingPbName="fltvec.cxunif.prob",
	                            std::string inDistribPbName="fltvec.cxunif.distribprob",
	                            std::string inName="FltVec-CrossoverUniformOp") :
			EC::CrossoverUniformOpT<FltVec::FloatVector>(inMatingPbName, inDistribPbName, inName)
	{ }

	virtual ~CrossoverUniformOp()
	{ }

};

}
}

#endif // Beagle_FltVec_CrossoverUniformOp_hpp
