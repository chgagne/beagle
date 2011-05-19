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
 *  \file   beagle/GA/InitFltVecOp.hpp
 *  \brief  Definition of the class GA::InitFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_InitFltVecOp_hpp
#define Beagle_GA_InitFltVecOp_hpp

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
 *  \class InitFltVecOp beagle/GA/InitFltVecOp.hpp "beagle/GA/InitFltVecOp.hpp"
 *  \brief Real-valued GA genotype uniformly distributed initialization operator class.
 *  \ingroup GAF
 *  \ingroup GAFV
 */
class InitFltVecOp : public InitializationOp
{

public:

	//! GA::InitFltVecOp allocator type.
	typedef AllocatorT<InitFltVecOp,InitializationOp::Alloc>
	Alloc;
	//! GA::InitFltVecOp handle type.
	typedef PointerT<InitFltVecOp,InitializationOp::Handle>
	Handle;
	//! GA::InitFltVecOp bag type.
	typedef ContainerT<InitFltVecOp,InitializationOp::Bag>
	Bag;

	explicit InitFltVecOp(unsigned int inFloatVectorSize=0,
	                      std::string inReproProbaName="ec.repro.prob",
	                      std::string inName="GA-InitFltVecOp");
	virtual ~InitFltVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void initIndividual(Individual& outIndividual, Context& ioContext);

	/*!
	 *  \return Float vector size to use for initialization.
	 */
	unsigned int getFloatVectorSize() const
	{
		Beagle_StackTraceBeginM();
		return mFloatVectorSize->getWrappedValue();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set Float vector size to use for initialization.
	 *  \param inFloatVectorSize New float vector size.
	 */
	void setFloatVectorSize(unsigned int inFloatVectorSize)
	{
		Beagle_StackTraceBeginM();
		mFloatVectorSize->getWrappedValue() = inFloatVectorSize;
		Beagle_StackTraceEndM();
	}

protected:

	DoubleArray::Handle  mMaxInitValue;     //!< Maximum value used to initialize floats.
	DoubleArray::Handle  mMinInitValue;     //!< Minimum value used to initialize floats.
	DoubleArray::Handle  mIncValue;         //!< Increment of valid value of GA float vectors.
	UInt::Handle         mFloatVectorSize;  //!< Individual float vectors size.

};

}
}

#endif // Beagle_GA_InitFltVecOp_hpp
