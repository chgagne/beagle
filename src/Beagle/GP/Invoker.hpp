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
 *  \file   beagle/GP/Invoker.hpp
 *  \brief  Definition of the templated class GP::Invoker.
 *  \author Christian Gagne
 *  \author Matthew Walker
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_Invoker_hpp
#define Beagle_GP_Invoker_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveInline.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Datum.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


namespace Beagle
{
namespace GP
{

/*!
 *  \class Invoker beagle/GP/Invoker.hpp "beagle/GP/Invoker.hpp"
 *  \brief Generic GP tree invoker primitive.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup FunctPrimit
 */
class Invoker : public Primitive
{

public:

	//! Specify the constants for specific kind of Invoker.
	enum InvokerKind {eGenerator=UINT_MAX};

	//! GP::Invoker allocator type.
	typedef AbstractAllocT<Invoker,Primitive::Alloc>
	Alloc;
	//! GP::Invoker handle type.
	typedef PointerT<Invoker,Primitive::Handle>
	Handle;
	//! GP::Invoker bag type.
	typedef ContainerT<Invoker,Primitive::Bag>
	Bag;

	explicit Invoker(unsigned int inIndex=eGenerator,
	                 unsigned int inNumberArgs=Beagle::GP::Primitive::eAny,
	                 std::string  inName="INVK",
	                 std::string  inArgsName="ARG");
	virtual ~Invoker()
	{ }

	/*!
	 *  \brief Generate a new invoker primitive from the given specifications.
	 *  \param inIndex Tree index for which the primitive is created.
	 *  \param inName Name of the primitive generated.
	 *  \param ioContext Evolutionary context.
	 *  \param inArgsName Name of the arguments associated to the invoker created.
	 */
	virtual Handle generateInvoker(unsigned int inIndex,
	                               std::string inName,
	                               std::string inArgsName,
	                               GP::Context& ioContext) const =0;

	/*!
	 *  \brief Return indices of the trees that can be invoked by the invoker.
	 *  \param outCandidates Indices of tree that can be selected as invokable in the actual context.
	 *  \param inNumberArguments Number of arguments for which the selection is desired.
	 *  \param ioContext Evolutionary context.
	 */
	virtual void getCandidatesToInvoke(std::vector<unsigned int>& outCandidates,
	                                   unsigned int inNumberArguments,
	                                   GP::Context& ioContext) const =0;

	/*!
	 *  \brief Get reference the tree to invoke.
	 *  \param ioContext Evolutionary context.
	 *  \return Handle to the invoked tree.
	 */
	virtual GP::Tree::Handle getInvokedTree(GP::Context& ioContext) const =0;

	/*!
	 *  \brief Invoke GP tree to execute.
	 *  \param outResult Result of GP tree invocation
	 *  \param ioTree Tree to invoke.
	 *  \param ioContext Evolutionary context.
	 */
	virtual void invoke(GP::Datum& outResult, GP::Tree::Handle ioTree, GP::Context& ioContext) =0;

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual double getSelectionWeight(unsigned int inNumberArguments, GP::Context& ioContext) const;
	virtual GP::Primitive::Handle giveReference(unsigned int inNumberArguments, GP::Context& ioContext);
	virtual void   execute(GP::Datum& outResult, GP::Context& ioContext);
	virtual bool   isSelectionWeightStable(unsigned int inNumberArguments) const;
	virtual void   readWithContext(PACC::XML::ConstIterator inIter, GP::Context& ioContext);
	virtual bool   validate(GP::Context& ioContext) const;
	virtual void   writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \return Name of the argument primitives associated to the invoker.
	 */
	inline std::string getArgsName() const
	{
		Beagle_StackTraceBeginM();
		return mArgsName;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Index of the tree invoked.
	 */
	inline unsigned int getIndex() const
	{
		Beagle_StackTraceBeginM();
		return mIndex;
		Beagle_StackTraceEndM();
	}

protected:

	std::string  mArgsName;  //!< Invoker arguments primitive name.
	unsigned int mIndex;     //!< Index of the associated tree to invoker.

};

}
}


#endif // Beagle_GP_Invoker_hpp
