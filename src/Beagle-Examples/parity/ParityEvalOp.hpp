/*
 *  Even-6 parity (parity):
 *  A GP example using ADFs with Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
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
 *  \file   ParityEvalOp.hpp
 *  \brief  Definition of the type ParityEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
 *  $Date: 2007/08/08 19:26:48 $
 */

/*!
 *  \defgroup Parity Even-6 Parity Example
 *  \brief Even-6 parity (parity): A GP example using ADFs with Open BEAGLE.
 *
 *  \par Objective
 *  Find a program that produces the value of the Boolean parity problem given
 *  six independent Boolean inputs.
 *
 *  \par Terminal set
 *  - IN0, IN1, IN2, IN3, IN4, and IN5, the inputs of the function.
 *
 *  \par Function set
 *  - AND
 *  - OR
 *  - NAND
 *  - NOR
 *
 *  \par Automatically defined functions
 *  Two ADFs used, each with two arguments inputs.
 *
 *  \par Fitness cases
 *  All the \f$2^6 = 64\f$ combinations of the six input Booleans.
 *
 *  \par Fitness
 *  Rate of correct outputs obtained over all the 64 fitness cases.
 *
 *  \par Stopping criteria
 *  When the evolution reaches the maximum number of generations.
 *
 *  \par Reference
 *  John R. Koza, "Genetic Programming II: Automatic Discovery of Reusable
 *  Programs", MIT Press, 1994, pages 157-199.
 *
 */


#ifndef ParityEvalOp_hpp
#define ParityEvalOp_hpp

#include "Beagle/GP.hpp"
#include <string>
#include <vector>

#define ParitySizeM  16  //<! Number of rows in the truth-table
#define ParityFanInM 4   //<! Size of parity problem


/*!
 *  \class ParityEvalOp ParityEvalOp.hpp "ParityEvalOp.hpp"
 *  \brief The individual evaluation class operator for the even-6 parity problem.
 *  \ingroup Parity
 */
class ParityEvalOp : public Beagle::GP::EvaluationOp
{

public:

	//! ParityEvalOp allocator type.
	typedef Beagle::AllocatorT<ParityEvalOp,Beagle::GP::EvaluationOp::Alloc>
	Alloc;
	//!< ParityEvalOp handle type.
	typedef Beagle::PointerT<ParityEvalOp,Beagle::GP::EvaluationOp::Handle>
	Handle;
	//!< ParityEvalOp bag type.
	typedef Beagle::ContainerT<ParityEvalOp,Beagle::GP::EvaluationOp::Bag>
	Bag;

	explicit ParityEvalOp();

	virtual void init(Beagle::System& ioSystem);
	virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
	        Beagle::GP::Context& ioContext);

protected:
	std::vector< std::vector<Beagle::Bool> > mInputs;
	std::vector<Beagle::Bool>                mOutputs;

};

#endif // ParityEvalOp_hpp
