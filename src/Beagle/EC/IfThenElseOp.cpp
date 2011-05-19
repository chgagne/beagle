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
 *  \file   Beagle/EC/IfThenElseOp.cpp
 *  \brief  Source code of class IfThenElseOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20 $
 *  $Date: 2007/08/10 20:32:22 $
 */

#include "Beagle/EC.hpp"

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a if-then-else operator.
 *  \param inConditionTag Tag in the register of the parameter to test.
 *  \param inConditionValue Value the test of the value.
 *  \param inName Name of the operator.
 */
IfThenElseOp::IfThenElseOp(std::string inConditionTag,
                           std::string inConditionValue,
                           std::string inName) :
	Beagle::Operator(inName),
	mConditionTag(inConditionTag),
	mConditionValue(inConditionValue)
{ }


/*!
 *  \brief Apply the if-then-else operation.
 *  \param ioDeme Actual deme of the evolution.
 *  \param ioContext Context of the evolution.
 *  \throw RunTimeException If condition tag does not to valid parameter.
 *  \par Note:
 *    The operator first look for the tagged parameter in the register. If the parameter
 *    is not in the register, RunTimeException is throw. If the value is registered,
 *    a test is made with the value contained in the operator. If the values are equal,
 *    then the positive operator set is executed, else the negative operator set is executed.
 */
void IfThenElseOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	if(ioContext.getSystem().getRegister().isRegistered(mConditionTag) == false) {
		std::ostringstream lOSS;
		lOSS << "Tag '" << mConditionTag << "' does not refer to a registered parameter!";
		throw Beagle_RunTimeExceptionM(lOSS.str());
	}
	Object::Handle lTestedObject = ioContext.getSystem().getRegister().getEntry(mConditionTag);
	std::string lTestedValue = lTestedObject->serialize();

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    std::string("if-then-else parameter is '") + mConditionTag + "=" + mConditionValue
	);
	if(lTestedValue == mConditionValue) {
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    "Applying positive operator set"
		);
		for(unsigned int i=0; i<mPositiveOpSet.size(); i++) {
			Beagle_LogDetailedM(
			    ioContext.getSystem().getLogger(),
			    std::string("Applying '")+mPositiveOpSet[i]->getName()+"'"
			);
			mPositiveOpSet[i]->operate(ioDeme, ioContext);
		}
	} else {
		Beagle_LogDetailedM(
		    ioContext.getSystem().getLogger(),
		    "Applying negative operator set"
		);
		for(unsigned int i=0; i<mNegativeOpSet.size(); i++) {
			Beagle_LogDetailedM(
			    ioContext.getSystem().getLogger(),
			    std::string("Applying '")+mNegativeOpSet[i]->getName()+std::string("'")
			);
			mNegativeOpSet[i]->operate(ioDeme, ioContext);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize this operator.
 *  \param ioSystem System to use to post-init the operator.
 */
void IfThenElseOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// initializing positive set operators
	for(unsigned int i = 0; i < mPositiveOpSet.size(); i++) {
		if(mPositiveOpSet[i]->isInitialized() == false) {
			Beagle_LogTraceM(
			    ioSystem.getLogger(),
			    std::string("Initializing positive set operator '") + mPositiveOpSet[i]->getName() + "'"
			);
			mPositiveOpSet[i]->init(ioSystem);
			mPositiveOpSet[i]->setInitializedFlag(true);
		}
	}

	// initializing negative set operators
	for(unsigned int i = 0; i < mNegativeOpSet.size(); i++) {
		if(mNegativeOpSet[i]->isInitialized() == false) {
			Beagle_LogTraceM(
			    ioSystem.getLogger(),
			    std::string("Initializing negative set operator '") + mNegativeOpSet[i]->getName() + "'"
			);
			mNegativeOpSet[i]->init(ioSystem);
			mNegativeOpSet[i]->setInitializedFlag(true);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read if-then-else operator from XML subtree.
 *  \param inIter XML iterator from which the operator is read.
 *  \param ioSystem System to use to read operator.
 *  \throw IOException If a reading error occurs.
 */
void IfThenElseOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();

	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	mConditionTag = inIter->getAttribute("parameter");
	if(mConditionTag.empty()) {
		std::ostringstream lOSS;
		lOSS << "attribute 'parameter' for operator " << getName() << " expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	mConditionValue = inIter->getAttribute("value");

	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType() == PACC::XML::eData) {
			if(lChild->getValue() == "PositiveOpSet") {
				mPositiveOpSet.clear();
				for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
					if(lChild2->getType() == PACC::XML::eData) {
						std::string lOpName = lChild2->getValue();
						Operator::Alloc::Handle lOpAlloc =
						    castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lOpName));
						if(lOpAlloc == NULL) {
							std::ostringstream lOSS;
							lOSS << "Operator '" << lOpName << "' not found in factory!";
							throw Beagle_RunTimeExceptionM(lOSS.str());
						}
						Operator::Handle lOp = castHandleT<Operator>(lOpAlloc->allocate());
						lOp->setName(lOpName);
						mPositiveOpSet.push_back(lOp);
						lOp->readWithSystem(lChild2, ioSystem);
					}
				}
			} else if(lChild->getValue() == "NegativeOpSet") {
				mNegativeOpSet.clear();
				for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
					if(lChild2->getType() == PACC::XML::eData) {
						std::string lOpName = lChild2->getValue();
						Operator::Alloc::Handle lOpAlloc =
						    castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lOpName));
						if(lOpAlloc == NULL) {
							std::ostringstream lOSS;
							lOSS << "Operator '" << lOpName << "' not found in factory!";
							throw Beagle_RunTimeExceptionM(lOSS.str());
						}
						Operator::Handle lOp = castHandleT<Operator>(lOpAlloc->allocate());
						lOp->setName(lOpName);
						mNegativeOpSet.push_back(lOp);
						lOp->readWithSystem(lChild2, ioSystem);
					}
				}
			}
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Register the paramaters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void IfThenElseOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	// register positive set parameters
	Beagle_LogTraceM(
	    ioSystem.getLogger(),
	    "Registering positive set operator parameters"
	);
	for(unsigned int i = 0; i < mPositiveOpSet.size(); i++) {
		if(mPositiveOpSet[i]->hasRegisteredParams() == false) {
			Beagle_LogTraceM(
			    ioSystem.getLogger(),
			    std::string("Registering '") + mPositiveOpSet[i]->getName() + "' parameters"
			);
			mPositiveOpSet[i]->registerParams(ioSystem);
			mPositiveOpSet[i]->setRegisteredFlag(true);
		}
	}

	// register negative set parameters
	Beagle_LogTraceM(
	    ioSystem.getLogger(),
	    "Registering negative set operator parameters"
	);
	for(unsigned int i = 0; i < mNegativeOpSet.size(); i++) {
		if(mNegativeOpSet[i]->hasRegisteredParams() == false) {
			Beagle_LogTraceM(
			    ioSystem.getLogger(),
			    std::string("Registering '") + mNegativeOpSet[i]->getName() + "' parameters"
			);
			mNegativeOpSet[i]->registerParams(ioSystem);
			mNegativeOpSet[i]->setRegisteredFlag(true);
		}
	}

	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write if-then-else operator into an XML stream.
 *  \param ioStreamer XML stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 */
void IfThenElseOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();

	ioStreamer.openTag(getName(), inIndent);
	ioStreamer.insertAttribute("parameter", mConditionTag);
	ioStreamer.insertAttribute("value", mConditionValue);
	ioStreamer.openTag("PositiveOpSet", inIndent);
	for(unsigned int i=0; i<mPositiveOpSet.size(); ++i) mPositiveOpSet[i]->write(ioStreamer, inIndent);
	ioStreamer.closeTag();
	ioStreamer.openTag("NegativeOpSet", inIndent);
	for(unsigned int i=0; i<mNegativeOpSet.size(); ++i) mNegativeOpSet[i]->write(ioStreamer, inIndent);
	ioStreamer.closeTag();
	ioStreamer.closeTag();

	Beagle_StackTraceEndM();
}




