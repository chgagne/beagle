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
 *  \file   Beagle/FltVec/InitGaussianOp.hpp
 *  \brief  Definition of the class FltVec::InitGaussianOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_FltVec_InitGaussianOp_hpp
#define Beagle_FltVec_InitGaussianOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"


namespace Beagle
{

namespace FltVec
{

/*!
 *  \class InitGaussianOp Beagle/FltVec/InitGaussianOp.hpp "Beagle/FltVec/InitGaussianOp.hpp"
 *  \brief Real-valued genotype Gaussian distributed initialization operator class.
 *  \ingroup FltVecF
 */
class InitGaussianOp : public EC::InitializationOp
{

public:

	//! FltVec::InitGaussianOp allocator type.
	typedef AllocatorT<InitGaussianOp,EC::InitializationOp::Alloc>
	Alloc;
	//! FltVec::InitGaussianOp handle type.
	typedef PointerT<InitGaussianOp,EC::InitializationOp::Handle>
	Handle;
	//! FltVec::InitGaussianOp bag type.
	typedef ContainerT<InitGaussianOp,EC::InitializationOp::Bag>
	Bag;

	explicit InitGaussianOp(unsigned int inFloatVectorSize=0,
	                        std::string inReproProbaName="ec.repro.prob",
	                        std::string inName="FltVec-InitGaussianOp");
	virtual ~InitGaussianOp()
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
	DoubleArray::Handle  mMean;             //!< Mean of Gaussian distribution for initialization.
	DoubleArray::Handle  mStdev;            //!< Standard deviation of Gaussian distribution for initialization.
	UInt::Handle         mFloatVectorSize;  //!< Individual float vectors size.

};

}
}

#endif // Beagle_FltVec_InitGaussianOp_hpp
