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
 *  \file   beagle/GA/CrossoverSBXFltVecOp.hpp
 *  \brief  Definition of the class GA::CrossoverSBXFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CrossoverSBXFltVecOp_hpp
#define Beagle_GA_CrossoverSBXFltVecOp_hpp

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
 *  \class CrossoverSBXFltVecOp beagle/GA/CrossoverSBXFltVecOp.hpp
 *    "beagle/GA/CrossoverSBXFltVecOp.hpp"
 *  \brief Real-valued GA simulated binary crossover (SBX) operator class.
 *  \ingroup GAF
 *  \ingroup GAFV
 *
 *  Real-valued GA simulated binary crossover (SBX) proceed by mating two
 *  float vectors, \f$(x^{(1,t)},x^{(2,t)})\f$. The resulting children
 *  \f$(x^{(1,t+1)},x^{(2,t+1)})\f$ are equal to
 *  \f$x^{(1,t+1)}_i=0.5((1+\beta_i) x^{(1,t)}_i+(1-\beta_i) x^{(2,t)}_i)\f$ and
 *  \f$x^{(2,t+1)}_i=0.5((1-\beta_i) x^{(1,t)}_i+(1+\beta_i) x^{(2,t)}_i)\f$, where
 *  \f$u_i\in[0,1]\f$ is a random value, \f$\beta_i=(2u_i)^{1/\nu+1}\f$ if \f$u_i\le0.5\f$
 *  otherwize \f$(\frac{1}{2(1-u_i)})^{1/\nu+1}\f$, and \f$\nu>0\f$ is user-configurable
 *  parameter.
 *
 *  Reference: Deb K., Beyer H.-G. (2001). Self-Adaptive Genetic Algorithms with
 *  Simulated Binary Crossover. Evolutionary Computation, 9(2), pp. 197-221.
 *
 */
class CrossoverSBXFltVecOp : public CrossoverOp
{

public:

	//! GA::CrossoverSBXFltVecOp allocator type.
	typedef AllocatorT<CrossoverSBXFltVecOp,CrossoverOp::Alloc>
	Alloc;
	//! GA::CrossoverSBXFltVecOp handle type.
	typedef PointerT<CrossoverSBXFltVecOp,CrossoverOp::Handle>
	Handle;
	//! GA::CrossoverSBXFltVecOp bag type.
	typedef ContainerT<CrossoverSBXFltVecOp,CrossoverOp::Bag>
	Bag;

	explicit CrossoverSBXFltVecOp(std::string inMatingPbName="ga.cxsbx.prob",
	                              std::string inName="GA-CrossoverSBXFltVecOp");
	virtual ~CrossoverSBXFltVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
	                  Individual& ioIndiv2, Context& ioContext2);

protected:

	DoubleArray::Handle mMaxValue;  //!< Max value of GA float vectors.
	DoubleArray::Handle mMinValue;  //!< Min value of GA float vectors.
	Double::Handle mNu;             //!< SBX Nu parameter.

};

}
}

#endif // Beagle_GA_CrossoverSBXFltVecOp_hpp
