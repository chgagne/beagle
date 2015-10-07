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

#ifndef Beagle_GP_InitializationOp_hpp
#define Beagle_GP_InitializationOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/BreederOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/String.hpp"
#include "beagle/Individual.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class InitializationOp beagle/InitializationOp.hpp "beagle/InitializationOp.hpp"
 *  \brief Abstract initialization operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 *  \ingroup GPPop
 */
class InitializationOp : public Beagle::InitializationOp
{

public:

	//! InitializationOp allocator type.
	typedef AbstractAllocT<InitializationOp,InitializationOp::Alloc>
	Alloc;
	//! InitializationOp handle type.
	typedef PointerT<InitializationOp,InitializationOp::Handle>
	Handle;
	//! InitializationOp  bag type.
	typedef ContainerT<InitializationOp,InitializationOp::Bag>
	Bag;

	explicit InitializationOp(std::string inReproProbaName="ec.repro.prob",
	                          std::string inName="GP-InitializationOp");
	virtual ~InitializationOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void initIndividual(Beagle::Individual& outIndividual, Beagle::Context& ioContext);

	/*!
	 *  \brief Initialize a tree.
	 *  \param outTree Tree to initialize.
	 *  \param inMinDepth Minimum depth to make tree.
	 *  \param inMaxDepth Maximum depth to make tree.
	 *  \param ioContext Evolution context.
	 *  \return The size of the initialized tree.
	 */
	virtual unsigned int initTree(GP::Tree& outTree,
	                              unsigned int inMinDepth,
	                              unsigned int inMaxDepth,
	                              GP::Context &ioContext) const =0;

protected:

	UInt::Handle      mMaxTreeDepth;     //!< Maximum depth of the initialized trees.
	UInt::Handle      mMinTreeDepth;     //!< Minimum depth of the initialized trees.
	UInt::Handle      mMaxNumberTrees;   //!< Maximum number of trees in generated individuals.
	UInt::Handle      mMinNumberTrees;   //!< Maximum number of trees in generated individuals.
	UIntArray::Handle mMaxTreeArgs;      //!< Maximum number of arguments in trees.
	UIntArray::Handle mMinTreeArgs;      //!< Maximum number of arguments in trees.

};

}
}

#endif // Beagle_GP_InitializationOp_hpp
