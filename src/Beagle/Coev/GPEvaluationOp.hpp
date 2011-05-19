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
 *  \file   beagle/Coev/GPEvaluationOp.hpp
 *  \brief  Declaration of file Beagle::Coev::GPEvaluation.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/08 19:26:32 $
 */

#ifndef Beagle_Coev_GPEvaluation_hpp
#define Beagle_Coev_GPEvaluation_hpp

#include <string>

#include "PACC/Threading.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/EvaluationOp.hpp"
#include "beagle/Coev/EvaluationOp.hpp"
#include "beagle/GP.hpp"


namespace Beagle
{
namespace Coev
{

/*!
 *  \class GPEvaluationOp beagle/Coev/GPEvaluationOp.hpp "beagle/Coev/GPEvaluationOp.hpp"
 *  \brief Abstract co-evolution GP evaluation operator class.
 *  \ingroup COEVF
 */
class GPEvaluationOp : public Beagle::Coev::EvaluationOp
{

public:

	//! Coev::GPEvaluationOp allocator type.
	typedef AbstractAllocT<GPEvaluationOp,Beagle::Coev::EvaluationOp::Alloc>
	Alloc;
	//! Coev::GPEvaluationOp handle type.
	typedef PointerT<GPEvaluationOp,Beagle::Coev::EvaluationOp::Handle>
	Handle;
	//! Coev::GPEvaluationOp bag type.
	typedef ContainerT<GPEvaluationOp,Beagle::Coev::EvaluationOp::Bag>
	Bag;

	explicit GPEvaluationOp(unsigned int inTrigger=1, std::string inName="Coev-GPEvaluationOp");
	virtual ~GPEvaluationOp()
	{ }

	/*!
	 *  \brief Evaluate fitness of a bunch of individual in an evaluation sets.
	 *  \param ioSets Sets to evaluate fitness.
	 *
	 *  You cannot make assumption about the order of the evaluation sets are in
	 *  the ioSets structure. These can change from evaluation to evaluation. Use
	 *  struct EvalSet member inID to identify the evaluation sets you put into
	 *  shared storage by call to add set. This ID is for convenience and is not used
	 *  internally to identify sets.
	 *
	 *  Assign fitness value to individuals using method assignFitness, otherwise
	 *  statistics value will be erroneous.
	 */
	virtual void evaluateSets(EvalSetVector& ioSets) =0;

	/*!
	 *  \brief Make evaluation sets from given deme.
	 *  \param ioIndivBag Bag of individuals to evaluate.
	 *  \param ioContext Evolutionary context.
	 *
	 *  This method is call one time at each generation, for each evolving
	 *  thread/population. This method is problem-specific and consists
	 *  to make evaluation sets that are mated with other thread/population
	 *  evaluation sets for co-evolutionary fitness evaluation. Sets are added
	 *  into a shared structure with a call to method addSet.
	 */
	virtual void makeSets(Beagle::Individual::Bag& ioIndivBag,
	                      Beagle::Context::Handle ioContext) =0;

	void setValue(std::string inName, const Object& inValue, GP::Context& ioContext) const;

};

}
}


#endif // Beagle_Coev_GPEvaluation_hpp

