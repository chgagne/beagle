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
 *  \file   beagle/GA/AlgoESOneFifthRule.hpp
 *  \brief  Definition of the class GA::AlgoESOneFifthRule.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/08 14:51:33 $
 */

#ifndef Beagle_GA_AlgoESOneFifthRule_hpp
#define Beagle_GA_AlgoESOneFifthRule_hpp

#include <map>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Algorithm.hpp"


namespace Beagle
{

namespace GA
{

/*!
 *  \class GA::AlgoESOneFifthRule beagle/GA/AlgoESOneFifthRule.hpp "beagle/GA/AlgoESOneFifthRule.hpp"
 *  \brief (1+1)-ES with one fifth rule algorithm.
 *  \ingroup GAF
 *  \ingroup GAFV
 */
class AlgoESOneFifthRule : public Algorithm
{

public:

	//! AlgoESOneFifthRule allocator type.
	typedef AllocatorT< AlgoESOneFifthRule, Algorithm::Alloc > Alloc;
	//! AlgoESOneFifthRule handle type.
	typedef PointerT< AlgoESOneFifthRule, Algorithm::Handle > Handle;
	//! AlgoESOneFifthRule bag type.
	typedef ContainerT< AlgoESOneFifthRule, Algorithm::Bag > Bag;

	AlgoESOneFifthRule(void);

	virtual void configure(Evolver& ioEvolver, System& ioSystem);

};

}
}

#endif // Beagle_GA_AlgoESOneFifthRule_hpp
