/*
 *  Function maximization with real-valued GA (MaxFctFloat):
 *  A simple real-valued GA example with Open BEAGLE
 *
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
 *  \file   MaxFctFloatEvalOp.hpp
 *  \brief  Definition of the type MaxFctFloatEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:48 $
 */

/*!
 *  \defgroup MaxFctFloat Function Maximization with Real-Valued GA Example
 *  \brief Function maximization with real-valued GA (MaxFctFloat):
 *    A simple real-valued GA example with Open BEAGLE.
 *
 *  \par Objective
 *  Find the maximum of the following 5D function:
 *  \f$f(x) = \frac{161.8}{(u_N^2 \sum_{k=0}^{N-1}(x_k^2 + u_k^2))}\f$
 *  with \f$x = <x_0, x_1, ..., x_{N-1}>\f$, \f$u_{k+1} = x_k + u_k\f$, \f$x_k\f$
 *  in \f$[-200,200]\f$ for all \f$k\f$, \f$N = 5\f$ and \f$u_0 = 10\f$.
 *
 *  \par Representation
 *  Vector of five floating-point numbers representing the function's five arguments
 *  \f$x_i\f$, limited in the interval \f$[-200,200]\f$.
 *
 *  \par Fitness
 *  Value of \f$f(x_1,x_2,x_3,x_4,x_5)\f$.
 *
 */

#ifndef MaxFctFloatEvalOp_hpp
#define MaxFctFloatEvalOp_hpp

#include "Beagle/EC.hpp"
#include <vector>


/*!
 *  \class MaxFctFloatEvalOp MaxFctFloatEvalOp.hpp "MaxFctFloatEvalOp.hpp"
 *  \brief The individual evaluation class operator for the problem of real-valued GA
 *    function maximisation.
 *  \ingroup MaxFctFloat
 */
class MaxFctFloatEvalOp : public Beagle::EvaluationOp
{

public:

	//! MaxFctFloatEvalOp allocator type.
	typedef Beagle::AllocatorT<MaxFctFloatEvalOp,Beagle::EvaluationOp::Alloc>
	Alloc;
	//!< MaxFctFloatEvalOp handle type.
	typedef Beagle::PointerT<MaxFctFloatEvalOp,Beagle::EvaluationOp::Handle>
	Handle;
	//!< MaxFctFloatEvalOp bag type.
	typedef Beagle::ContainerT<MaxFctFloatEvalOp,Beagle::EvaluationOp::Bag>
	Bag;

	explicit MaxFctFloatEvalOp();

	virtual Beagle::Fitness::Handle evaluate(Beagle::Individual& inIndividual,
	        Beagle::Context& ioContext);

};

#endif // MaxFctFloatEvalOp_hpp
