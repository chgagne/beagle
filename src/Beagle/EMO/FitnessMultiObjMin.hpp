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
 *  \file   beagle/FitnessMultiObjMin.hpp
 *  \brief  Definition of the class FitnessMultiObjMin.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:12 $
 */

#ifndef Beagle_FitnessMultiObjMin_hpp
#define Beagle_FitnessMultiObjMin_hpp

#include <vector>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Stats.hpp"
#include "beagle/FitnessMultiObj.hpp"

namespace Beagle
{

/*!
 *  \class FitnessMultiObjMin beagle/FitnessMultiObjMin.hpp "beagle/FitnessMultiObjMin.hpp"
 *  \brief Minimization multiobjective fitness measure class.
 *  \ingroup ECF
 *  \ingroup FitStats
 *  \ingroup MultiObj
 */
class FitnessMultiObjMin : public FitnessMultiObj
{

public:

	//! FitnessMultiObjMin allocator type.
	typedef AllocatorT<FitnessMultiObjMin,FitnessMultiObj::Alloc> Alloc;
	//! FitnessMultiObjMin handle type.
	typedef PointerT<FitnessMultiObjMin,FitnessMultiObj::Handle> Handle;
	//! FitnessMultiObjMin bag type.
	typedef ContainerT<FitnessMultiObj,FitnessMultiObj::Bag> Bag;

	explicit FitnessMultiObjMin(unsigned int inSize=0, double inValue=0.f);
	explicit FitnessMultiObjMin(const std::vector<double>& inFitness);
	virtual ~FitnessMultiObjMin()
	{ }

	virtual void                copy(const Member& inOriginal, System& ioSystem);
	virtual const std::string&  getType() const;
	virtual bool                isDominated(const Fitness& inRightFitness) const;
	virtual bool                isLess(const Object& inRightObj) const;

};

}

#endif // Beagle_FitnessMultiObjMin_hpp

