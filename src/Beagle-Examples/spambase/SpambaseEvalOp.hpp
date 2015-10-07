/*
 *  SPAM e-mail database (spambase):
 *  Machine learning using strongly-typed GP with Open BEAGLE
 *  Copyright (C) 2001-2006 by Christian Gagne and Marc Parizeau
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
 *  \file   SpambaseEvalOp.hpp
 *  \brief  Definition of the type SpambaseEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/10 20:32:28 $
 */

/*!
 *  \defgroup Spambase Spambase Example
 *  \brief SPAM e-mail database (spambase): Machine learning using strongly-typed GP.
 *
 *  \par Objective
 *  Find a program the will successfully predict whether a given e-mail is spam
 *  or not from some extracted features.
 *
 *  \par Comments
 *  The evolved programs works on floating-point values AND Booleans values.
 *  The programs must return a Boolean value which must be true if e-mail is
 *  spam, and false otherwise.
 *
 *  Don't expect too much from this program as it is quite basic and not oriented
 *  toward performance. It is given to illustrate the use of strongly-typed GP
 *  with Open BEAGLE. From a machine learning perspective, it is mainly wrong.
 *
 *  \par Terminal set
 *  - IN0, IN1, ...  up to IN56, the e-mail features.      [Double]
 *  - 0 and 1, two Boolean constants.                      [Bool]
 *  - Ephemeral constants randomly generated in $[0,100]$  [Double]
 *
 *  \par Function set
 *  - AND               [Inputs: Bool,   Output: Bool]
 *  - OR                [Input:  Bool,   Output: Bool]
 *  - NOT               [Inputs: Bool,   Output: Bool]
 *  - +                 [Inputs: Double, Output: Double]
 *  - -                 [Inputs: Double, Output: Double]
 *  - *                 [Inputs: Double, Output: Double]
 *  - /                 [Inputs: Double, Output: Double]
 *  - <                 [Inputs: Double, Output: Bool]
 *  - ==                [Inputs: Double, Output: Bool]
 *  - if-then-else      [1st Input: Bool, 2nd & 3rd Input: Double, Output: Double]
 *
 *  \par Fitness cases
 *  A random sample of 400 e-mails from the database, re-chosen for
 *  each fitness evaluation.
 *
 *  \par Fitness
 *  Classification rate over the 400 e-mails sampled.
 *
 *  \par Stopping criteria
 *  When an individual get a 100% classification rate (fitness of 1.) or when
 *  the evolution reaches the maximum number of generations.
 *
 *  \par Reference
 *  Machine learning repository, http://www.ics.uci.edu/~mlearn/MLRepository.html
 *
 */

#ifndef SpambaseEvalOp_hpp
#define SpambaseEvalOp_hpp

#include "Beagle/GP.hpp"
#include <string>
#include <vector>

#define Spambase_TestSize 400


/*!
 *  \class SpambaseEvalOp SpambaseEvalOp.hpp "SpambaseEvalOp.hpp"
 *  \brief The individual evaluation class operator for the spambase problem.
 *  \ingroup Spambase
 */
class SpambaseEvalOp : public Beagle::GP::EvaluationOp
{

public:

	//! SpambaseEvalOp allocator type.
	typedef Beagle::AllocatorT<SpambaseEvalOp,Beagle::GP::EvaluationOp::Alloc> Alloc;
	//!< SpambaseEvalOp handle type.
	typedef Beagle::PointerT<SpambaseEvalOp,Beagle::GP::EvaluationOp::Handle> Handle;
	//!< SpambaseEvalOp bag type.
	typedef Beagle::ContainerT<SpambaseEvalOp,Beagle::GP::EvaluationOp::Bag> Bag;

	SpambaseEvalOp();

	virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
	        Beagle::GP::Context& ioContext);

};

#endif // SpambaseEvalOp_hpp
