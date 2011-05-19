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
 *  \file   beagle/HPC/SwitchTypeOp.cpp
 *  \brief  Source code of class HPC::SwitchTypeOp.
 *  \author Felix-Antoine Fortin
 *  $Revision: $
 *  $Date:  $
 */

#include "beagle/HPC.hpp"

using namespace Beagle;

/*!
 *  \brief Construct a switch type operator.
 *  \param inName Name of the operator.
 */
HPC::SwitchTypeOp::SwitchTypeOp(std::string inName) : Operator(inName) { }

/*!
 *  \brief Initialize the switch case operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void HPC::SwitchTypeOp::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	for(OperatorSetMap::iterator lIterMap = mOperatorSetMap.begin(); lIterMap != mOperatorSetMap.end(); ++lIterMap){
		for(Operator::Bag::iterator lIterBag = lIterMap->second.begin(); lIterBag != lIterMap->second.end(); ++lIterBag){
			Operator::Handle lOp = castHandleT<Operator>(*lIterBag);
			if(lOp->isInitialized()) continue;
			Beagle_LogTraceM(
				ioSystem.getLogger(),
				"switch case", "Beagle::Evolver",
				std::string("Initializing operator \"") + lOp->getName() + "\""
			);
			lOp->init(ioSystem);
			lOp->setInitializedFlag(true);
		}
	}
	Beagle_StackTraceEndM();
}

/*!
 *  \brief Apply the switch type operation.
 *  \param ioDeme Actual deme of the evolution.
 *  \param ioContext Context of the evolution.
 *  The operator use the MPICommunication handle to retrieve the type of the current node on
 *  which the evolution is executed. With this type, the operator can then retrieve the operator
 *  set corresponding to the type. It then call the operate function of each operator present in
 *  the set.
 */
void HPC::SwitchTypeOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	HPC::MPICommunication::Handle lMPI =
		castHandleT<HPC::MPICommunication>(ioContext.getSystem().getComponent("MPICommunication"));
	
	OperatorSetMap::iterator lIterMap = mOperatorSetMap.find(lMPI->getNodeType());
	Beagle_AssertM(lIterMap != mOperatorSetMap.end());
	
	Beagle_LogDetailedM(
		ioContext.getSystem().getLogger(),
		"switch case", "Beagle::SwitchTypeOp",
		"Applying "+lMPI->getNodeType()+" operator set."
	);  
	for(Operator::Bag::iterator lIterBag = lIterMap->second.begin(); lIterBag != lIterMap->second.end(); ++lIterBag){
		Operator::Handle lOp = castHandleT<Operator>(*lIterBag);
		Beagle_LogDetailedM(
			ioContext.getSystem().getLogger(),
			"switch case", "Beagle::SwitchTypeOp",
			std::string("Applying \"")+lOp->getName()+std::string("\"")
		); 
		lOp->operate(ioDeme, ioContext);
	}
	Beagle_StackTraceEndM();
}

/*!
 *  \brief Read switch case operator from XML subtree and system.
 *  \param inIter XML iterator from which the operator is read.
 *  \param ioSystem Beagle System.
 *  \throw IOException If a reading error occurs.
 */
void HPC::SwitchTypeOp::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
        Beagle_StackTraceBeginM();

        if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
                std::ostringstream lOSS;
                lOSS << "tag <" << getName() << "> expected!" << std::flush;
                throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
        }
	
	mOperatorSetMap.clear();
        for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
                if(lChild->getType() == PACC::XML::eData) {
			mOperatorSetMap[lChild->getValue()] = Operator::Bag();
			OperatorSetMap::iterator lIterMap = mOperatorSetMap.find(lChild->getValue());
			for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
				if(lChild2->getType() == PACC::XML::eData) {
					const std::string& lOpName = lChild2->getValue();
					Operator::Alloc::Handle lOpAlloc =
						castHandleT<Operator::Alloc>(ioSystem.getFactory().getAllocator(lOpName));
					if(lOpAlloc == 0) {
						std::ostringstream lOSS;
						lOSS << "Operator '" << lOpName << "' not found in factory!";
						throw Beagle_RunTimeExceptionM(lOSS.str());
					}
					Operator::Handle lOp = castHandleT<Operator>(lOpAlloc->allocate());
					lOp->setName(lOpName);
					lIterMap->second.push_back(lOp);
					lOp->readWithSystem(lChild2, ioSystem);
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
void HPC::SwitchTypeOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	
	for(OperatorSetMap::iterator lIterMap = mOperatorSetMap.begin(); lIterMap != mOperatorSetMap.end(); ++lIterMap){
		Beagle_LogTraceM(
			ioSystem.getLogger(),
			"SwitchTypeOp", "Beagle::Evolver",
			"Registering"+lIterMap->first+" set operator parameters"
		);
		for(Operator::Bag::iterator lIterBag = lIterMap->second.begin(); lIterBag != lIterMap->second.end(); ++lIterBag){
			Operator::Handle lOp = castHandleT<Operator>(*lIterBag);
			if(lOp->hasRegisteredParams()) continue;
			Beagle_LogTraceM(
				ioSystem.getLogger(),
				"SwitchTypeOp", "Beagle::Evolver",
				std::string("Registering '") + lOp->getName() + "' parameters"
			);
			lOp->registerParams(ioSystem);
			lOp->setRegisteredFlag(true);
		}
	}
	Beagle_StackTraceEndM();
}

/*!
 *  \brief Write switch type operator into an XML stream.
 *  \param ioStreamer XML stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 */
void HPC::SwitchTypeOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	for(OperatorSetMap::const_iterator lIterMap = mOperatorSetMap.begin(); lIterMap != mOperatorSetMap.end(); ++lIterMap){
		ioStreamer.openTag(lIterMap->first, inIndent);
		for(Operator::Bag::const_iterator lIterBag = lIterMap->second.begin(); lIterBag != lIterMap->second.end(); ++lIterBag){
			Operator::Handle lOp = castHandleT<Operator>(*lIterBag);
			lOp->write(ioStreamer, inIndent);
		}
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}
