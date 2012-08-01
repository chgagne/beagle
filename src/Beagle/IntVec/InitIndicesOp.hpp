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
 *  \file   Beagle/IntVec/InitIndicesOp.hpp
 *  \brief  Definition of the class IntVec::InitIndicesOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_IntVec_InitIndicesOp_hpp
#define Beagle_IntVec_InitIndicesOp_hpp

#include "Beagle/IntVec.hpp"


namespace Beagle
{
namespace IntVec
{


/*!
 *  \class InitIndicesOp Beagle/IntVec/InitIndicesOp.hpp "Beagle/IntVec/InitIndicesOp.hpp"
 *  \brief Integer-valued genotype uniformly distributed initialization operator class.
 *  \ingroup IntVecF
 */
class InitIndicesOp : public EC::InitializationOp
{

public:

	//! IntVec::InitIndicesOp allocator type.
	typedef AllocatorT<InitIndicesOp,EC::InitializationOp::Alloc>
	Alloc;
	//! IntVec::InitIndicesOp handle type.
	typedef PointerT<InitIndicesOp,EC::InitializationOp::Handle>
	Handle;
	//! IntVec::InitIndicesOp bag type.
	typedef ContainerT<InitIndicesOp,EC::InitializationOp::Bag>
	Bag;

	explicit InitIndicesOp(unsigned int inIntVectorSize=0,
	                       std::string inReproProbaName="ec.repro.prob",
	                       std::string inName="IntVec-InitIndicesOp");
	virtual ~InitIndicesOp()
	{ }

	virtual void initIndividual(Individual& outIndividual, Context& ioContext);

	/*!
	 *  \return Integer vector size to use for initialization.
	 */
	unsigned int getIntVectorSize() const
	{
		Beagle_StackTraceBeginM();
		return mIntVectorSize->getWrappedValue();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set integer vector size to use for initialization.
	 *  \param inIntVectorSize New integer vector size.
	 */
	void setIntVectorSize(unsigned int inIntVectorSize)
	{
		Beagle_StackTraceBeginM();
		mIntVectorSize->getWrappedValue() = inIntVectorSize;
		Beagle_StackTraceEndM();
	}

	virtual void registerParams(System& ioSystem);

protected:

	UInt::Handle mIntVectorSize;  //!< Individual integer vectors size.

};

}
}

#endif // Beagle_IntVec_InitIndicesOp_hpp
