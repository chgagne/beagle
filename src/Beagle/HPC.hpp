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
 *  \file   beagle/HPC.hpp
 *  \brief  File to import all the HPC classes.
 *  \author Felix-Antoine Fortin
 *  \author Maxime Girardin
 *  \author Yannick Morin-Rivest
 *  \author Mathieu Couillard
 *  $Revision: 1.16 $
 *  $Date: 2007/08/14 01:41:37 $
 */

/*!
 *  \namespace Beagle::HPC
 *  \brief     Namespace of all HPC classes
 *  \defgroup  HPC High Performance Computing Framework
 */

#ifndef Beagle_HPC_hpp
#define Beagle_HPC_hpp

#include "Beagle/config.hpp"

#include "PACC/Util.hpp"
#include "PACC/XML.hpp"
#include "PACC/Math.hpp"
#include "PACC/Threading.hpp"


// Beagle generic framework
#include "Beagle/Core.hpp"
// Generic HPC classes
#include "Beagle/HPC/PackageBase.hpp"
#include "Beagle/HPC/PackageMultiObj.hpp"
#include "Beagle/HPC/MPICommunication.hpp"
#include "Beagle/HPC/LoggerXMLD.hpp"
#include "Beagle/HPC/Randomizer.hpp"
#include "Beagle/HPC/AlgoSequential.hpp"
#include "Beagle/HPC/AlgoNSGA2.hpp"
#include "Beagle/HPC/SwitchTypeOp.hpp"
#include "Beagle/HPC/EvaluationOp.hpp"
#include "Beagle/HPC/DistributeDemeToEvaluatorsOp.hpp"
#include "Beagle/HPC/SendDemeToEvolverOp.hpp"
#include "Beagle/HPC/RecvDemeFromEvolverOp.hpp"
#include "Beagle/HPC/RecvProcessedFromEvolverOp.hpp"
#include "Beagle/HPC/RecvDemeFromSupervisorOp.hpp"
#include "Beagle/HPC/RecvFitnessFromEvaluatorOp.hpp"
#include "Beagle/HPC/RecvIndividualsFromEvolverOp.hpp"
#include "Beagle/HPC/SendDemeToSupervisorOp.hpp"
#include "Beagle/HPC/SendProcessedToSupervisorOp.hpp"
#include "Beagle/HPC/SendFitnessToEvolverOp.hpp"
#include "Beagle/HPC/ProcessingBuffer.hpp"
#include "Beagle/HPC/MPIException.hpp"

//#include "Beagle/HPC/MigrationSyncOp.hpp"
//#include "Beagle/HPC/MigrationRingMonoDemeOp.hpp"
//#include "Beagle/HPC/SendLogToParentOp.hpp"
//#include "Beagle/HPC/RecvLogFromChildOp.hpp"
#endif // Beagle_HPC_hpp

