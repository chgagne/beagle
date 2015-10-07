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
 *  \file   beagle/GP/Module.hpp
 *  \brief  Definition of class GP::Module.
 *  \author Christian Gagne
 *  \author Matthew Walker
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:35 $
 */

/*!
 *  \defgroup EMA Evolutionary module acquisition
 *  \ingroup GPF
 *  \brief Mechanism to dynamically encapsulate and expand program modules, part of the GP framework.
 *
 *  \par Reference
 *  Peter J. Angeline and Jordan Pollack, "Evolutionary Module Acquisition",
 *  Second Annual Conference on Evolutionary Programming, La Jolla, CA, 1993.
 *
 */

#ifndef Beagle_GP_Module_hpp
#define Beagle_GP_Module_hpp

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
#include "beagle/GP/Tree.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class Module beagle/GP/Module.hpp "beagle/GP/Module.hpp"
 *  \brief GP generic module primitive.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup EMA
 */
class Module : public GP::Invoker
{

public:

	//! GP::Module allocator type.
	typedef AllocatorT<Module,GP::Invoker::Alloc>
	Alloc;
	//! GP::Module handle type.
	typedef PointerT<Module,GP::Invoker::Handle>
	Handle;
	//! GP::Module bag type.
	typedef ContainerT<Module,GP::Invoker::Bag>
	Bag;

	explicit Module(unsigned int inIndex=GP::Invoker::eGenerator,
	                unsigned int inNumberArgs=GP::Primitive::eAny,
	                std::string  inName="MODULE",
	                std::string  inArgsName="ARG");
	virtual ~Module()
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


#endif // Beagle_GP_Module_hpp
