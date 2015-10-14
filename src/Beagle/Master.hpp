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
 *  \file   beagle/Distrib/Master.hpp
 *  \brief  File to include to import all the master-slave distribution classes.
 *  \author Christian Gagne
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 17:02:59 $
 */

/*!
 *  \namespace Beagle::Distrib::Master
 *  \brief     Namespace of all master-slave distribution classes.
 *  \ingroup   DBMS
 */

/*!
 *  \defgroup DBMS Master-slave TCP/IP distributed EC
 *  \brief Specialized framework for master-slave TCP/IP distributed evolutionary computations.
 */

#ifndef Beagle_Distrib_Master_hpp
#define Beagle_Distrib_Master_hpp

#include "Beagle/config.hpp"

#include "PACC/Util.hpp"
#include "PACC/XML.hpp"
#include "PACC/Math.hpp"
#include "PACC/Threading.hpp"
#ifdef BEAGLE_HAVE_SYS_SOCKET_H
#include "PACC/Socket.hpp"
#endif // BEAGLE_HAVE_SYS_SOCKET_H

// DAGS
#ifdef BEAGLE_HAVE_SYS_SOCKET_H
#include "DAGS/Dags.hpp"
#endif // BEAGLE_HAVE_SYS_SOCKET_H

// Beagle frameworks
#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"
// include "Beagle/GP.hpp"

// Beagle::Distrib TCP-IP master-slave
#ifdef BEAGLE_HAVE_SYS_SOCKET_H
#include "Beagle/Master/BootStrapBeginOp.hpp"
#include "Beagle/Master/BootStrapEndOp.hpp"
#include "Beagle/Master/DistribEvalOp.hpp"
#include "Beagle/Master/EnvironmentManager.hpp"
#include "Beagle/Master/EvalClient.hpp"
#include "Beagle/Master/EvolClient.hpp"
#include "Beagle/Master/FetchCruncherDataOp.hpp"
#include "Beagle/Master/MainLoopBeginOp.hpp"
#include "Beagle/Master/MainLoopEndOp.hpp"
#include "Beagle/Master/MilestoneWriteOp.hpp"
#include "Beagle/Master/AlgoCruncher.hpp"
#include "Beagle/Master/AlgoSequential.hpp"
#include "Beagle/Master/Package.hpp"
#endif // BEAGLE_HAVE_SYS_SOCKET_H

#endif // Beagle_Distrib_Master_hpp
