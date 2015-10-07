/*
 *  OneMax (onemax):
 *  Simple GA example with Open BEAGLE
 *
 *  Copyright (C) 2003-2006 by Christian Gagne and Marc Parizeau
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
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
 *  \file   OneMaxEvalOp.hpp
 *  \brief  Definition of the type OneMaxEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:48 $
 */

/*!
 *  \defgroup OneMax OneMax Example
 *  \brief OneMax (onemax): Simple GA example with Open BEAGLE.
 *
 *  \par Objective
 *  Find a bit string filled with ones.
 *
 *  \par Representation
 *  Bit strings made of N bits.
 *
 *  \par Fitness
 *  Number of ones in the bit string, where the perfect solution has a fitness equal to N, the size of
 *  the bit strings.
 *
 */

#ifndef OneMaxEvalOp_hpp
#define OneMaxEvalOp_hpp

#include "Beagle/EC.hpp"
#include <vector>


/*!
 *  \class OneMaxEvalOp OneMaxEvalOp.hpp "OneMaxEvalOp.hpp"
 *  \brief The individual evaluation class operator for the OneMax problem.
 *  \ingroup OneMax
 */
class OneMaxEvalOp : public Beagle::EvaluationOp
{

public:

	//! OneMaxEvalOp allocator type.
	typedef Beagle::AllocatorT<OneMaxEvalOp,Beagle::EvaluationOp::Alloc>
	Alloc;
	//!< OneMaxEvalOp handle type.
	typedef Beagle::PointerT<OneMaxEvalOp,Beagle::EvaluationOp::Handle>
	Handle;
	//!< OneMaxEvalOp bag type.
	typedef Beagle::ContainerT<OneMaxEvalOp,Beagle::EvaluationOp::Bag>
	Bag;

	explicit OneMaxEvalOp();

	virtual Beagle::Fitness::Handle evaluate(Beagle::Individual& inIndividual,
	        Beagle::Context& ioContext);

};

#endif // OneMaxEvalOp_hpp
