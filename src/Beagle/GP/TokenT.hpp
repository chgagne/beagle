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
 *  \file   beagle/GP/TokenT.hpp
 *  \brief  Definition of the templated class GP::TokenT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2008/05/18 19:16:49 $
 */

/*!
 *  \defgroup FunctPrimit Functional GP Primitives
 *  \ingroup Primit
 *  \brief Various GP primitives for different purpose (terminals, ADF, randomly generated
 *    ephemerals), part of the GP primitives module.
 */

#ifndef Beagle_GP_TokenT_hpp
#define Beagle_GP_TokenT_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
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
 *  \class TokenT beagle/GP/TokenT.hpp "beagle/GP/TokenT.hpp"
 *  \brief GP simple token terminal primitive class.
 *  \param T Type of the token contained.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup FunctPrimit
 */
template <class T>
class TokenT : public Primitive
{

public:

	//! GP::TokenT allocator type.
	typedef AllocatorT<TokenT<T>,Primitive::Alloc>
	Alloc;
	//! GP::TokenT handle type.
	typedef PointerT<TokenT<T>,Primitive::Handle>
	Handle;
	//! GP::TokenT bag type.
	typedef ContainerT<TokenT<T>,Primitive::Bag>
	Bag;

	explicit TokenT(std::string inName="TOKEN");
	explicit TokenT(std::string inName, const T& inToken);
	virtual ~TokenT()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual std::string deparse(std::vector<std::string>& lSubTrees) const;
	virtual void        getValue(Object& outValue);
	virtual void        execute(GP::Datum& outResult, GP::Context& ioContext);
	virtual bool        haveValue() const;
	virtual void        setValue(const Object& inValue);

	/*!
	 *  \brief Get the value of the token (const version).
	 *  \return The token value.
	 */
	inline const T& getToken() const
	{
		Beagle_StackTraceBeginM();
		return mToken;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get the value of the token.
	 *  \return The token value.
	 */
	inline T& getToken()
	{
		Beagle_StackTraceBeginM();
		return mToken;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the value of the token.
	 *  \param inToken Value of the token.
	 */
	inline void setToken(const T& inToken)
	{
		Beagle_StackTraceBeginM();
		mToken = inToken;
		Beagle_StackTraceEndM();
	}

protected:

	T mToken;  //!< Value of the token.

};

}
}


/*!
 *  \brief Construct a new token terminal primitive.
 *  \param inName Name of the token terminal primitive.
 */
template <class T>
Beagle::GP::TokenT<T>::TokenT(std::string inName) :
		Beagle::GP::Primitive(0, inName)
{ }


/*!
 *  \brief Construct a new token terminal primitive.
 *  \param inName Name of the token terminal primitive.
 *  \param inToken Initial value of the token terminal.
 */
template <class T>
Beagle::GP::TokenT<T>::TokenT(std::string inName, const T& inToken) :
		Beagle::GP::Primitive(0, inName),
		mToken(inToken)
{ }


/*!
 *  \brief Write this primitive as a C function
 */
template <class T>
std::string Beagle::GP::TokenT<T>::deparse(std::vector<std::string>& lSubTrees) const
{
	return mToken.serialize();
}


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data return by primitive TokenT.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
template <class T>
const std::type_info* Beagle::GP::TokenT<T>::getReturnType(Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	return &typeid(T);
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Get the primitive value.
 *  \param outValue Object in which the actual primitive value is written.
 */
template <class T>
void Beagle::GP::TokenT<T>::getValue(Beagle::Object& outValue)
{
	Beagle_StackTraceBeginM();
	T& lValueT = castObjectT<T&>(outValue);
	lValueT = mToken;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Execute the characteristic operation of the token terminal.
 *  \param outResult Result of the operation (copy of the token).
 *  \param ioContext Evolutionary context.
 */
template <class T>
void Beagle::GP::TokenT<T>::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	T& lResult = castObjectT<T&>(outResult);
	lResult = mToken;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Return whether token primitive have value.
 *  \return Always true.
 */
template <class T>
bool Beagle::GP::TokenT<T>::haveValue() const
{
	Beagle_StackTraceBeginM();
	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Set the value of the variable, in this case the value of the token.
 *  \param inValue Value of the variable.
 */
template <class T>
void Beagle::GP::TokenT<T>::setValue(const Beagle::Object& inValue)
{
	Beagle_StackTraceBeginM();
	const T& lValueT = castObjectT<const T&>(inValue);
	mToken = lValueT;
	Beagle_StackTraceEndM();
}


#endif // Beagle_GP_TokenT_hpp
