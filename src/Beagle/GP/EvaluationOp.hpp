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
 *  \file   beagle/GP/EvaluationOp.hpp
 *  \brief  Definition of the class GP::EvaluationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_EvaluationOp_hpp
#define Beagle_GP_EvaluationOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/EvaluationOp.hpp"
#include "beagle/GP/Individual.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Datum.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class EvaluationOp beagle/GP/EvaluationOp.hpp "beagle/GP/EvaluationOp.hpp"
 *  \brief Abstract GP evaluation operator class.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class EvaluationOp : public Beagle::EvaluationOp
{

public:

	//! GP::EvaluationOp allocator type.
	typedef AbstractAllocT<EvaluationOp,Beagle::EvaluationOp::Alloc>
	Alloc;
	//! GP::EvaluationOp handle type.
	typedef PointerT<EvaluationOp,Beagle::EvaluationOp::Handle>
	Handle;
	//! GP::EvaluationOp bag type.
	typedef ContainerT<EvaluationOp,Beagle::EvaluationOp::Bag>
	Bag;

	explicit EvaluationOp(std::string inName="GP-EvaluationOp");
	virtual ~EvaluationOp()
	{ }

	virtual Fitness::Handle evaluate(Beagle::Individual& inIndividual, Beagle::Context& ioContext);
	void setValue(std::string inName, const Object& inValue, GP::Context& ioContext) const;

	/*!
	 *  \brief Evaluate the fitness of the given GP individual.
	 *  \param inIndividual Current GP individual to evaluate.
	 *  \param ioContext Evolutionary context.
	 *  \return Handle to the fitness value of the GP individual.
	 */
	virtual Fitness::Handle evaluate(GP::Individual& inIndividual, GP::Context& ioContext) =0;

};

}
}

#endif // Beagle_GP_EvaluationOp_hpp
