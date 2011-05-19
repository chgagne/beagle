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

#include "beagle/config.hpp"

#include "PACC/Util.hpp"
#include "PACC/XML.hpp"
#include "PACC/Math.hpp"
#include "PACC/Threading.hpp"


// Beagle generic framework
#include "beagle/Beagle.hpp"
// Generic HPC classes
#include "beagle/HPC/PackageBase.hpp"
#include "beagle/HPC/PackageMultiObj.hpp"
#include "beagle/HPC/MPICommunication.hpp"
#include "beagle/HPC/LoggerXMLD.hpp"
#include "beagle/HPC/Randomizer.hpp"
#include "beagle/HPC/AlgoSequential.hpp"
#include "beagle/HPC/AlgoNSGA2.hpp"
#include "beagle/HPC/SwitchTypeOp.hpp"
#include "beagle/HPC/EvaluationOp.hpp"
#include "beagle/HPC/DistributeDemeToEvaluatorsOp.hpp"
#include "beagle/HPC/SendDemeToEvolverOp.hpp"
#include "beagle/HPC/RecvDemeFromEvolverOp.hpp"
#include "beagle/HPC/RecvProcessedFromEvolverOp.hpp"
#include "beagle/HPC/RecvDemeFromSupervisorOp.hpp"
#include "beagle/HPC/RecvFitnessFromEvaluatorOp.hpp"
#include "beagle/HPC/RecvIndividualsFromEvolverOp.hpp"
#include "beagle/HPC/SendDemeToSupervisorOp.hpp"
#include "beagle/HPC/SendProcessedToSupervisorOp.hpp"
#include "beagle/HPC/SendFitnessToEvolverOp.hpp"
#include "beagle/HPC/ProcessingBuffer.hpp"
#include "beagle/HPC/MPIException.hpp"

//#include "beagle/HPC/MigrationSyncOp.hpp"
//#include "beagle/HPC/MigrationRingMonoDemeOp.hpp"
//#include "beagle/HPC/SendLogToParentOp.hpp"
//#include "beagle/HPC/RecvLogFromChildOp.hpp"
#endif // Beagle_HPC_hpp

