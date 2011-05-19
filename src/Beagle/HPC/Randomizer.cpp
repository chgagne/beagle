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
 *  \file   beagle/HPC/Randomizer.cpp
 *  \brief  Source code of class Randomizer.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Felix-Antoine Fortin
 *  $Revision: 1.1 $
 *  $Date: 2007/09/13 13:17:43 $
 */

#include "beagle/HPC.hpp"

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a randomizer.
 *  \param inSeed Random number generator seed, can be overwritten by the registered seed.
 */
HPC::Randomizer::Randomizer(unsigned long inSeed) : Beagle::Randomizer(inSeed)
{ }


/*!
 *  \brief Initialize the HPC Randomizer. 
 *  \param ioSystem Evolutionary system.
 *  This Randomizer initialization is based on the type of the node. 
 *  If the node is a Supervisor, the Randomizer is initialised with
 *  the init function of Beagle::Randomizer. The Randomizer then 
 *  generate a seed for each of its Evolver childs. Those seeds are
 *  then sent via MPI to each Evolver. 
 *  If the node is an Evolver, the node wait to receive its seed from
 *  the supervisor. It then initialise its on generator with this seed.
 *  This method guarantee experiments repetability with only one seed. 
 *  Since the Evolver's seeds are generated with the Supervisor Randomizer,
 *  only one seed is needed to firstly initialised the Supervisor Randomizer.
 */
void HPC::Randomizer::init(System& ioSystem)
{
	Beagle_StackTraceBeginM();

	MPICommunication::Handle lMPI = castHandleT<MPICommunication>(ioSystem.getComponent("MPICommunication"));
	
	if(!lMPI->isInitialized()){
		lMPI->init(ioSystem);
		lMPI->setInitializedFlag(true);
	}
	
	const string& lNodeType = lMPI->getNodeType();
	if(lNodeType == "Supervisor"){
		Beagle::Randomizer::init(ioSystem);
		for(unsigned int i = 0; i < lMPI->getNbrOfRelation("Child"); ++i){
			std::string lSeedStr = uint2str(rollInteger());
			lMPI->send(lSeedStr, "Seed", "Child", i);
		}
	} else if(lNodeType == "Evolver"){
		std::string lSeedStr;
		lMPI->receive(lSeedStr, "Seed", "Parent");
		unsigned long lSeed = str2uint(lSeedStr);
		seed(lSeed);
		mSeed = lSeed;
	
		Beagle_LogBasicM(
		  ioSystem.getLogger(),
		  "randomizer", "Beagle::Randomizer",
		  std::string("Randomizer seed used: ")+uint2str(mSeed)
		);
	}

	Beagle_StackTraceEndM();
}
