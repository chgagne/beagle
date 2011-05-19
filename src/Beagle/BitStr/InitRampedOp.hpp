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
 *  \file   Beagle/BitStr/InitRampedOp.hpp
 *  \brief  Definition of the class BitStr::InitRampedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.4 $
 *  $Date: 2007/08/08 19:26:34 $
 */


#ifndef Beagle_BitStr_InitRampedOp_hpp
#define Beagle_BitStr_InitRampedOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"
#include "Beagle/BitStr/BitString.hpp"


namespace Beagle
{
namespace BitStr
{

/*!
 *  \class InitRampedOp Beagle/BitStr/InitRampedOp.hpp "Beagle/BitStr/InitRampedOp.hpp"
 *  \brief Ramped bit string initialization operator class.
 *  \ingroup BitStrF
 *
 *  Ramped initialization consists in generating bit string individuals by first drawing the number
 *  of true bits in the individual uniformly and randomly, and then drawing the indices of the true
 *  bits randomly. Thus, on opposition with the traditionnal bit string initialization operator,
 *  the probabilities are uniform for all possible number of true bits (i.e. probabilities of
 *  generating bit string with one true bit == probability of bit string with n/2 true bits ==
 *  probability of bit string with n-1 true bits, etc.).
 */
class InitRampedOp : public EC::InitializationOp
{

public:

	//! BitStr::InitRampedOp allocator type.
	typedef AllocatorT<InitRampedOp,EC::InitializationOp::Alloc>
	Alloc;
	//! BitStr::InitRampedOp handle type.
	typedef PointerT<InitRampedOp,EC::InitializationOp::Handle>
	Handle;
	//! BitStr::InitRampedOp bag type.
	typedef ContainerT<InitRampedOp,EC::InitializationOp::Bag>
	Bag;

	explicit InitRampedOp(unsigned int inNumberBits=0,
	                      std::string inReproProbaName="ec.repro.prob",
	                      std::string inName="BitStr-InitRampedOp");
	virtual ~InitRampedOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void initIndividual(Individual& outIndividual, Context& ioContext);

	/*!
	 *  \return Number of bits in strings used for initialization.
	 */
	unsigned int getNumberBits() const
	{
		Beagle_StackTraceBeginM();
		return mNumberBits->getWrappedValue();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set number of bits in strings used for initialization.
	 *  \param inNumberBits Number of bits of individuals.
	 */
	void setNumberBits(unsigned int inNumberBits)
	{
		Beagle_StackTraceBeginM();
		mNumberBits->getWrappedValue() = inNumberBits;
		Beagle_StackTraceEndM();
	}

protected:

	UInt::Handle  mNumberBits;   //!< Number of bits in string.

};

}
}

#endif // Beagle_BitStr_InitRampedOp_hpp
