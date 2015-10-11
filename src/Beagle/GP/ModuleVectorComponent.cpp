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
 *  \file   beagle/GP/src/ModuleVectorComponent.cpp
 *  \brief  Implementation of class ModuleVectorComponent.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:11 $
 */


#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct module vector GP system component for evolutionary module acquisition.
 */
GP::ModuleVectorComponent::ModuleVectorComponent() :
		Component("GP-ModuleVector")
{ }


/*!
 *  \brief Read module vector from XML subtree with a context.
 *  \param inIter XML iterator to read modules from.
 *  \param ioSystem Evolutionary system.
 */
void GP::ModuleVectorComponent::readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="GP-ModuleVector"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <GP-ModuleVector> expected!");

	const Factory& lFactory = ioSystem.getFactory();
	Context::Alloc::Handle lContextAlloc =
	    castHandleT<Context::Alloc>(lFactory.getConceptAllocator("Context"));
	GP::Context::Handle lGPContext =
	    castHandleT<GP::Context>(lContextAlloc->allocate());
	lGPContext->setSystemHandle(System::Handle(&ioSystem));
	GP::Tree::Alloc::Handle lTreeAlloc =
		castHandleT<GP::Tree::Alloc>(lFactory.getConceptAllocator("Genotype"));
	mModules.clear();
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Genotype")) {
			if(lChild->getFirstChild()==NULL) mModules.push_back(NULL);
			else {
				mModules.push_back(lTreeAlloc->allocate());
				lGPContext->setGenotypeHandle(mModules.back());
				lGPContext->setGenotypeIndex(mModules.size()-1);
				mModules.back()->readWithContext(lChild, *lGPContext);
			}
		}
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write modules into XML streamer.
 *  \param ioStreamer XML streamer to write modules into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::ModuleVectorComponent::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("GP-ModuleVector", inIndent);
	for(unsigned int i=0; i<mModules.size(); ++i) {
		if(mModules[i]==NULL) {
			ioStreamer.openTag("Genotype", inIndent);
			ioStreamer.closeTag();
		} else mModules[i]->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM();
}
