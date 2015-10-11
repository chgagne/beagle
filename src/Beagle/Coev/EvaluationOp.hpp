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
 *  \file   beagle/Coev/EvaluationOp.hpp
 *  \brief  Declaration of file Beagle::Coev::EvaluationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.22 $
 *  $Date: 2007/08/08 19:26:32 $
 */

#ifndef Beagle_Coev_EvaluationOp_hpp
#define Beagle_Coev_EvaluationOp_hpp

#include <string>

#include "PACC/Threading.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/EvaluationOp.hpp"


namespace Beagle
{
namespace Coev
{

/*!
 *  \class EvaluationOp beagle/Coev/EvaluationOp.hpp "beagle/Coev/EvaluationOp.hpp"
 *  \brief Abstract co-evolution evaluation operator class.
 *  \ingroup COEVF
 *
 *  For its application co-evolution fitness evaluation operators, the user must over-defined the pure 
 *  virtual methods makeSets and evaluateSets. The makeSets method is called to ask the evaluation
 *  operator to make evaluation sets from the actual deme. An evaluation set corresponds to a
 *  group of individuals than can be mated with groups of individual from other population for
 *  co-evolutionary fitness evaluation. The groups of individuals created in method makeSets are
 *  added to the shared structure by a call to the method addSets. This method takes a second
 *  optionnal argument which indicated whether or not the call is blocking. If the call is 
 *  blocking, the call to addSets will return only when the set will be evaluated. Otherwise, the
 *  evaluation set return immediatly after the set is added  to the shared structure. Once the 
 *  number of evaluation sets added to the shared structure is equal to the trigger value, the
 *  problem-specific co-evolutionary fitness evaluation is done by a call to the pure virtual method 
 *  evaluateSets.
 *
 *  Fitness value evaluated in method evaluateSets must be assigned to the individuals using
 *  the method assignFitness. If this method is not used, the statistics values will
 *  not be accurate.
 *
 *  The member mID of the struct EvalSet is provided for convenience only and is not used internally.
 *  This can be used to identity the different evaluation sets add into the common structure. You 
 *  cannot make any assumptions on the order the evaluation sets are put in the common structure, 
 *  and so the order they are given to method evaluateSets.
 *
 *  The trigger value is specified when constructing the different co-evolutionary evaluation operator.
 *  Each co-evolutionary evaluation must specify the trigger value, and the value given must be the 
 *  same for the different thread/population evaluation operators.
 *
 */
class EvaluationOp : public Beagle::EvaluationOp
{

public:

	//! Coev::EvaluationOp allocator type.
	typedef AbstractAllocT<EvaluationOp,Beagle::EvaluationOp::Alloc>
	Alloc;
	//! Coev::EvaluationOp handle type.
	typedef PointerT<EvaluationOp,Beagle::EvaluationOp::Handle>
	Handle;
	//! Coev::EvaluationOp bag type.
	typedef ContainerT<EvaluationOp,Beagle::EvaluationOp::Bag>
	Bag;

	/*!
	 *  \struct EvalSet beagle/Coev/EvaluationOp.hpp "beagle/Coev/EvaluationOp.hpp"
	 *  \brief Evaluation set association used in co-evolution evaluation operator.
	 */
	struct EvalSet
	{
		Beagle::Individual::Bag mIndividuals;    //!< Individuals of the evaluation set
		Beagle::Context::Handle mContext;        //!< Context associated to the evaluation set
		unsigned int            mID;             //!< Evaluation set ID

		EvalSet();
		EvalSet(Beagle::Individual::Bag& inIndividuals,
		        Beagle::Context::Handle inContext,
		        unsigned int inID=0);
	};

	//! Vector of evaluation set.
	typedef std::vector<EvalSet>  EvalSetVector;

	explicit EvaluationOp(unsigned int inTrigger=1, std::string inName="Coev-EvaluationOp");
	virtual ~EvaluationOp()
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

	virtual void addSet(EvalSet& inEvalSet, bool inBlocking=true);
	virtual void assignFitness(Beagle::Fitness::Handle inFitness,
	                           Beagle::Individual& ioIndividual,
	                           Beagle::Context& ioContext) const;
	virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
	                                 BreederNode::Handle inChild,
	                                 Context& ioContext);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

protected:

	static PACC::Threading::Condition smCondition;      //!< Condition of co-evaluation
	static EvalSetVector              smEvalSets;       //!< Shared storage of evaluation sets
	static unsigned int               smTrigger;        //!< Number of sets needed to start an evaluation

private:

	virtual Fitness::Handle evaluate(Individual& inIndividual, Context& ioContext);

};

}
}


#endif // Beagle_Coev_EvaluationOp_hpp

