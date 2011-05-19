/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/EvaluationOp.hpp
 *  \brief  Definition of the class LinGP::EvaluationOp.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/17 18:09:22 $
 */

#ifndef Beagle_LinGP_EvaluationOp_hpp
#define Beagle_LinGP_EvaluationOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/EvaluationOp.hpp"
#include "beagle/LinGP/Context.hpp"


namespace Beagle
{
namespace LinGP
{

/*!
 *  \class EvaluationOp beagle/LinGP/EvaluationOp.hpp "beagle/LinGP/EvaluationOp.hpp"
 *  \brief Abstract linear GP evaluation operator class.
 *  \ingroup LinGPF
 *  \ingroup LinGPOp
 */
class EvaluationOp : public Beagle::EvaluationOp
{

public:

	//! LinGP::EvaluationOp allocator type.
	typedef AbstractAllocT<EvaluationOp,Beagle::EvaluationOp::Alloc> Alloc;
	//! LinGP::EvaluationOp handle type.
	typedef PointerT<EvaluationOp,Beagle::EvaluationOp::Handle> Handle;
	//! LinGP::EvaluationOp bag type.
	typedef ContainerT<EvaluationOp,Beagle::EvaluationOp::Bag> Bag;

	explicit EvaluationOp(std::string inName="LinGP-EvaluationOp");
	virtual ~EvaluationOp()
	{ }

	/*!
	 *  \brief Evaluate the fitness of the given linear GP individual.
	 *  \param inIndividual Current individual to evaluate.
	 *  \param ioContext Evolutionary context.
	 *  \return Handle to the fitness value of the GP individual.
	 */
	virtual Fitness::Handle evaluate(Beagle::Individual& inIndividual, Beagle::Context& ioContext)
	{
		return evaluate(inIndividual, castObjectT<LinGP::Context&>(ioContext));
	}

	/*!
	 *  \brief Evaluate the fitness of the given linear GP individual.
	 *  \param inIndividual Current linear GP individual to evaluate.
	 *  \param ioContext Evolutionary context.
	 *  \return Handle to the fitness value of the linear GP individual.
	 */
	virtual Fitness::Handle evaluate(Beagle::Individual& inIndividual, LinGP::Context& ioContext) =0;

};

}
}

#endif // Beagle_LinGP_EvaluationOp_hpp
