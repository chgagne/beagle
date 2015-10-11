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
 *  \file   beagle/GP/ADF.hpp
 *  \brief  Definition of the class GP::ADF.
 *  \author Christian Gagne
 *  \author Matthew Walker
 *  \author Marc Parizeau
 *  $Revision: 1.6 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_ADF_hpp
#define Beagle_GP_ADF_hpp

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
#include "beagle/GP/Invoker.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class ADF beagle/GP/ADF.hpp "beagle/GP/ADF.hpp"
 *  \brief GP automatically defined function (ADF) primitive
 *
 *  This class implements Automatically Defined Functions (ADFs) as
 *  described in Koza's second book on genetic programming.  ADFs are
 *  discussed in sections 2.2.2 and 3.4.7 of the %Beagle User Manual.
 *
 *  An example usage of ADFs can be found in \ref Parity.
 *
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup FunctPrimit
 */
class ADF : public GP::Invoker
{

public:

	//! GP::ADF allocator type.
	typedef AllocatorT<GP::ADF,GP::Invoker::Alloc>
	Alloc;
	//! GP::ADF handle type.
	typedef PointerT<GP::ADF,GP::Invoker::Handle>
	Handle;
	//! GP::AdfT bag type.
	typedef ContainerT<GP::ADF,GP::Invoker::Bag>
	Bag;

	explicit ADF(unsigned int inIndex=GP::Invoker::eGenerator,
	             unsigned int inNumberArgs=GP::Primitive::eAny,
	             std::string  inName="ADF",
	             std::string  inArgsName="ARG");
	virtual ~ADF()
	{ }

	virtual GP::Invoker::Handle generateInvoker(unsigned int inIndex,
	        std::string inName,
	        std::string inArgsName,
	        GP::Context& ioContext) const;
	virtual void                getCandidatesToInvoke(std::vector<unsigned int>& outCandidates,
	        unsigned int inNumberArguments,
	        GP::Context& ioContext) const;
	virtual GP::Tree::Handle    getInvokedTree(GP::Context& ioContext) const;
	virtual void                invoke(GP::Datum& outResult,
	                                   GP::Tree::Handle ioTree,
	                                   GP::Context& ioContext);
	virtual bool                validate(GP::Context& ioContext) const;

};

}
}


#endif // Beagle_GP_ADF_hpp
