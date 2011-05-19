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
 *  \file   beagle/GA/InitESVecOp.hpp
 *  \brief  Definition of the class GA::InitESVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_InitESVecOp_hpp
#define Beagle_GA_InitESVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/Float.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class InitESVecOp beagle/GA/InitESVecOp.hpp "beagle/GA/InitESVecOp.hpp"
 *  \brief ES genotype initialization operator class.
 *  \ingroup GAF
 *  \ingroup GAES
 *
 *  Initialization is based on one parameter, the initial strategy value (es.init.strategy). Each
 *  strategy parameters (\f$sigma_{init}\f$) are initialized to this value, while the value of the ES 
 *  vector is randomly initialized to a value following a Gaussian distribution (\f$N(\mu,\sigma)\f$)
 *  of zero mean (\f$\mu=0.0\f$) and a standard deviation of the initial stategy parameter value
 *  (\f$\sigma=\sigma_{init}\f$).
 *
 */
class InitESVecOp : public InitializationOp
{

public:

	//! GA::InitESVecOp allocator type.
	typedef AllocatorT<InitESVecOp,InitializationOp::Alloc>
	Alloc;
	//! GA::InitESVecOp handle type.
	typedef PointerT<InitESVecOp,InitializationOp::Handle>
	Handle;
	//! GA::InitESVecOp bag type.
	typedef ContainerT<InitESVecOp,InitializationOp::Bag>
	Bag;

	explicit InitESVecOp(unsigned int inESVectorSize=0,
	                     std::string inReproProbaName="ec.repro.prob",
	                     std::string inName="GA-InitESVecOp");
	virtual ~InitESVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void initIndividual(Individual& outIndividual, Context& ioContext);

	/*!
	 *  \return ES vector size to use for initialization.
	 */
	unsigned int getESVectorSize() const
	{
		Beagle_StackTraceBeginM();
		return mESVectorSize->getWrappedValue();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set ES vector size to use for initialization.
	 *  \param inESVectorSize New ES vector size.
	 */
	void setESVectorSize(unsigned int inESVectorSize)
	{
		Beagle_StackTraceBeginM();
		mESVectorSize->getWrappedValue() = inESVectorSize;
		Beagle_StackTraceEndM();
	}

protected:

	DoubleArray::Handle  mMaxValue;           //!< Maximum of ES vector values.
	DoubleArray::Handle  mMinValue;           //!< Minimum of ES vector values.
	UInt::Handle         mESVectorSize;       //!< Individual float vectors size.
	Double::Handle       mInitStrategyValue;  //!< Mutation strategy value at initialization.

};

}
}

#endif // Beagle_GA_InitESVecOp_hpp
