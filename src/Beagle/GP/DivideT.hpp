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
 *  \file   beagle/GP/DivideT.hpp
 *  \brief  Definition of the templated class GP::DivideT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2008/05/18 19:16:49 $
 */

#ifndef Beagle_GP_DivideT_hpp
#define Beagle_GP_DivideT_hpp

#include <cmath>
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
 *  \class DivideT beagle/GP/DivideT.hpp "beagle/GP/DivideT.hpp"
 *  \brief GP generic 2 arguments protected division primitive class.
 *  \param T Type of the data added. operator/ must be properly defined for this type.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup MathPrimit
 */
template <class T>
class DivideT : public Primitive
{

public:

	//! GP::DivideT allocator type.
	typedef AllocatorT<DivideT<T>,Primitive::Alloc>
	Alloc;
	//! GP::DivideT handle type.
	typedef PointerT<DivideT<T>,Primitive::Handle>
	Handle;
	//! GP::DivideT bag type.
	typedef ContainerT<DivideT<T>,Primitive::Bag>
	Bag;

	explicit DivideT(std::string inName="DIV");
	virtual ~DivideT()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual std::string deparse(std::vector<std::string>& lSubTrees) const;
	virtual void        execute(GP::Datum& outResult, GP::Context& ioContext);

};


/*!
 *  \brief Divide primitive for the double type. Min divide value is 0.001.
 */
typedef DivideT<Double>
Divide;

}
}


/*!
 *  \brief Construct a new division primitive.
 *  \param inName Name of the division primitive.
 */
template <class T>
Beagle::GP::DivideT<T>::DivideT(std::string inName) :
		Beagle::GP::Primitive(2, inName)
{ }


/*!
 *  \brief Write this primitive as a C function
 */
template <class T>
std::string Beagle::GP::DivideT<T>::deparse(std::vector<std::string>& lSubTrees) const
{
	Beagle_AssertM(getNumberArguments()==2);
	Beagle_AssertM(lSubTrees.size() == 2);
	std::ostringstream lOSS;
	lOSS << "(" << lSubTrees[0] << "/" << lSubTrees[1] << ")";
	return lOSS.str();
}


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive DivideT.
 *  \param  inN Index of the argument to get the type tag.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type needed.
 *  \throw  AssertException If the index inN given is greater than 1.
 */
template <class T>
const std::type_info* Beagle::GP::DivideT<T>::getArgType(unsigned int inN,
        Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inN<2);
	return &typeid(T);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Return the tag of the type of data return by primitive DivideT.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
template <class T>
const std::type_info* Beagle::GP::DivideT<T>::getReturnType(Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	return &typeid(T);
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Execute the characteristic operation of the division primitive.
 *  \param outResult Result of the division.
 *  \param ioContext Evolutionary context.
 *  \warning operator/= and operator< must be properly defined for the type T, the type of data added.
 *    Type of data must be compatible with real value comparisions.
 *    The DivideT primitive give as result 1 when the denominator is below 0.001.
 */
template <class T>
void Beagle::GP::DivideT<T>::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	T& lResult = castObjectT<T&>(outResult);
	T lArg2;
	get1stArgument(lResult, ioContext);
	get2ndArgument(lArg2, ioContext);
	if((lArg2 < T(0.001)) && (T(-0.001) < lArg2)) lResult = T(1.0);
	else lResult /= lArg2;
	Beagle_StackTraceEndM();
}


#endif // Beagle_GP_DivideT_hpp
