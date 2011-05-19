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
 *  \file   beagle/Distrib/Island/src/TransportReceiveOp.cpp
 *  \brief  Source code of class TransportReceiveOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.18 $
 *  $Date: 2007/08/17 20:57:59 $
 */

#include "beagle/Distrib/Island.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Construct TransportReceiveOp with name inName.
 *  \param inCommsName Name of the Communications component.
 *  \param inName Name of this operator.
 */
Island::TransportReceiveOp::TransportReceiveOp(const std::string& inCommsName,
        const std::string& inTransCompName,
        const std::string& inName) :
		Operator(inName),
		mReplacementOp(new SelectRandomOp),
		mCommsName(inCommsName),
		mTransCompName(inTransCompName)
{}



/*!
 *  \brief Construct TransportReceiveOp with replacement selection operator.
 *  \param inReplacementOp Replacement selection operator.
 *  \param inCommsName Name of the Communications component.
 *  \param inName Name of this operator.
 */
Island::TransportReceiveOp::TransportReceiveOp(SelectionOp::Handle inReplacementOp,
        const std::string& inCommsName,
        const std::string& inTransCompName,
        const std::string& inName) :
		Operator(inName),
		mReplacementOp(inReplacementOp),
		mCommsName(inCommsName),
		mTransCompName(inTransCompName)
{}

/*!
 *  \brief TransportReceiveOp destructor.
 */
Island::TransportReceiveOp::~TransportReceiveOp()
{}


/*!
 *  \brief Register TransportReceiveOp's parameters.
 *  \param ioSystem Evolutionary system.
 */
void Island::TransportReceiveOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	// Add interval parameter
	{
		std::ostringstream lOSS;
		lOSS << "Interval between each transport receive, in number of generations. ";
		lOSS << "An interval of 0 disables receive.";
		Register::Description lDescription(
		    "Interval between receive transportations",
		    "UInt",
		    "1",
		    lOSS.str()
		);
		mTransportInterval = castHandleT<UInt>(
		                         ioSystem.getRegister().insertEntry("ec.transport.recvinterval",new UInt(1), lDescription));
	}

	// Add max replacement parameter
	{
		std::ostringstream lOSS;
		lOSS << "Maximum proportion of the population to replace.  If this value is ";
		lOSS << "reached and still more individuals have been received, then the most ";
		lOSS << "recently received individuals will replace randomly selected older replacements.";
		Register::Description lDescription(
		    "Maximum proportion of the population to replace",
		    "Double",
		    "0.20",
		    lOSS.str()
		);
		mMaxReplace = castHandleT<Double>
		              (ioSystem.getRegister().insertEntry("ec.transport.maxreplace",
		                                                  new Double(0.20),
		                                                  lDescription));
	}

	// Add "replaceop" parameter
	{
		std::string lReplacementOp;
		if (mReplacementOp != NULL) lReplacementOp = mReplacementOp->getName();
		else lReplacementOp = "<not specified>";
		Register::Description lDescription(
		    "Flag for use of replacement selection operator",
		    "Bool",
		    "0",
		    std::string("Flag for use of replacement selection operator.  A value of 0 (false)")+
		    " disables the use of the replacement selection operator (currently '"+
		    lReplacementOp+"')."
		);
		mUseReplaceOp = castHandleT<Bool>
		                (ioSystem.getRegister().insertEntry("ec.transport.replaceop",
		                                                    new Bool(0),
		                                                    lDescription));
	}

	// Add "replacesent" parameter
	{
		std::ostringstream lOSS;
		lOSS << "Flag to indicate if incoming individuals should replace sent individuals.  "
		<< "If both this and 'ec.transport.replaceop' are set, then sent individuals are replaced "
		<< "first and any unplaced individuals will replace those selected by the replacement selection "
		<< "operator.  If this is set and 'ec.transport.replaceop' is not set then unplaced individuals "
		<< "will be discarded.";
		Register::Description lDescription(
		    "Flag for incoming individuals to replace sent individuals",
		    "Bool",
		    "1",
		    lOSS.str()
		);
		mReplaceSent = castHandleT<Bool>
		               (ioSystem.getRegister().insertEntry("ec.transport.replacesent",
		                                                   new Bool(1),
		                                                   lDescription));
	}

	Beagle_StackTraceEndM();
}


void Island::TransportReceiveOp::init(System& ioSystem)
{
	// Add to Comm's do-not-purge list
	if(mCommsName.empty()) {
		mCommsName = ioSystem.getFactory().getConceptTypeName("DBIM-Communications");
	}
	Communications::Handle lComms
	= castHandleT<Communications>(ioSystem.getComponent( mCommsName ));
	lComms->doNotPurge(getName());
}




/*!
 *  \brief Receive transported individuals via Communications component.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 *
 *  TransportReceiveOp replaces the worst individuals in the deme.
 *  The number of individuals that are replaced is limited by the
 *  register variable 'ec.transport.maxreplace'.
 */
void Island::TransportReceiveOp::operate(Deme &ioDeme, Context &ioContext)
{
	Beagle_StackTraceBeginM();
	// Enforce transport interval
	unsigned int lTransportInterval = mTransportInterval->getWrappedValue();
	if((lTransportInterval == 0) ||
	        (ioContext.getGeneration() == 0) ||
	        (ioContext.getGeneration() % lTransportInterval) != 0) {
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    "transport", "Beagle::TransportReceiveOp",
		    std::string("Transport interval is set to ")+uint2str(lTransportInterval)+" generations for '"+
		    getName()+"'.  Skipping this "+uint2ordinal(ioContext.getGeneration())+" generation"
		);
		return;
	}

	// Check that mMaxReplace isn't zero
	if(mMaxReplace->getWrappedValue()==0.0) return;

	// Receive individuals
	Individual::Bag lBag;
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "transport", "Beagle::TransportReceiveOp",
	    std::string("Receiving transported individuals")
	);
	receive(lBag,ioContext);

	// Check that the bag is not empty
	if (lBag.empty()) {
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    "transport", "Beagle::TransportReceiveOp",
		    "No individuals were received"
		);
		return;
	}
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "transport", "Beagle::TransportReceiveOp",
	    std::string("Received ")+uint2str(lBag.size())+" individuals."
	);

	// Calculate the maximum number of individuals to replace
	unsigned int lMaxReplace =
	    (unsigned int)(mMaxReplace->getWrappedValue()*double(ioDeme.size())+0.5);
	if(lMaxReplace==0) lMaxReplace=1;
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "transport", "Beagle::TransportReceiveOp",
	    std::string("The maximum number of individuals to replace is ")+uint2str(lMaxReplace)
	);

	// Find how many individuals should be replaced (based on number received and lMaxReplace).
	unsigned int lNumberToReplace = lBag.size();
	if (lNumberToReplace>lMaxReplace) lNumberToReplace = lMaxReplace;
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "transport", "Beagle::Island::TransportReceiveOp",
	    std::string("Replacing ")+uint2str(lNumberToReplace)+" individuals"
	);


	// Check that selection will occur
	if (!(mReplaceSent->getWrappedValue() || mUseReplaceOp->getWrappedValue())) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "transport", "Beagle::Island::TransportReceiveOp",
		    std::string("Warning!  No individuals will be replaced in TransportReceiveOp because both ")+
		    "'ec.transport.replaceop' and 'ec.transport.replacesent' are set to false"
		);
	}

	// Select indices of individuals for replacement using the indices of those who were sent
	std::vector<unsigned int> lIndices;
	if (mReplaceSent->getWrappedValue()) {
		TransportComponent::Handle lTransComp
		= castHandleT<TransportComponent>(ioContext.getSystem().getComponent( mTransCompName ));
		lTransComp->getIndices(lNumberToReplace, lIndices, ioContext.getDemeIndex());
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    "transport", "Beagle::Island::TransportReceiveOp",
		    std::string("Obtained ")+uint2str(lIndices.size())+" indices from transport component"
		);
		lNumberToReplace -= lIndices.size();
	}

	// Select indices of individuals for replacement using replacement selection operator
	if (mUseReplaceOp->getWrappedValue()) {
		unsigned int lNumberToSelect = lNumberToReplace - lIndices.size();
		Beagle_LogTraceM(
		    ioContext.getSystem().getLogger(),
		    "transport", "Beagle::Island::TransportReceiveOp",
		    std::string("Selecting (with '")+mReplacementOp->getName()+"')"+
		    uint2str(lNumberToSelect)+" individuals from the "+
		    uint2ordinal(ioContext.getDemeIndex()+1)+" deme to be replaced by incoming individuals"
		);
		std::vector<unsigned int> lIndices2;
		const unsigned int lOldSelectionIndex = ioContext.getSelectionIndex();
		ioContext.setSelectionIndex(0);
		mReplacementOp->selectNIndividuals(lNumberToSelect, ioDeme, ioContext, lIndices2);
		mReplacementOp->convertToList(lNumberToSelect, lIndices2);
		ioContext.setSelectionIndex(lOldSelectionIndex);
		std::random_shuffle(lIndices2.begin(), lIndices2.end(),
		                    ioContext.getSystem().getRandomizer());

		// Merge the selected indices together
		lIndices.insert(lIndices.end(),lIndices2.begin(),lIndices2.end());
	}


	// Replace the selected individuals with received individuals
	for (unsigned int i=0; i<lIndices.size(); i++) {
		unsigned int lReplaceIndex = lIndices[i];
		Beagle_LogVerboseM(
		    ioContext.getSystem().getLogger(),
		    "transport", "Beagle::TransportReceiveOp",
		    std::string("Replacing ")+uint2ordinal(lReplaceIndex+1)+
		    " individual"
		);
		ioDeme[lReplaceIndex]->copy(*(lBag[lBag.size()-i-1]), ioContext.getSystem());
		ioDeme[lReplaceIndex]->getFitness()->setInvalid();

		// Record history
		History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));
		if (lHistory != NULL) {
			lHistory->allocateNewID(*(ioDeme[lReplaceIndex]));
			lHistory->trace(ioContext, std::vector<HistoryID>(), ioDeme[lReplaceIndex], getName(), "migration");
		}

	}
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "transport", "Beagle::TransportReceiveOp",
	    std::string("Transportation complete; ")+
	    uint2str(lIndices.size())+" individuals incorporated into deme; "+
	    uint2str(lNumberToReplace)+" individuals discarded"
	);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read migration map classes from configuration file.
 *  \param inIter XML iterator from which the operator is read.
 *  \param System Evolutionary system.
 *
 *  This method reads the 'replace' attribute so that mReplacementOp can
 *  be specified from a configuration file.
 */
void Island::TransportReceiveOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Operator::readWithSystem(inIter, ioSystem);
	if (inIter->isDefined("replace")) {
		std::string lReplaceOpName = inIter->getAttribute("replace").c_str();
		Operator::Alloc::Handle lReplaceOpAlloc =
		    castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lReplaceOpName));
		if(lReplaceOpAlloc == NULL) {
			std::ostringstream lOSS;
			lOSS << "Operator '" << lReplaceOpName << "' not found in operator map!";
			throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
		}
		mReplacementOp = castHandleT<SelectionOp>(lReplaceOpAlloc->allocate());
		mReplacementOp->setName(lReplaceOpName);
	}
	Beagle_StackTraceEndM("void TransportReceiveOp::readWithSystem(PACC::XML::ConstIterator, System&)")
}


/*!
 *  \brief Actually receives the transported individuals via Communications component.
 *  \param outIndividuals Received individuals (most desirable at the back).
 *  \param ioContext Evolutionary context of the operation.
 *
 *  TransportReceiveOp replaces the worst individuals in the deme.
 *  The number of individuals that are replaced is limited by the
 *  register variable 'ec.transport.maxreplace'.
 */
void Island::TransportReceiveOp::receive(Individual::Bag& outIndividuals, Context &ioContext)
{
	Beagle_StackTraceBeginM();
	// Get Comms component
	System& lSystem = ioContext.getSystem();
	Communications::Handle lComms
	= castHandleT<Communications>(lSystem.getComponent( mCommsName ));

	std::string lMessage;
	PACC::XML::Document lParser;
	Individual::Bag lIndividuals;
	while (lComms->getNextMessage( getName(), lMessage, ioContext )) {
		// Parse and read message
		std::istringstream lISS(lMessage);
		lParser.parse(lISS,"istringstream");
		PACC::XML::ConstIterator lNode = lParser.getFirstRoot();
		if (lNode) {
			// Parse message
			lIndividuals.readWithContext( lNode, ioContext );
			// Sort individuals
			std::sort(lIndividuals.begin(),lIndividuals.end(),
			          IsMorePointerPredicate());
			// Copy lIndividuals to outIndividuals
			unsigned int lCurrentSize = outIndividuals.size();
			unsigned int lIncrement = lIndividuals.size();
			outIndividuals.resize(lCurrentSize + lIncrement);
			for (unsigned int i = 0; i<lIncrement; ++i) {
				outIndividuals[lCurrentSize+i] = lIndividuals[i];
			}
		}
	}
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "transport", "Beagle::TransportReceiveOp",

	    std::string("Received a total of ")+
	    uint2str(outIndividuals.size())+
	    " individuals"
	);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write an operator into an XML stream.
 *  \param ioStreamer XML stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 *
 *  Writes "comms" attribute, that declares the name of the
 *  communications component to use, and a "replace" attribute, that
 *  declares the selection operator that was installed at the time of
 *  calling write().
 */
void Island::TransportReceiveOp::write(PACC::XML::Streamer &ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName().c_str(), inIndent);

	ioStreamer.insertAttribute("comms",mCommsName);
	if (mReplacementOp!=NULL) {
		ioStreamer.insertAttribute("replace",mReplacementOp->getName());
	}
	ioStreamer.closeTag(); // getName()
	Beagle_StackTraceEndM();
}

