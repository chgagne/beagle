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
 *  \file   beagle/GP/Argument.hpp
 *  \brief  Definition of class GP::Argument.
 *  \author Christian Gagne
 *  \author Matthew Walker
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_GP_Argument_hpp
#define Beagle_GP_Argument_hpp

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
 *  \class Argument beagle/GP/Argument.hpp "beagle/GP/Argument.hpp"
 *  \brief GP generic arguments for invokable primitive.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup FunctPrimit
 */
class Argument : public Primitive
{

public:

	//! Specify the constants for specific kind of arguments.
	enum ArgumentKind {eGenerator=UINT_MAX};

	//!< Modes available for ADF arguments computation.
	enum ArgumentsEvaluationMode
	{
	    eCaching=0,        //!< Compute arguments on-the-fly and cache results.
	    eJustInTime=1,     //!< Compute arguments on-the-fly but don't cache results.
	    ePreCompute=2      //!< Pre-compute values of all arguments.
	};

	//! GP::Argument allocator type.
	typedef AllocatorT<Argument,Primitive::Alloc>
	Alloc;
	//! GP::Argument handle type.
	typedef PointerT<Argument,Primitive::Handle>
	Handle;
	//! GP::Argument bag type.
	typedef ContainerT<Argument,Primitive::Bag>
	Bag;

	explicit Argument(Object::Alloc::Handle inValueAlloc,
	                  unsigned int inEvalMode=eCaching,
	                  unsigned int inIndex=eGenerator,
	                  std::string inName="ARG");
	virtual ~Argument()
	{ }

	virtual Handle generateArgument(unsigned int inIndex) const;

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual double getSelectionWeight(unsigned int inNumberArguments,
	                                  GP::Context& ioContext) const;
	virtual GP::Primitive::Handle giveReference(unsigned int inNumberArguments,
	        GP::Context& ioContext);
	virtual void execute(GP::Datum& outResult, GP::Context& ioContext);
	virtual void forceEvaluation(GP::Datum& outResult, GP::Context& ioContext);
	virtual bool isSelectionWeightStable(unsigned int inNumberArguments) const;
	virtual void readWithContext(PACC::XML::ConstIterator inIter, GP::Context& ioContext);
	virtual void pushExecutionContext(unsigned int inNumberArguments, GP::Context& ioContext);
	virtual void popExecutionContext();
	virtual bool validate(GP::Context& ioContext) const;
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \return Evaluation mode of the argument.
	 */
	inline unsigned int getEvalMode() const
	{
		Beagle_StackTraceBeginM();
		return mSharedData->mEvalMode;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Index of the argument primtive.
	 */
	inline unsigned int getIndex() const
	{
		Beagle_StackTraceBeginM();
		return mIndex;
		Beagle_StackTraceEndM();
	}

protected:

	/*!
	 *  \brief Class defining an internal data structure of the arguments.
	 */
class SharedData : public Beagle::Object
	{
	public:
		typedef AllocatorT<SharedData,Beagle::Object::Alloc>
		Alloc;
		typedef PointerT<SharedData,Beagle::Object::Handle>
		Handle;
		typedef ContainerT<SharedData,Beagle::Object::Bag>
		Bag;

		explicit SharedData(Object::Alloc::Handle inCacheAlloc,
		                    unsigned int inEvalMode=GP::Argument::eJustInTime) :
				mTypeAllocator(inCacheAlloc),
				mEvalMode(inEvalMode)
		{
			clear();
		}

		virtual ~SharedData()
		{ }

		void clear()
		{
			mEvalContext.clear();
			mCaches.clear();
		}

		Object::Alloc::Handle mTypeAllocator; //!< Allocator of type of data processed.
		GP::Context::Bag      mEvalContext;   //!< Stack of evolutionary context for args evaluation.
		Container::Bag        mCaches;        //!< Stack of argument values.
		unsigned int          mEvalMode;      //!< Evaluation mode of the arguments.
	};

	SharedData::Handle mSharedData;    //!< Shared data between the arguments.
	unsigned int       mIndex;         //!< Argument index in GP tree.

	friend class AllocatorT<Argument,Primitive::Alloc>;

	Argument()
	{ }            //!< Default constructor disabled.
	explicit Argument(SharedData::Handle inSharedData, unsigned int inIndex, std::string inName);

};

}
}

#endif // Beagle_GP_Argument_hpp
