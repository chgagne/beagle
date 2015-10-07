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
 *  \file   beagle/GP/ModuleExpandOp.hpp
 *  \brief  Definition of ModuleExpandOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.9 $
 *  $Date: 2007/08/09 21:43:10 $
 */

#ifndef Beagle_GP_ModuleExpandOp_hpp
#define Beagle_GP_ModuleExpandOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/System.hpp"
#include "beagle/Context.hpp"
#include "beagle/Float.hpp"
#include "beagle/String.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Tree.hpp"
#include "beagle/GP/Individual.hpp"

namespace Beagle
{
namespace GP
{


/*!
 *  \brief Expand operator for evolutionary module acquisition, expand an existing module in a tree.
 *  \ingroup GPF
 *  \ingroup EMA
 */
class ModuleExpandOp : public Operator
{

public:

	//! ModuleExpandOp allocator type.
	typedef AllocatorT<ModuleExpandOp,Operator::Alloc>
	Alloc;
	//! ModuleExpandOp handle type.
	typedef PointerT<ModuleExpandOp,Operator::Handle>
	Handle;
	//! ModuleExpandOp bag type.
	typedef ContainerT<ModuleExpandOp,Operator::Bag>
	Bag;

	explicit ModuleExpandOp(std::string inName="GP-ModuleExpandOp");
	virtual ~ModuleExpandOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void expand(unsigned int inNodeToExpand, GP::Tree& ioTree, GP::Context& ioContext);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

protected:

	String::Handle mModulePrimitName;  //!< Name of modules primitives.
	Float::Handle  mExpandProba;       //!< Probability of expanding a given module in tree.

};

}
}

#endif // Beagle_GP_ModuleExpandOp_hpp
