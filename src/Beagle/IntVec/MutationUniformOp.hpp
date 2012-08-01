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
 *  \file   Beagle/IntVec/MutationUniformOp.hpp
 *  \brief  Definition of the class IntVec::MutationUniformOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_IntVec_MutationUniformOp_hpp
#define Beagle_IntVec_MutationUniformOp_hpp

#include "Beagle/IntVec.hpp"


namespace Beagle
{
namespace IntVec
{

/*!
 *  \class MutationUniformOp Beagle/IntVec/MutationUniformOp.hpp "Beagle/IntVec/MutationUniformOp.hpp"
 *  \brief Integer vector uniform mutation operator class.
 *  \ingroup IntVecF
 */
class MutationUniformOp : public EC::MutationOp
{

public:

	//! IntVec::MutationUniformOp allocator type.
	typedef AllocatorT<MutationUniformOp,EC::MutationOp::Alloc> Alloc;
	//! IntVec::MutationUniformOp handle type.
	typedef PointerT<MutationUniformOp,EC::MutationOp::Handle> Handle;
	//! IntVec::MutationUniformOp bag type.
	typedef ContainerT<MutationUniformOp,EC::MutationOp::Bag> Bag;

	explicit MutationUniformOp(std::string inMutationPbName="intvec.mutunif.indpb",
		std::string inIntMutatePbName="intvec.mutunif.intpb",
	    std::string inName="IntVec-MutationUniformOp");
	virtual ~MutationUniformOp()
	{ }

	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void registerParams(System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	IntArray::Handle mMaxValue;         //!< Integers maximum value.
	IntArray::Handle mMinValue;         //!< Integers minimum value.
	Double::Handle   mIntMutateProba;   //!< Single integer mutation probability.
	std::string      mIntMutatePbName;  //!< Single integer mutation probability parameter name.

};

}
}

#endif // Beagle_IntVec_MutationUniformOp_hpp
