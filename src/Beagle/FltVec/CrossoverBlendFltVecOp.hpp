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
 *  \file   beagle/GA/CrossoverBlendFltVecOp.hpp
 *  \brief  Definition of the class GA::CrossoverBlendFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CrossoverBlendFltVecOp_hpp
#define Beagle_GA_CrossoverBlendFltVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class CrossoverBlendFltVecOp beagle/GA/CrossoverBlendFltVecOp.hpp
 *    "beagle/GA/CrossoverBlendFltVecOp.hpp"
 *  \brief Real-valued GA blend crossover (BLX-alpha) operator class.
 *  \ingroup GAF
 *  \ingroup GAFV
 *
 *  Real-valued GA blend crossover (BLX-alpha) proceed by blending two float vectors,
 *  \f$(x^{(1,t)},x^{(2,t)})\f$, using a parameter \f$\alpha\in[0,1]\f$.
 *  The resulting children \f$(x^{(1,t+1)},x^{(2,t+1)})\f$ are equal to
 *  \f$x^{(1,t+1)}_i=(1-\gamma_i) x^{(1,t)}_i+\gamma_i x^{(2,t)}_i\f$ and
 *  \f$x^{(2,t+1)}_i=\gamma_i x^{(1,t)}_i+(1-\gamma_i) x^{(2,t)}_i\f$, where
 *  \f$u_i\in[0,1]\f$ is a random value and \f$\gamma_i=(1 + 2\alpha)u_i-\alpha\f$.
 *
 *  This crossover operation consists in generating two children that depends on the
 *  difference between the parents individuals.
 *
 */
class CrossoverBlendFltVecOp : public CrossoverOp
{

public:

	//! GA::CrossoverBlendFltVecOp allocator type.
	typedef AllocatorT<CrossoverBlendFltVecOp,CrossoverOp::Alloc>
	Alloc;
	//! GA::CrossoverBlendFltVecOp handle type.
	typedef PointerT<CrossoverBlendFltVecOp,CrossoverOp::Handle>
	Handle;
	//! GA::CrossoverBlendFltVecOp bag type.
	typedef ContainerT<CrossoverBlendFltVecOp,CrossoverOp::Bag>
	Bag;

	explicit CrossoverBlendFltVecOp(std::string inMatingPbName="ga.cxblend.prob",
	                                std::string inName="GA-CrossoverBlendFltVecOp");
	virtual ~CrossoverBlendFltVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
	                  Individual& ioIndiv2, Context& ioContext2);

protected:

	DoubleArray::Handle mMaxValue;  //!< Max value of GA float vectors.
	DoubleArray::Handle mMinValue;  //!< Min value of GA float vectors.
	DoubleArray::Handle mIncValue;  //!< Increment of valid value of GA float vectors.
	Double::Handle mAlpha;          //!< Blend crossover alpha parameter.

};

}
}


#endif // Beagle_GA_CrossoverBlendFltVecOp_hpp
