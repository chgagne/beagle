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
 *  \file   Beagle/FltVec/CrossoverOnePointOp.hpp
 *  \brief  Definition of the class FltVec::CrossoverOnePointOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_FltVec_CrossoverOnePointOp_hpp
#define Beagle_FltVec_CrossoverOnePointOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"
#include "Beagle/FltVec/FloatVector.hpp"


namespace Beagle
{

namespace FltVec
{

/*!
 *  \class CrossoverOnePointOp Beagle/FltVec/CrossoverOnePointOp.hpp
 *    "Beagle/FltVec/CrossoverOnePointOp.hpp"
 *  \brief One point crossover for float vectore representation operator class.
 *  \ingroup FltVecF
 */
class CrossoverOnePointOp : public EC::CrossoverOnePointOpT<FltVec::FloatVector>
{

public:

	//! FltVec::CrossoverOnePointOp allocator type.
	typedef AllocatorT<CrossoverOnePointOp,EC::CrossoverOnePointOpT<FltVec::FloatVector>::Alloc>
	Alloc;
	//! FltVec::CrossoverOnePointOp handle type.
	typedef PointerT<CrossoverOnePointOp,EC::CrossoverOnePointOpT<FltVec::FloatVector>::Handle>
	Handle;
	//! FltVec::CrossoverOnePointOp bag type.
	typedef ContainerT<CrossoverOnePointOp,EC::CrossoverOnePointOpT<FltVec::FloatVector>::Bag>
	Bag;

	/*!
	 *  \brief Construct float vector one point crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inName Name of the float vector one point crossover operator.
	 */
	explicit CrossoverOnePointOp(std::string inMatingPbName="fltvec.cx1p.prob",
	                             std::string inName="FltVec-CrossoverOnePointOp") :
			EC::CrossoverOnePointOpT<FltVec::FloatVector>(inMatingPbName, inName)
	{ }

	virtual ~CrossoverOnePointOp()
	{ }

};

}
}

#endif // Beagle_FltVec_CrossoverOnePointOp_hpp
