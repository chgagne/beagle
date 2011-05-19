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
 *  \file   beagle/Distrib/Island/src/ransportSendOp.cpp
 *  \brief  Source code of class TransportSendOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/Distrib/Island.hpp"

using namespace Beagle;
using namespace Beagle::Distrib;


/*!
 *  \brief Construct TransportSendOp with name inName
 *  \param inCommsName Name of the Communications component.
 *  \param inName Name of this operator.
 */
Island::TransportSendOp::TransportSendOp(const std::string& inCommsName,
        const std::string& inTransCompName,
        const std::string& inDefaultRecipient,
        const std::string& inName) :
		SendOp(inCommsName,inDefaultRecipient,inName),
		mSelectionOp( new SelectRandomOp ),
		mTransCompName(inTransCompName)
{}


/*!
 *  \brief Construct TransportSendOp with name inName
 *  \param inSelectionOp Selection operator used to select individuals to send.
 *  \param inCommsName Name of the Communications component.
 *  \param inName Name of this operator.
 */
Island::TransportSendOp::TransportSendOp(SelectionOp::Handle inSelectionOp,
        const std::string& inCommsName,
        const std::string& inTransCompName,
        const std::string& inDefaultRecipient,
        const std::string& inName) :
		SendOp(inCommsName,inDefaultRecipient,inName),
		mSelectionOp(inSelectionOp),
		mTransCompName(inTransCompName)
{}



/*!
 *  \brief TransportSendOp destructor.
 */
Island::TransportSendOp::~TransportSendOp()
{}


/*!
 *  \brief Register TransportSendOp's parameters.
 *  \param ioSystem Reference to the evolutionary system.
 */
void Island::TransportSendOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle_LogVerboseM(
	    ioSystem.getLogger(),
	    "transport", "Beagle::TransportSendOp",
	    "Initializing TransportSendOp"
	);
	// Add interval parameter
	{
		std::ostringstream lOSS;
		lOSS << "Interval between each transport send, in number of generations. ";
		lOSS << "An interval of 0 disables send.";
		Register::Description lDescription(
		    "Interval between send transportations",
		    "UInt",
		    "1",
		    lOSS.str()
		);
		mTransportInterval = castHandleT<UInt>(
		                         ioSystem.getRegister().insertEntry("ec.transport.sendinterval", new UInt(1), lDescription));
	}
	// Add proportion to send parameter
	{
		Register::Description lDescription(
		    "Proportion to transport",
		    "Float",
		    "0.05",
		    "Proportion of the population to send (sent individuals are not removed from the deme)"
		);
		mProportionToSend = castHandleT<Float>(
		                        ioSystem.getRegister().insertEntry("ec.transport.send", new Float(0.05f), lDescription));
	}
	Beagle_StackTraceEndM();
}



/*!
 *  \brief Transport the best individuals.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 *
 *  The best individuals a selected and sent to all specified
 *  addresses.  Addresses are specified in the parameters file.
 */
void Island::TransportSendOp::operate(Deme &ioDeme, Context &ioContext)
{
	Beagle_StackTraceBeginM();
	// Enforce transport interval
	unsigned int lTransportInterval = mTransportInterval->getWrappedValue();
	if((lTransportInterval == 0) || (ioContext.getGeneration() == 0)) return;
	if((ioContext.getGeneration() % lTransportInterval) != 0) return;

	// Check there are individuals in the deme
	if (ioDeme.empty()) {
		Beagle_LogBasicM(
		    ioContext.getSystem().getLogger(),
		    "transport", "Beagle::Island::TransportSendOp",
		    "WARNING: There are no individuals in the deme, unable to transport any individuals"
		);
		return;
	}

	// Calculate the number of indiviuals to send
	unsigned int lNumberToSend
	= static_cast<unsigned int>(std::ceil(mProportionToSend->getWrappedValue() * ioDeme.size()));

	// Get index of individuals to transport.
	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "transport", "Beagle::Island::TransportSendOp",
	    std::string("Selecting ")+uint2str(lNumberToSend)+" individuals for transportation from "+
	    uint2ordinal(ioContext.getDemeIndex()+1)+" deme using '"+mSelectionOp->getName()+"'"
	);
	std::vector<unsigned int> lIndices;
	const unsigned int lOldSelectionIndex = ioContext.getSelectionIndex();
	ioContext.setSelectionIndex(0);
	mSelectionOp->selectNIndividuals(lNumberToSend, ioDeme, ioContext, lIndices);
	mSelectionOp->convertToList(lNumberToSend, lIndices);
	ioContext.setSelectionIndex(lOldSelectionIndex);
	std::random_shuffle(lIndices.begin(), lIndices.end(),
	                    ioContext.getSystem().getRandomizer());

	// Insert selected indices into TransportComponent
	TransportComponent::Handle lTransComp
	= castHandleT<TransportComponent>(ioContext.getSystem().getComponent( mTransCompName ));
	lTransComp->insertIndices(lIndices, ioContext.getDemeIndex());

	// Put the individuals into a bag
	Individual::Bag lBag;
	lBag.resize(lNumberToSend);
	for(unsigned int i=0; i<lIndices.size(); i++) {
		lBag[i] = ioDeme[lIndices[i]];
	}

	// Send the bag of individuals
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "transport", "Beagle::Island::TransportSendOp",
	    std::string("Transporting ")+uint2str(lBag.size())+
	    std::string(" individuals")
	);
	send(lBag,ioContext);
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "transport", "Beagle::Island::TransportSendOp",
	    std::string("Transport complete")
	);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read migration map classes from configuration file.
 *  \param inIter XML iterator from which the operator is read.
 *  \param System Evolutionary system.
 *
 *  This method reads the 'select' attribute so that mSelectionOp can
 *  be specified from a configuration file.
 */
void Island::TransportSendOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	SendOp::readWithSystem(inIter, ioSystem);
	if (inIter->isDefined("select")) {
		std::string lSelectOpName = inIter->getAttribute("select").c_str();
		Operator::Alloc::Handle lSelectOpAlloc =
		    castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lSelectOpName));
		if(lSelectOpAlloc == NULL) {
			std::ostringstream lOSS;
			lOSS << "Operator '" << lSelectOpName << "' not found in factory!";
			throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
		}
		mSelectionOp = castHandleT<SelectionOp>(lSelectOpAlloc->allocate());
		mSelectionOp->setName(lSelectOpName);
	}
	Beagle_StackTraceEndM("void TransportSendOp::readWithSystem(PACC::XML::ConstIterator, System&)")
}


/*!
 *  \brief Send the individuals to all specified addresses.
 *  \param inIndividuals Individuals to send.
 *  \param ioContext Evolutionary context of the operation.
 */
void Island::TransportSendOp::send(const Individual::Bag& inIndividuals, Context &ioContext)
{
	Beagle_StackTraceBeginM();
	// Get Comms component
	System& lSystem = ioContext.getSystem();
	Communications::Handle lComms
	= castHandleT<Communications>(lSystem.getComponent( mCommsName ));

	// Send individuals to every address
	std::string lMessage = inIndividuals.serialize();

	Beagle_LogTraceM(
	    ioContext.getSystem().getLogger(),
	    "transport", "Beagle::Island::TransportSendOp",
	    std::string("Transporting individuals")
	);
	sendToAll(lMessage, ioContext);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write an operator into an XML stream.
 *  \param ioStreamer XML stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 *
 *  Writes "comms" attribute, that declares the name of the
 *  communications component to use, and a "select" attribute, that
 *  declares the selection operator that was installed at the time of
 *  calling write().  Addresses that have been specified for sending
 *  will also be written.
 */
void Island::TransportSendOp::write(PACC::XML::Streamer &ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName().c_str(), inIndent);

	ioStreamer.insertAttribute("comms",mCommsName);
	if (mSelectionOp!=NULL) {
		ioStreamer.insertAttribute("select",mSelectionOp->getName());
	}

	// Write addresses
	writeAddresses(ioStreamer, inIndent);

	ioStreamer.closeTag(); // getName()
	Beagle_StackTraceEndM();
}

