/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagn�and Marc Parizeau
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
 *  \file   beagle/GP/SubtractT.hpp
 *  \brief  Definition of the templated class GP::SubtractT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2008/05/18 19:16:49 $
 */

#ifndef Beagle_GP_SubtractT_hpp
#define Beagle_GP_SubtractT_hpp

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
 *  \class SubtractT beagle/GP/SubtractT.hpp "beagle/GP/SubtractT.hpp"
 *  \brief GP generic 2 arguments subtraction primitive class.
 *  \param T Type of the data subtratced. operator- must be properly defined for this type.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup MathPrimit
 */
template <class T>
class SubtractT : public Primitive
{

public:

	//! GP::SubtractT allocator type.
	typedef AllocatorT<SubtractT<T>,Primitive::Alloc>
	Alloc;
	//! GP::SubtractT handle type.
	typedef PointerT<SubtractT<T>,Primitive::Handle>
	Handle;
	//! GP::SubtractT bag type.
	typedef ContainerT<SubtractT<T>,Primitive::Bag>
	Bag;

	explicit SubtractT(std::string inName="SUB");
	virtual ~SubtractT()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual std::string deparse(std::vector<std::string>& lSubTrees) const;
	virtual void        execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext);

};


/*!
 *  \brief Subtract primitive for the double type.
 */
typedef SubtractT<Double>
Subtract;

}
}


/*!
 *  \brief Construct a new subtraction primitive.
 *  \param inName Name of the subtraction primitive.
 */
template <class T>
Beagle::GP::SubtractT<T>::SubtractT(std::string inName) :
		Beagle::GP::Primitive(2, inName)
{ }


/*!
 *  \brief Write this primitive as a C function
 */
template <class T>
std::string Beagle::GP::SubtractT<T>::deparse(std::vector<std::string>& lSubTrees) const
{
	Beagle_AssertM(getNumberArguments()==2);
	Beagle_AssertM(lSubTrees.size() == 2);
	std::ostringstream lOSS;
	lOSS << "(" << lSubTrees[0] << "-" << lSubTrees[1] << ")";
	return lOSS.str();
}


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive SubtractT.
 *  \param  inN Index of the argument to get the type tag.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type needed.
 *  \throw  AssertException If the index inN given is greater than 1.
 */
template <class T>
const std::type_info* Beagle::GP::SubtractT<T>::getArgType(unsigned int inN,
        Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inN<2);
	return &typeid(T);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Return the tag of the type of data return by primitive SubtractT.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
template <class T>
const std::type_info* Beagle::GP::SubtractT<T>::getReturnType(Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	return &typeid(T);
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Execute the characteristic operation of the subtraction primitive.
 *  \param outResult Result of the subtraction.
 *  \param ioContext Evolutionary context.
 *  \warning operator-= must be properly defined for the type T, the type of data added.
 */
template <class T>
void Beagle::GP::SubtractT<T>::execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	T& lResult = castObjectT<T&>(outResult);
	T lArg2;
	get1stArgument(lResult, ioContext);
	get2ndArgument(lArg2, ioContext);
	lResult -= lArg2;
	Beagle_StackTraceEndM();
}


#endif // Beagle_GP_SubtractT_hpp
