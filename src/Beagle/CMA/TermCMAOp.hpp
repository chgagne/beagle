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
 *  \file   beagle/GA/TermCMAOp.hpp
 *  \brief  Definition of the class GA::TermCMAOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_TermCMAOp_hpp
#define Beagle_TermCMAOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/TerminationOp.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class TermCMAOp beagle/GA/TermCMAOp.hpp "beagle/GA/TermCMAOp.hpp"
 *  \brief CMA-ES termination operator class.
 *  \ingroup GAF
 *  \ingroup GAFV
 */
class TermCMAOp : public Beagle::TerminationOp
{

public:

	//! TermCMAOp allocator type
	typedef AllocatorT<GA::TermCMAOp,Beagle::TerminationOp::Alloc>
	Alloc;
	//! TermCMAOp handle type.
	typedef PointerT<GA::TermCMAOp,Beagle::TerminationOp::Handle>
	Handle;
	//! TermCMAOp bag type.
	typedef ContainerT<GA::TermCMAOp,Beagle::TerminationOp::Bag>
	Bag;

	explicit TermCMAOp(std::string inName="GA-TermCMAOp");
	virtual ~TermCMAOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool terminate(const Deme& inDeme, Context& ioContext);

protected:

	Double::Handle mSigma;       //!< Initial CMA-ES sigma value.
	Double::Handle mTolX;        //!< Tolerance on variations of values.
	Double::Handle mTolCond;     //!< Tolerance on condition number of matrix C.

};

}
}

#endif // Beagle_TerminationCheckOp_hpp
