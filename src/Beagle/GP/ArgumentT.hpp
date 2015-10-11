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
 *  \file   beagle/GP/ArgumentT.hpp
 *  \brief  Implementation of template GP::ArgumentT.
 *  \author Christian Gagne
 *  \author Matthew Walker
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2008/05/18 19:16:49 $
 */

#ifndef Beagle_GP_ArgumentT_hpp
#define Beagle_GP_ArgumentT_hpp

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
#include "beagle/GP/ArgumentT.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class ArgumentT beagle/GP/ArgumentT.hpp "beagle/GP/ArgumentT.hpp"
 *  \brief GP generic arguments for invokable primitive.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup FunctPrimit
 */
template <class T>
class ArgumentT : public GP::Argument
{

public:

	//! GP::Argument allocator type.
	typedef AllocatorT<ArgumentT<T>,GP::Argument::Alloc>
	Alloc;
	//! GP::Argument handle type.
	typedef PointerT<ArgumentT<T>,GP::Argument::Handle>
	Handle;
	//! GP::Argument bag type.
	typedef ContainerT<ArgumentT<T>,GP::Argument::Bag>
	Bag;

	explicit ArgumentT(unsigned int inEvalMode=eCaching,
	                   unsigned int inIndex=eGenerator,
	                   std::string inName="ARG");
	virtual ~ArgumentT()
	{ }

	virtual std::string          deparse(std::vector<std::string>& lSubTrees) const;
	virtual GP::Argument::Handle generateArgument(unsigned int inIndex) const;

protected:

	explicit ArgumentT(Beagle::GP::Argument::SharedData::Handle inSharedData,
	                   unsigned int inIndex, std::string inName);

};

}
}


/*!
 *  \brief Construct an argument primitive.
 *  \param inEvalMode Evaluation mode of the arguments 
 *    (see \ref Beagle::GP::Argument::ArgumentsEvaluationMode).
 *  \param inIndex Argument index.
 *  \param inName Name of the argument primitive.
 */
template <class T>
Beagle::GP::ArgumentT<T>::ArgumentT(unsigned int inEvalMode,
                                    unsigned int inIndex,
                                    std::string inName) :
		Beagle::GP::Argument(new typename T::Alloc, inEvalMode, inIndex, inName)
{ }


/*!
 *  \brief Construct an argument primitive.
 *  \param inSharedData Shared structure between argument of the same type.
 *  \param inIndex Argument index.
 *  \param inName Name of the argument primitive.
 */
template <class T>
Beagle::GP::ArgumentT<T>::ArgumentT(Beagle::GP::Argument::SharedData::Handle inSharedData,
                                    unsigned int inIndex,
                                    std::string inName) :
		Beagle::GP::Argument(inSharedData, inIndex, inName)
{ }


/*!
 *  \brief Write this primitive as a C function
 */
template <class T>
std::string Beagle::GP::ArgumentT<T>::deparse(std::vector<std::string>& lSubTrees) const
{
  throw Beagle_UndefinedMethodInternalExceptionM("deparse","GP::ArgumentT<T>",getName());
}


/*!
 *  \brief Generate a new argument of the same type than the actual one.
 *  \param inIndex Index the generated argument is refering to.
 *  \return Handle to the generated argument primitive.
 */
template <class T>
Beagle::GP::Argument::Handle Beagle::GP::ArgumentT<T>::generateArgument(unsigned int inIndex) const
{
	Beagle_StackTraceBeginM();
	return new GP::ArgumentT<T>(mSharedData, inIndex, getName());
	Beagle_StackTraceEndM();
}


#endif // Beagle_GP_ArgumentT_hpp
