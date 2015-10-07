/*
 *  Boolean 11-multiplexer (multiplexer)
 *  Copyright (C) 2006 by Christian Gagne
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
 *  \file   MultiplexerEvalOp.hpp
 *  \brief  Definition of the type MultiplexerEvalOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.3 $
 *  $Date: 2007/08/08 19:26:48 $
 */

/*!
 *  \defgroup Multiplexer Boolean 11-Multiplexer Example
 *  \brief Boolean 11-Multiplexer (multiplexer).
 *
 *  \par Objective
 *  Find a program that produces the value of the Boolean 11-multiplexer, that
 *  is to return the value of the input line between the 8 input channels
 *  D0-D7 according to the address given by inputs A0-A2.
 *
 *  \par Terminal set
 *  - D0, D1, D2, D3, D4, D5, D6, D7: the input channels.
 *  - A0, A1, A2: the input addresses.
 *
 *  \par Function set
 *  - AND
 *  - OR
 *  - NOT
 *  - IF      (3-arguments Boolean if-then-else)
 *
 *  \par Fitness cases
 *  All the \f$2^8 = 2048$\f test cases possible are used for fitness evaluation.
 *
 *  \par Fitness
 *  Rate of correct outputs obtained over all the 2048 fitness cases.
 *
 *  \par Stopping criteria
 *  When the evolution reaches the maximum number of generations.
 *
 *  \par Reference
 *  John R. Koza, "Genetic Programming I: On the Programming of Computers by Means
 *  of Natural Selection", MIT Press, 1992, pages 170-187.
 *
 */


#ifndef MultiplexerEvalOp_hpp
#define MultiplexerEvalOp_hpp

#include "Beagle/GP.hpp"
#include <string>
#include <vector>


/*!
 *  \class MultiplexerEvalOp MultiplexerEvalOp.hpp "MultiplexerEvalOp.hpp"
 *  \brief The individual evaluation class operator for the Boolean 11-multiplexer problem.
 *  \ingroup Multiplexer
 */
class MultiplexerEvalOp : public Beagle::GP::EvaluationOp
{

public:

	//! MultiplexerEvalOp allocator type.
	typedef Beagle::AllocatorT<MultiplexerEvalOp,Beagle::GP::EvaluationOp::Alloc>
	Alloc;
	//!< MultiplexerEvalOp handle type.
	typedef Beagle::PointerT<MultiplexerEvalOp,Beagle::GP::EvaluationOp::Handle>
	Handle;
	//!< MultiplexerEvalOp bag type.
	typedef Beagle::ContainerT<MultiplexerEvalOp,Beagle::GP::EvaluationOp::Bag>
	Bag;

	explicit MultiplexerEvalOp();

	virtual void init(Beagle::System& ioSystem);
	virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
	        Beagle::GP::Context& ioContext);

protected:

	std::vector< std::vector<Beagle::Bool> > mInputs;
	std::vector<Beagle::Bool>                mOutputs;

};

#endif // MultiplexerEvalOp_hpp
