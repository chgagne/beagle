/*
 *  Symbolic regression (symbreg):
 *  A simple GP example with Open BEAGLE
 *
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
 *  \file   SymbRegEvalOp.hpp
 *  \brief  Definition of the type SymbRegEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/09 21:43:12 $
 */

/*!
 *  \defgroup SymbReg Symbolic Regression Example
 *  \brief Symbolic regression (symbreg): A simple GP example with Open BEAGLE.
 *
 *  \par Objective
 *  Find a function of one independent variable and one dependent variable, in
 *  symbolic form, that fits a given sample of 20 \f$(x_i,y_i)\f$ data points,
 *  where the target function is the quadratic polynomial \f$x^4 + x^3 + x^2 + x\f$.
 *
 *  \par Terminal set
 *  - X (the independent variable)
 *  - PI
 *  - Ephemeral constants randomly generated in [-1,1]
 *
 *  \par Function set
 *  - +
 *  - -
 *  - *
 *  - /     (protected division)
 *  - SIN
 *  - COS
 *  - EXP
 *  - LOG   (protected logarithm)
 *
 *  \par Fitness cases
 *  The given sample of 20 data points \f$(x_i,y_i)\f$, randomly chosen within
 *  interval [-1,1].
 *
 *  \par Fitness
 *  \f$\frac{1.}{1.+RMSE}\f$ where RMSE is the Root Mean Square Error on the
 *  fitness cases.
 *
 *  \par Stopping criteria
 *  When the evolution reaches the maximum number of generations.
 *
 *  \par Reference
 *  John R. Koza, "Genetic Programming: On the Programming of Computers by Means
 *  of Natural Selection", MIT Press, 1992, pages 162-169.
 *
 */

#ifndef SymbRegEvalOp_hpp
#define SymbRegEvalOp_hpp

#include "Beagle/GP.hpp"
#include <string>
#include <vector>


/*!
 *  \class SymbRegEvalOp SymbRegEvalOp.hpp "SymbRegEvalOp.hpp"
 *  \brief The individual evaluation class operator for the problem of symbolic regression.
 *  \ingroup SymbReg
 */
class SymbRegEvalOp : public Beagle::GP::EvaluationOp
{

public:

	//! SymbRegEvalOp allocator type.
	typedef Beagle::AllocatorT< SymbRegEvalOp, Beagle::GP::EvaluationOp::Alloc > Alloc;
	//!< SymbRegEvalOp handle type.
	typedef Beagle::PointerT< SymbRegEvalOp, Beagle::GP::EvaluationOp::Handle > Handle;
	//!< SymbRegEvalOp bag type.
	typedef Beagle::ContainerT <SymbRegEvalOp, Beagle::GP::EvaluationOp::Bag > Bag;

	SymbRegEvalOp();

	virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
	        Beagle::GP::Context& ioContext);
	virtual void init(Beagle::System& ioSystem);

protected:

	Beagle::DataSetRegression::Handle mDataSet;  //!< Set of sampled data.

};

#endif // SymbRegEvalOp_hpp
