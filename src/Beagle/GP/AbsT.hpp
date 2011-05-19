/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  \file   beagle/GP/AbsT.hpp
 *  \brief  Definition of the templated class GP::AbsT.
 *  \author Matthew Walker
 *  $Revision: 1.7 $
 *  $Date: 2008/05/18 19:16:49 $
 */

#ifndef Beagle_GP_AbsT_hpp
#define Beagle_GP_AbsT_hpp

#include <cmath>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Bool.hpp"
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
 *  \class AbsT beagle/GP/AbsT.hpp "beagle/GP/AbsT.hpp"
 *  \brief GP templated absolute class
 *  \ingroup GPF
 *  \ingroup Primit
 */
template <class T>
class AbsT : public GP::Primitive
{

public:

	//! GP::AbsT allocator type.
	typedef AllocatorT<AbsT<T>,GP::Primitive::Alloc>
	Alloc;
	//! GP::AbsT handle type.
	typedef PointerT<AbsT<T>,GP::Primitive::Handle>
	Handle;
	//! GP::AbsT bag type.
	typedef ContainerT<AbsT<T>,GP::Primitive::Bag>
	Bag;

	explicit AbsT(std::string inName="ABS");
	virtual ~AbsT()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual std::string deparse(std::vector<std::string>& lSubTrees) const;
	virtual void        execute(GP::Datum& outResult, GP::Context& ioContext);

};

} // end of namespace GP
} // end of namespace Beagle


/*!
 *  \brief Construct a new ABS primitive.
 *  \param inName Name of the primitive.
 */
template <class T>
Beagle::GP::AbsT<T>::AbsT(std::string inName) :
		Beagle::GP::Primitive(1, inName)
{ }


/*!
 *  \brief Write this primitive as a C function
 */
template <class T>
std::string Beagle::GP::AbsT<T>::deparse(std::vector<std::string>& lSubTrees) const
{
  throw Beagle_UndefinedMethodInternalExceptionM("deparse","GP::AbsT<T>",getName());
}


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive AbsT.
 *  \param  inN Index of the argument to get the type tag.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type needed.
 *  \throw  AssertException If the index inN given is greater than 0.
 */
template <class T>
const std::type_info* Beagle::GP::AbsT<T>::getArgType(unsigned int inN,
        Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inN<1);
	return &typeid(T);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Return the tag of the type of data return by primitive AbsT.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
template <class T>
const std::type_info* Beagle::GP::AbsT<T>::getReturnType(Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	return &typeid(T);
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Find the absolute value of the argument
 *  \param outResult Result of the absolute method.
 *  \param ioContext Evolutionary context.
 */
template <class T>
void Beagle::GP::AbsT<T>::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	T& lResult = castObjectT<T&>(outResult);
	get1stArgument(lResult, ioContext);
	lResult = Beagle::absolute(lResult);
	Beagle_StackTraceEndM();
}


#endif // Beagle_GP_AbsT_hpp
