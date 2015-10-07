/*
 *  Function maximization (maxfct):
 *  A simple GA example
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
 *  \file   MaxFctEvalOp.hpp
 *  \brief  Definition of the type MaxFctEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/10 20:32:23 $
 */

/*!
 *  \defgroup MaxFct Function Maximization Example
 *  \brief Function maximization (maxfct): A simple GA example with Open BEAGLE.
 *
 *  \par Objective
 *  Find the maximum of the following 5D function:
 *  \f$f(x) = \frac{161.8}{(u_N^2 \sum_{k=0}^{N-1}(x_k^2 + u_k^2))}\f$
 *  with \f$x = <x_0, x_1, ..., x_{N-1}>\f$, \f$u_{k+1} = x_k + u_k\f$, \f$x_k\f$
 *  in \f$[-200,200]\f$ for all \f$k\f$, \f$N = 5\f$ and \f$u_0 = 10\f$.
 *
 *  \par Representation
 *  Bit strings of 125 bits, constructed from the function's five arguments \f$x_i\f$,
 *  each encoded with 25 bits on the interval \f$[-200,200]\f$.
 *
 *  \par Fitness
 *  Value of \f$f(x_1,x_2,x_3,x_4,x_5)\f$.
 *
 */

#ifndef MaxFctEvalOp_hpp
#define MaxFctEvalOp_hpp

#include "Beagle/EC.hpp"
#include "Beagle/BitStr.hpp"

#include <vector>


/*!
 *  \class MaxFctEvalOp MaxFctEvalOp.hpp "MaxFctEvalOp.hpp"
 *  \brief The individual evaluation class operator for the problem of function maximisation.
 *  \ingroup MaxFct
 */
class MaxFctEvalOp : public Beagle::EvaluationOp
{

public:

	//! MaxFctEvalOp allocator type.
	typedef Beagle::AllocatorT<MaxFctEvalOp,Beagle::EvaluationOp::Alloc>
	Alloc;
	//!< MaxFctEvalOp handle type.
	typedef Beagle::PointerT<MaxFctEvalOp,Beagle::EvaluationOp::Handle>
	Handle;
	//!< MaxFctEvalOp bag type.
	typedef Beagle::ContainerT<MaxFctEvalOp,Beagle::EvaluationOp::Bag>
	Bag;

	MaxFctEvalOp();

	virtual Beagle::Fitness::Handle evaluate(Beagle::Individual& inIndividual,
	        Beagle::Context& ioContext);

protected:

	Beagle::BitStr::BitString::DecodingKeyVector mDecodingKeys;   //!< Decoding keys.

};

#endif // MaxFctEvalOp_hpp
