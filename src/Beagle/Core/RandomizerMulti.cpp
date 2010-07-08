/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2009 by Christian Gagne and Marc Parizeau
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
 *  \file   Beagle/Core/RandomizerMulti.cpp
 *  \brief  Source code of class RandomizerMulti.
 *	\author Felix-Antoine Fortin
 *	\author Francois-Michel De Rainville
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: $
 *  $Date: $
 */

#include "Beagle/Core.hpp"
#ifdef BEAGLE_HAVE_OPENMP
#include <omp.h>
#endif

/*!
 *  \brief Construct a component formed of multiple randomizers.
 *  \param inRandomizers Bag containing the randomizers to use by the RandomizerMulti.
 */
Beagle::RandomizerMulti::RandomizerMulti(Randomizer::Bag::Handle inRandomizers)
 : Beagle::Component("RandomizerMulti"), mRandomizers(inRandomizers)
{
	if(mRandomizers == 0){
		mRandomizers = new Randomizer::Bag(1);
		(*mRandomizers)[0] = new Randomizer;
	}
}

/*!
*  \brief Register the randomizer seeds parameter, and build each randomizer.
 *  \param ioSystem Reference to the system.
 */
void Beagle::RandomizerMulti::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	std::ostringstream lStream;
	lStream << "Randomizer seed. A zero value means that ";
#ifdef BEAGLE_HAVE_DEV_URANDOM
	lStream << "the seed will be initialized using the /dev/urandom device.";
#else // BEAGLE_HAVE_DEV_URANDOM
	lStream << "the seed will be initialized using the current system time.";
#endif // BEAGLE_HAVE_DEV_URANDOM
	Register::Description lDescription(
	    "Randomizer seed",
	    "ULongArray",
	    "0",
	    lStream.str()
	);
	
	mRegisteredSeed = castHandleT<ULongArray>(
		ioSystem.getRegister().insertEntry("ec.rand.seed", new ULongArray(1,0), lDescription));
	for(unsigned int i = 0; i < mRandomizers->size(); ++i){
		(*mRandomizers)[i]->registerParams(ioSystem);
		(*mRandomizers)[i]->setRegisteredFlag(true);	
	}
	Beagle_StackTraceEndM("void RandomizerMulti::registerParams(System&)");
}

/*!
 *  \brief Initialize each randomizer of the bag.
 *  \param ioSystem Evolutionary system.
 */
void Beagle::RandomizerMulti::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	OpenMP::Handle lOpenMP = castHandleT<OpenMP>(ioSystem.getComponent("OpenMP"));
	
	if(mRandomizers->size() != lOpenMP->getMaxNumThreads())
		mRandomizers = new Randomizer::Bag(lOpenMP->getMaxNumThreads());
	if(mRegisteredSeed->size() != mRandomizers->size())
		mRegisteredSeed->resize(mRandomizers->size(),0);
	#pragma omp parallel for ordered schedule(static)
	for(int i = 0; i < (int)mRandomizers->size(); ++i){
		if((*mRandomizers)[i] == NULL)
			(*mRandomizers)[i] = new Randomizer;
		(*mRandomizers)[i]->registerParams(ioSystem);
		(*mRandomizers)[i]->setRegisteredFlag(true);
		#pragma omp ordered
		(*mRandomizers)[i]->init(ioSystem);
		(*mRandomizers)[i]->setInitializedFlag(true);
	}
	Beagle_StackTraceEndM("void RandomizerMulti::init(System&)");
}

/*!
 *  \brief Read the state of the each randomizer from a XML subtree.
 *  \param inIter XML iterator to read the mersenne twister randomizer from.
 *  \param ioSystem
 */
void Beagle::RandomizerMulti::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="RandomizerMulti"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <RandomizerMulti> expected!");

	OpenMP::Handle lOpenMP = castHandleT<OpenMP>(ioSystem.getComponent("OpenMP"));
	if(inIter->getChildCount() == 0) return;
	else if(inIter->getChildCount() != lOpenMP->getMaxNumThreads())
		throw Beagle_RunTimeExceptionM("The number of threads and the number of Randomizers are not equal.");
	
	mRandomizers = new Randomizer::Bag(lOpenMP->getMaxNumThreads());
	PACC::XML::ConstIterator lChild = inIter->getFirstChild();
	for(unsigned int i = 0; i < mRandomizers->size(); ++i){
		(*mRandomizers)[i] = new Randomizer;
		(*mRandomizers)[i]->readWithSystem(lChild, ioSystem);
		++lChild;
	}
	Beagle_StackTraceEndM("void RandomizerMulti::readWithSystem(PACC::XML::ConstIterator, System&)");
}

/*!
 *  \brief Write the state of each randomizer into a XML streamer.
 *  \param ioStreamer XML streamer to write the randomizer into.
 *  \param inIndent Whether output should be indented.
 */
void Beagle::RandomizerMulti::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	if(isInitialized()){
		for(unsigned int i = 0; i < mRandomizers->size(); ++i)
			(*mRandomizers)[i]->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM("void RandomizerMulti::writeContent(PACC::XML::Streamer&, bool) const");
}
