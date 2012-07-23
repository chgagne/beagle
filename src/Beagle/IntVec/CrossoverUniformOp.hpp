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
 *  \file   Beagle/IntVec/CrossoverUniformOp.hpp
 *  \brief  Definition of the class IntVec::CrossoverUniformOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_IntVec_CrossoverUniformOp_hpp
#define Beagle_IntVec_CrossoverUniformOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"


namespace Beagle
{
namespace IntVec
{

/*!
 *  \class CrossoverUniformOp Beagle/IntVec/CrossoverUniformOp.hpp
 *    "Beagle/IntVec/CrossoverUniformOp.hpp"
 *  \brief Integer-valued IntVec uniform crossover operator class.
 *  \ingroup IntVecF
 */
class CrossoverUniformOp : public EC::CrossoverUniformOpT<IntegerVector>
{

public:

	//! IntVec::CrossoverUniformOp allocator type.
	typedef AllocatorT<CrossoverUniformOp,EC::CrossoverUniformOpT<IntegerVector>::Alloc>
	Alloc;
	//! IntVec::CrossoverUniformOp handle type.
	typedef PointerT<CrossoverUniformOp,EC::CrossoverUniformOpT<IntegerVector>::Handle>
	Handle;
	//! IntVec::CrossoverUniformOp bag type.
	typedef ContainerT<CrossoverUniformOp,EC::CrossoverUniformOpT<IntegerVector>::Bag>
	Bag;

	/*!
	 *  \brief Construct integer vector IntVec uniform crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inDistribPbName Name of the distribution probability parameter used in the register.
	 *  \param inName Name of the integer vector IntVec uniform crossover operator.
	 */
	explicit CrossoverUniformOp(std::string inMatingPbName="intvec.cxunif.prob",
	                            std::string inDistribPbName="intvec.cxunif.distribprob",
	                            std::string inName="IntVec-CrossoverUniformOp") :
			EC::CrossoverUniformOpT<IntegerVector>(inMatingPbName, inDistribPbName, inName)
	{ }

	virtual ~CrossoverUniformOp()
	{ }

};

}
}

#endif // Beagle_IntVec_CrossoverUniformOp_hpp
