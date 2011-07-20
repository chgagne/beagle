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
 *  \file   Beagle/SAES/InitializationOp.hpp
 *  \brief  Definition of the class SAES::InitializationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_SAES_InitializationOp_hpp
#define Beagle_SAES_InitializationOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"


namespace Beagle
{

namespace SAES
{

/*!
 *  \class InitializationOp Beagle/SAES/InitializationOp.hpp "Beagle/SAES/InitializationOp.hpp"
 *  \brief SA-ES genotype initialization operator class.
 *  \ingroup SAESF
 *
 *  Initialization is based on two parameters, the initial strategy value (saes.init.strategy) and
 *  the mean around it will be initialized (saes.init.mean). Each strategy parameters
 *  (\f$sigma_{init}\f$) are initialized to the associated initial strategy value, while the value of the SA-ES 
 *  vector is randomly initialized to a value following a Gaussian distribution (\f$N(\mu,\sigma_{init})\f$)
 *  around the mean, with the initial strategy value as standard deviation.
 *
 */
class InitializationOp : public EC::InitializationOp
{

public:

	//! SAES::InitializationOp allocator type.
	typedef AllocatorT<InitializationOp,EC::InitializationOp::Alloc>
	Alloc;
	//! SAES::InitializationOp handle type.
	typedef PointerT<InitializationOp,EC::InitializationOp::Handle>
	Handle;
	//! SAES::InitializationOp bag type.
	typedef ContainerT<InitializationOp,EC::InitializationOp::Bag>
	Bag;

	explicit InitializationOp(unsigned int inVectorSize=0,
	                          std::string inReproProbaName="ec.repro.prob",
	                          std::string inName="SAES-InitializationOp");
	virtual ~InitializationOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void initIndividual(Individual& outIndividual, Context& ioContext);

	/*!
	 *  \return Vector size to use for initialization.
	 */
	unsigned int getVectorSize() const
	{
		Beagle_StackTraceBeginM();
		return mVectorSize->getWrappedValue();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set vector size to use for initialization.
	 *  \param inVectorSize New vector size.
	 */
	void setVectorSize(unsigned int inVectorSize)
	{
		Beagle_StackTraceBeginM();
		mVectorSize->getWrappedValue() = inVectorSize;
		Beagle_StackTraceEndM();
	}

protected:

	DoubleArray::Handle  mMaxValue;           //!< Maximum of ES vector values.
	DoubleArray::Handle  mMinValue;           //!< Minimum of ES vector values.
	DoubleArray::Handle  mMeanInitValue;      //!< Mean value of Gaussian distribution at initialization.
	DoubleArray::Handle  mInitStrategyValue;  //!< Mutation strategy value at initialization.
	UInt::Handle         mVectorSize;         //!< Individual float vectors size.

};

}
}

#endif // Beagle_SAES_InitializationOp_hpp
