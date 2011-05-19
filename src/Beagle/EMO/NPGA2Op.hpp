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
 *  \file   beagle/NPGA2Op.hpp
 *  \brief  Definition of the class NPGA2Op.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:13 $
 */


#ifndef Beagle_NPGA2Op_hpp
#define Beagle_NPGA2Op_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Individual.hpp"
#include "beagle/FitnessMultiObj.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Float.hpp"

namespace Beagle
{

/*!
 *  \class NPGA2Op beagle/NPGA2Op.hpp "beagle/NPGA2Op.hpp"
 *  \brief NPGA2 multiobjective selection operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *  \ingroup MultiObj
 *
 *  The NPGA2 operator implement the multiobjective evolutionary algorithm NPGA 2
 *  (Niched Pareto Genetic Algorithm) of Erickson, Mayer, and Horn.
 *  \par Reference
 *  Mark Erickson, Alex Mayer, and Jeffrey Horn, "The Niched Pareto Genetic Algorithm 2
 *  Applied to the Design of Groundwater Remediation Systems", First International
 *  Conference on Evolutionary Multi-Criterion Optimization (EMO 2001), March 2001,
 *  pages 681-695.
 *
 */
class NPGA2Op : public Operator
{

public:

	//! NPGA2Op allocator type.
	typedef AllocatorT<NPGA2Op,Operator::Alloc>
	Alloc;
	//! NPGA2Op handle type.
	typedef PointerT<NPGA2Op,Operator::Handle>
	Handle;
	//! NPGA2Op bag type.
	typedef ContainerT<NPGA2Op,Operator::Bag>
	Bag;

	NPGA2Op(std::string inName="NPGA2Op");
	virtual ~NPGA2Op()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void operate(Deme& ioDeme, Context& ioContext);
	virtual void init(System& ioSystem);

protected:

	float evalNicheCount(const Individual& inEvalIndividual,
	                     const Individual::Bag& inIndividualPool) const;

	UInt::Handle  mNumberParticipants;  //!< Number of participants to each tournament.
	Float::Handle mNicheRadius;         //!< Minimal distance used for niche counting.

};

}

#endif // Beagle_NPGA2Op_hpp


