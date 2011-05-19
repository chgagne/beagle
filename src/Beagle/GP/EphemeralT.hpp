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
 *  \file   beagle/GP/EphemeralT.hpp
 *  \brief  Definition of the templated class GP::EphemeralT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.24 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_EphemeralT_hpp
#define Beagle_GP_EphemeralT_hpp

#include <string>

#include "PACC/XML.hpp"

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
 *  \class EphemeralT beagle/GP/EphemeralT.hpp "beagle/GP/EphemeralT.hpp"
 *  \brief Generic randomly generated ephemeral constant primitive.
 *  \param T Type of the constants.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup FunctPrimit
 */
template <class T>
class EphemeralT : public Primitive
{

public:

	//! GP::EphemeralT allocator type.
	typedef AbstractAllocT<EphemeralT<T>,Primitive::Alloc>
	Alloc;
	//! GP::EphemeralT handle type.
	typedef PointerT<EphemeralT<T>,Primitive::Handle>
	Handle;
	//! GP::EphemeralT bag type.
	typedef ContainerT<EphemeralT<T>,Primitive::Bag>
	Bag;

	explicit EphemeralT(typename T::Handle inValue=NULL, std::string inName="E");
	virtual ~EphemeralT()
	{ }

	/*!
	 *  \brief  Generate a new ephemeral constant.
	 *  \param  inName Name of the constant.
	 *  \param  ioContext Context to use to generate the value.
	 *  \return Handle to the ephemeral constant generated.
	 */
	virtual GP::Primitive::Handle generate(std::string inName, GP::Context& ioContext) =0;

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void                  getValue(Object& outValue);
	virtual bool                  haveValue() const;
	virtual GP::Primitive::Handle giveReference(unsigned int inNumberArguments, GP::Context& ioContext);
	virtual void                  execute(GP::Datum& outDatum, GP::Context& ioContext);
	virtual void                  readWithContext(PACC::XML::ConstIterator inIter, GP::Context& ioContext);
	virtual void                  setValue(const Object& inValue);
	virtual void                  writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:
	typename T::Handle mValue;     //!< Value of the ephemeral.

};

}
}


/*!
 *  \brief Construct a new ephemeral constant. If the value is NULL, act a an ephemeral generator.
 *  \param inValue Value of the ephemeral constant. When NULL, act a an ephemeral generator.
 *  \param inName Name of the ephemeral primitive.
 */
template <class T>
Beagle::GP::EphemeralT<T>::EphemeralT(typename T::Handle inValue, std::string inName) :
		Beagle::GP::Primitive(0, inName),
		mValue(inValue)
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data return by primitive EphemeralT.
 *  \param ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
template <class T>
const std::type_info* Beagle::GP::EphemeralT<T>::getReturnType(Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	return &typeid(T);
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Get the value of the ephemeral constant.
 *  \param outValue Object containing the value of the ephemeral constant.
 *  \throw InternalException If the constant value is missing.
 */
template <class T>
void Beagle::GP::EphemeralT<T>::getValue(Beagle::Object& outValue)
{
	Beagle_StackTraceBeginM();
	if(!mValue) throw Beagle_InternalExceptionM("Could not execute as the constant value is missing!");
	T& lValueT = castObjectT<T&>(outValue);
	lValueT = *mValue;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Give a referene to the ephemeral. If the value is NULL, generate a new ephemeral.
 *  \param inNumberArguments Number of arguments of the context used to generate the ephemeral.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the actual ephemeral, or a new ephemeral if the value is missing (NULL).
 */
template <class T>
Beagle::GP::Primitive::Handle
Beagle::GP::EphemeralT<T>::giveReference(unsigned int inNumberArguments, Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(!mValue) return generate(getName(), ioContext);
	return this;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Execute the ephemeral constant operation by copying the value.
 *  \param outDatum Result of the execution, the value of the constant.
 *  \param ioContext Evolutionary context.
 *  \throw InternalException If the constant value is missing.
 */
template <class T>
void Beagle::GP::EphemeralT<T>::execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(!mValue) throw Beagle_InternalExceptionM("Could not execute as the constant value is missing!");
	T& lResult = castObjectT<T&>(outDatum);
	lResult = *mValue;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Return whether ephemeral random constant have value.
 *  \return Always true.
 */
template <class T>
bool Beagle::GP::EphemeralT<T>::haveValue() const
{
	Beagle_StackTraceBeginM();
	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read ephemeral constant from XML node.
 *  \param inIter XML iterator to read the ephemeral from.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::IOException If the constant value is missing.
 */
template <class T>
void Beagle::GP::EphemeralT<T>::readWithContext(PACC::XML::ConstIterator inIter, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	if(inIter->getType() != PACC::XML::eData)
		throw Beagle_IOExceptionNodeM(*inIter, "tag expected!");

	if(inIter->getValue() != getName()) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}

	std::string lValue = inIter->getAttribute("value");
	if(lValue.empty()) mValue = NULL;
	else {
		if(mValue == NULL) {
			std::ostringstream lOSS;
			lOSS << "Could not read ephemeral value when the value member is NULL.";
			throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
		}
		std::istringstream lISS(lValue);
		PACC::XML::Document lParser;
		lParser.parse(lISS);
		mValue->read(lParser.getFirstRoot());
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set the value of the ephemeral constant.
 *  \param inValue Object containing the new value of the ephemeral constant.
 *  \throw InternalException If the constant value is missing.
 */
template <class T>
void Beagle::GP::EphemeralT<T>::setValue(const Beagle::Object& inValue)
{
	Beagle_StackTraceBeginM();
	if(!mValue) {
		std::ostringstream lOSS;
		lOSS << "Could not execute EphemeralT::setValue() as this instance's mValue is NULL.";
		lOSS << "Such a value indicates that this instance was intended to be used as a generator ";
		lOSS << "of emphemeral values rather than have a single set value.  Consider using this ";
		lOSS << "instance's giveReference() method.  giveReference() generates handles to new ";
		lOSS << "instances of the same type; the difference is that the new instance ";
		lOSS << "has a specific value (i.e. its mValue is set).";
		throw Beagle_InternalExceptionM(lOSS.str());
	}
	const T& lValueT = castObjectT<const T&>(inValue);
	*mValue = lValueT;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write the value of the ephemeral constant.
 *  \param ioStreamer XML streamer to write content of ephemeral constant.
 *  \param inIndent Whether output should be indented.
 */
template <class T>
void Beagle::GP::EphemeralT<T>::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	if(mValue != NULL) {
		std::ostringstream lOSS;
		PACC::XML::Streamer lStreamer2(lOSS);
		mValue->write(lStreamer2, false);
		ioStreamer.insertAttribute("value", lOSS.str());
	}
	Beagle_StackTraceEndM();
}


#endif // Beagle_GP_EphemeralT_hpp

