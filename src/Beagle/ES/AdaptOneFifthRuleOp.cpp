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
 *  \file   Beagle/ES/AdaptOneFifthRuleOp.cpp
 *  \brief  Source code of class ES::AdaptOneFifthRuleOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.9 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#include "Beagle/ES.hpp"

#include <string>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct an operator for 1/5th-rule sigma adaptation.
 *  \param inSigmaName Adapted sigma parameter name used in register.
 *  \param inName Name of the operator.
 */
ES::AdaptOneFifthRuleOp::AdaptOneFifthRuleOp(std::string inSigmaName, std::string inName) :
		BreederOp(inName),
		mMutateGaussSigmaName(inSigmaName)
{ }


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 */
double ES::AdaptOneFifthRuleOp::getBreedingProba(BreederNode::Handle inChild)
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(inChild);
	Beagle_NonNullPointerAssertM(inChild->getBreederOp());
	return inChild->getBreederOp()->getBreedingProba(inChild->getFirstChild());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register parameters for the real-valued Gaussian mutation operator.
 *  \param ioSystem System of the evolution.
 */
void ES::AdaptOneFifthRuleOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	BreederOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Number of mutation observations before adapting the value of the sigma ";
		lOSS << "according to the 1/5th-rule.";
		Register::Description lDescription(
		    "Sigma adaptation period",
		    "UInt",
		    "10",
		    lOSS.str()
		);
		mAdaptationPeriod = castHandleT<UInt>(
		                        ioSystem.getRegister().insertEntry("es.adapt.period", new UInt(10), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Number of successful mutation since last adaptation of sigma mutation parameter ";
		lOSS << "(internal parameter of the 1/5th-rule, do not modify).";
		Register::Description lDescription(
		    "Internal adaptation parameter",
		    "UInt",
		    "0",
		    lOSS.str()
		);
		mSuccessCount = castHandleT<UInt>(
		                    ioSystem.getRegister().insertEntry("es.adapt.success", new UInt(0), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Number of mutation observed since last adaptation of sigma mutation parameter ";
		lOSS << "(internal parameter of the 1/5th-rule, do not modify).";
		Register::Description lDescription(
		    "Internal adaptation parameter",
		    "UInt",
		    "0",
		    lOSS.str()
		);
		mMutationsCount = castHandleT<UInt>(
		                      ioSystem.getRegister().insertEntry("es.adapt.count", new UInt(0), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Modulation factor when adapting sigma values. Lower value means stronger ";
		lOSS << "modification of the sigma values at each adaptation. Value domain is ]0,1[.";
		Register::Description lDescription(
		    "Sigma adaptation factor",
		    "Double",
		    "0.85",
		    lOSS.str()
		);
		mSigmaAdaptFactor = castHandleT<Double>(
		                        ioSystem.getRegister().insertEntry("es.adapt.alpha", new Double(0.85), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Standard deviation of values added to real-values vector numbers at ";
		lOSS << "each Gaussian mutation. Value can be a scalar, which state the sigma for ";
		lOSS << "all float vector parameters, or a vector which state the sigma ";
		lOSS << "for the parameters individually. If the sigma vector size is smaller than the ";
		lOSS << "float vector size, the sigma used for the last values of the ";
		lOSS << "float vector is equal to the last value of the sigma vector.";
		Register::Description lDescription(
		    "Gaussian mutation std deviation",
		    "DoubleArray",
		    "0.1",
		    lOSS.str()
		);
		mMutateGaussSigma = castHandleT<DoubleArray>(
		                        ioSystem.getRegister().insertEntry(mMutateGaussSigmaName, new DoubleArray(1,0.1), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the adaptation operation on a breeding pool.
 *  \param inBreedingPool Breeding pool to give to the underlying breeding tree.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the mutation operation.
 *  \return Mutated individual.
 */
Individual::Handle ES::AdaptOneFifthRuleOp::breed(Individual::Bag& inBreedingPool,
        BreederNode::Handle inChild,
        Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(inChild);
	Beagle_NonNullPointerAssertM(inChild->getBreederOp());
	Individual::Handle lIndiv =
	    inChild->getBreederOp()->breed(inBreedingPool, inChild->getFirstChild(), ioContext);

	// Look whether the mutation is successful.
	Fitness::Handle lFitParent = inBreedingPool[ioContext.getIndividualIndex()]->getFitness();
	Fitness::Handle lFitChild = lIndiv->getFitness();
	if(lFitChild != NULL) {

		// Check whether the mutation is successful or not.
		const bool lSuccessful = (*lFitChild) > (*lFitParent);
		if(lSuccessful) {
			++mSuccessCount->getWrappedValue();
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "Successful mutation"
			);
		} else {
			Beagle_LogVerboseM(
			    ioContext.getSystem().getLogger(),
			    "The mutation is not successful"
			);
		}
		++mMutationsCount->getWrappedValue();

		// Adapt the sigma if the necessary number of observed mutations is reached.
		if(lSuccessful && (mMutationsCount->getWrappedValue()>=mAdaptationPeriod->getWrappedValue())) {
			const double lSuccessRate = double(mSuccessCount->getWrappedValue()) /
			                            double(mMutationsCount->getWrappedValue());
			if(lSuccessRate > 0.2) {
				for(unsigned int i=0; i<mMutateGaussSigma->size(); ++i) {
					(*mMutateGaussSigma)[i] /= mSigmaAdaptFactor->getWrappedValue();
				}
				Beagle_LogDetailedM(
				    ioContext.getSystem().getLogger(),
				    string("Increasing the sigma value by a factor ")+
				    dbl2str(1./mSigmaAdaptFactor->getWrappedValue())
				);
				Beagle_LogVerboseM(
				    ioContext.getSystem().getLogger(),
				    string("Sigma value after the increase: ")+mMutateGaussSigma->serialize()
				);
			} else if(lSuccessRate < 0.2) {
				for(unsigned int i=0; i<mMutateGaussSigma->size(); ++i) {
					(*mMutateGaussSigma)[i] *= mSigmaAdaptFactor->getWrappedValue();
				}
				Beagle_LogDetailedM(
				    ioContext.getSystem().getLogger(),
				    string("Reducing the sigma value by a factor ")+
				    dbl2str(mSigmaAdaptFactor->getWrappedValue())
				);
				Beagle_LogVerboseM(
				    ioContext.getSystem().getLogger(),
				    string("Sigma value after the reduction: ")+mMutateGaussSigma->serialize()
				);
			} else {
				Beagle_LogDetailedM(
				    ioContext.getSystem().getLogger(),
				    "Sigma value unchanged"
				);
			}
		}
	}

	return lIndiv;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Apply the adaptation operation on the deme.
 *  \param ioDeme Current deme of individuals to mutate.
 *  \param ioContext Context of the evolution.
 */
void ES::AdaptOneFifthRuleOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	throw Beagle_UndefinedMethodInternalExceptionM("operate", "ES::AdaptOneFifthRuleOp", getName());
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a sigma adaptation operator from a XML subtree.
 *  \param inIter XML iterator to use to read adaptation operator.
 *  \param ioSystem Evolutionary system.
 */
void ES::AdaptOneFifthRuleOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	string lMutateGaussSigmaReadName = inIter->getAttribute("mutgausssigma");
	if(lMutateGaussSigmaReadName.empty() == false) mMutateGaussSigmaName = lMutateGaussSigmaReadName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write sigma adaptation operator into XML streamer.
 *  \param ioStreamer XML streamer to write adaptation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void ES::AdaptOneFifthRuleOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	Beagle::BreederOp::writeContent(ioStreamer, inIndent);
	ioStreamer.insertAttribute("mutgausssigma", mMutateGaussSigmaName);
	Beagle_StackTraceEndM();
}
