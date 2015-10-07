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
 *  \file   beagle/GP/IfThenElseT.hpp
 *  \brief  Definition of the templated class GP::IfThenElseT.
 *  \author Matthew Walker 
 *  \author Christian Gagne
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:35 $
 */


#ifndef Beagle_GP_IfThenElseT_hpp
#define Beagle_GP_IfThenElseT_hpp

#include "beagle/GP/Primitive.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


namespace Beagle
{
namespace GP
{

/*!
 *  \class IfThenElseT beagle/GP/IfThenElseT.hpp "beagle/GP/IfThenElseT.hpp"
 *  \brief GP templated if-then-else class.
 *  \param ProcessedType Type of data processed by the GP primitive.
 *  \param ConditionType Type used to evaluate conditional statement.
 *  \ingroup GPF
 *  \ingroup Primit
 * 
 *  The if-then-else template is defined as a four arguments GP primitive. A condition is 
 *  evaluated from the two first arguments, the third argument being returned if the previous 
 *  condition is true, otherwize the fourth argument being returned. The condition is evaluated
 *  by the ConditionType type, which is a functor that follow the STL's adaptable binary 
 *  predicate interface (see http://www.sgi.com/tech/stl/AdaptableBinaryPredicate.html). 
 *  This mean that the method bool operator()(ProcessedType,ProcessedType) must be correctly defined
 *  in order to evaluate the condition on two ProcessedType objects. When unspecified, the functor
 *  used is the STL std::less adaptable binary predicate, which test whether the first argument of
 *  the primitive is less than the second (using operator<).
 *
 */
template <class ProcessedType, class ConditionType=std::less<ProcessedType> >
class IfThenElseT : public GP::Primitive
{

public:

	typedef AllocatorT<IfThenElseT<ProcessedType,ConditionType>,GP::Primitive::Alloc>
	Alloc;
	typedef PointerT<IfThenElseT<ProcessedType,ConditionType>,GP::Primitive::Handle>
	Handle;
	typedef ContainerT<IfThenElseT<ProcessedType,ConditionType>,GP::Primitive::Bag>
	Bag;

	explicit IfThenElseT(std::string inName="IFTE");
	virtual ~IfThenElseT()
	{}

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void execute(GP::Datum& outResult, GP::Context& ioContext);

};

} // namespace GP
} // namespace Beagle


/*!
 *  \brief Construct if-then-else primitive.
 *  \param inName If-then-else primitive name.
 */
template <class ProcessedType,class ConditionType>
Beagle::GP::IfThenElseT<ProcessedType,ConditionType>::IfThenElseT(std::string inName) :
		Beagle::GP::Primitive(4, inName)
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive IfThenElseT.
 *  \param  inN Index of the argument to get the type tag.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type needed.
 *  \throw  AssertException If the index inN given is greater than 3.
 */
template <class ProcessedType,class ConditionType>
const std::type_info*
Beagle::GP::IfThenElseT<ProcessedType,ConditionType>::getArgType(unsigned int inN,
        Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inN<4);
	return &typeid(ProcessedType);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Return the tag of the type of data return by primitive IfThenElseT.
 *  \param  inDesiredType Desired return type.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
template <class ProcessedType,class ConditionType>
const std::type_info*
Beagle::GP::IfThenElseT<ProcessedType,ConditionType>::getReturnType(Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	return &typeid(ProcessedType);
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Execute if-then-else primitive.
 *  \param outResult Result of primitive execution.
 *  \param ioContext Evolutionary context.
 *
 *  Return third argument if the condition evaluated by a ConditionType instance on the two first
 *  arguments of the primitive is true, otherwize return value of the fourth primitive argument.
 */
template <class ProcessedType,class ConditionType>
void Beagle::GP::IfThenElseT<ProcessedType,ConditionType>::execute(Beagle::GP::Datum& outResult,
        Beagle::GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	ProcessedType& lResult = castObjectT<ProcessedType&>(outResult);
	ProcessedType lArg2;
	ConditionType lCondition;
	get1stArgument(lResult, ioContext);
	get2ndArgument(lArg2, ioContext);
	if(lCondition(lResult,lArg2)) getArgument(2, lResult, ioContext);
	else getArgument(3, lResult, ioContext);
	Beagle_StackTraceEndM();
}


#endif // Beagle_GP_IfThenElseT_hpp
