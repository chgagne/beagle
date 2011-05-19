/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/EC/EvaluationMultipleOp.hpp
 *  \brief  Definition of the class EvaluationMultipleOp.
 *  \author Matthew Walker
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 18:09:12 $
 */

#ifndef Beagle_EC_EvaluationMultipleOp_hpp
#define Beagle_EC_EvaluationMultipleOp_hpp

#include <string>
#include <iostream>

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class EvaluationMultipleOp Beagle/EC/EvaluationMultipleOp.hpp "Beagle/EC/EvaluationMultipleOp.hpp"
 *  \brief Multiple individual evaluation operator class.
 *  \ingroup EC
 *  \ingroup Op
 *  \ingroup FitStats
 */
class EvaluationMultipleOp : public Beagle::EvaluationOp
{

public:

	//! EvaluationMultipleOp allocator type.
	typedef Beagle::AbstractAllocT<EvaluationMultipleOp,Beagle::EvaluationOp::Alloc>
	Alloc;
	//! EvaluationMultipleOp handle type.
	typedef Beagle::PointerT<EvaluationMultipleOp,Beagle::EvaluationOp::Handle>
	Handle;
	//! EvaluationMultipleOp bag type.
	typedef Beagle::ContainerT<EvaluationMultipleOp,Beagle::EvaluationOp::Bag>
	Bag;

	explicit EvaluationMultipleOp(unsigned int inIndisPerCaseAndGroup=0,
	                              std::string inName="EvaluationMultipleOp");
	explicit EvaluationMultipleOp(unsigned int inIndisPerCase,
	                              unsigned int inIndisPerGroup,
	                              std::string inName="EvaluationMultipleOp");
	virtual ~EvaluationMultipleOp()
	{ }

	/*!
	 *  \brief Combine two fitnesses into one.
	 *  \param ioFitness1 First fitness to combine and result from the combination.
	 *  \param inFitness2 Second fitness to combine.
	 */
	virtual void combineFitnesses(Beagle::Fitness::Handle ioFitness1,
	                              const Beagle::Fitness::Handle inFitness2) const =0;

	/*!
	 *  \brief Evaluate the fitness of a set of individuals.
	 *  \param inIndividuals The individuals to evaluate.
	 *  \param ioContexts Evolutionary contexts.
	 *  \return Handle to the set of fitness values of the individuals.
	 *
	 *  The index of the individual will be the same as the index for
	 *  the individual's fitness.
	 */
	virtual Beagle::Fitness::Bag::Handle evaluateCase(Beagle::Individual::Bag& inIndividuals,
	        Beagle::Context::Bag& ioContexts) =0;

	/*!
	 *  \brief Set number of individual per evaluation cases.
	 *  \param inIndisPerCase Number of individual per evaluation cases.
	 */
	inline void setIndisPerCase(unsigned int inIndisPerCase) {
		Beagle_StackTraceBeginM();
		mIndisPerCase=inIndisPerCase;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set number of individual per groups.
	 *  \param inIndisPerGroup Number of individual per evaluation groups.
	 */
	inline void setIndisPerGroup(unsigned int inIndisPerGroup) {
		Beagle_StackTraceBeginM();
		mIndisPerGroup=inIndisPerGroup;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Number of individual per evaluation cases.
	 */
	inline unsigned int getIndisPerCase() const {
		Beagle_StackTraceBeginM();
		return mIndisPerCase;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Number of individual per groups.
	 */
	inline unsigned int getIndisPerGroup() const {
		Beagle_StackTraceBeginM();
		return mIndisPerGroup;
		Beagle_StackTraceEndM();
	}

	virtual Beagle::Individual::Handle    breed(Beagle::Individual::Bag& inBreedingPool,
	        Beagle::BreederNode::Handle inChild,
	        Beagle::Context& ioContext);
	virtual Beagle::Fitness::Handle       evaluate(Beagle::Individual& inIndividual, Beagle::Context& ioContext);
	virtual Beagle::Fitness::Bag::Handle  evaluateIndividuals(Beagle::Individual::Bag& ioIndividuals,
	        Beagle::Context::Bag& ioContexts);
	virtual void                          operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);


protected:

	struct Case : public Beagle::Object {
		//! Case allocator type.
		typedef Beagle::AbstractAllocT<Case,Beagle::Object::Alloc>
		Alloc;
		//! Case handle type.
		typedef Beagle::PointerT<Case,Beagle::Object::Handle>
		Handle;
		//! Case bag type.
		typedef Beagle::ContainerT<Case,Beagle::Object::Bag>
		Bag;

		virtual const std::string& getName() {
			const static std::string lName("Case");
			return lName;
		}

		std::vector<unsigned int> mIndices;
	};

	virtual unsigned int      enlargeGroup(Beagle::Individual::Bag& ioIndividuals, Beagle::Context::Bag& ioContexts);
	virtual Case::Bag::Handle pruneIgnorableCases(unsigned int inNumToIgnore);
	virtual void              setupCases(unsigned int inSize, Beagle::Context& ioContext);
	virtual void              setupCaseRecursive(unsigned int inSize,
	        unsigned int inLastIndex,
	        Case::Handle inCase);

	Case::Bag::Handle mCases;           //!< Evaluation cases.
	unsigned int      mIndisPerCase;    //!< Number of individual per cases.
	unsigned int      mIndisPerGroup;   //!< Number of individual per groups.

};

}

}

#endif // Beagle_EC_EvaluationMultipleOp_hpp
