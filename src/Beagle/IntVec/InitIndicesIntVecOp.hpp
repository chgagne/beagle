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
 *  \file   beagle/GA/InitIndicesIntVecOp.hpp
 *  \brief  Definition of the class GA::InitIndicesIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_InitIndicesIntVecOp_hpp
#define Beagle_GA_InitIndicesIntVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/Int.hpp"
#include "beagle/UInt.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class InitIndicesIntVecOp beagle/GA/InitIndicesIntVecOp.hpp "beagle/GA/InitIndicesIntVecOp.hpp"
 *  \brief Integer-valued GA genotype uniformly distributed initialization operator class.
 *  \ingroup GAF
 *  \ingroup GAIV
 */
class InitIndicesIntVecOp : public InitializationOp
{

public:

	//! GA::InitIndicesIntVecOp allocator type.
	typedef AllocatorT<InitIndicesIntVecOp,InitializationOp::Alloc>
	Alloc;
	//! GA::InitIndicesIntVecOp handle type.
	typedef PointerT<InitIndicesIntVecOp,InitializationOp::Handle>
	Handle;
	//! GA::InitIndicesIntVecOp bag type.
	typedef ContainerT<InitIndicesIntVecOp,InitializationOp::Bag>
	Bag;

	explicit InitIndicesIntVecOp(unsigned int inIntVectorSize=0,
	                             std::string inReproProbaName="ec.repro.prob",
	                             std::string inName="GA-InitIndicesIntVecOp");
	virtual ~InitIndicesIntVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
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

protected:

	UInt::Handle      mIntVectorSize;  //!< Individual integer vectors size.

};

}
}

#endif // Beagle_GA_InitIndicesIntVecOp_hpp
