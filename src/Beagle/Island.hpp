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
 *  \file   beagle/Distrib/Island.hpp
 *  \brief  File to include to import all the island-model distribution classes.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 17:02:59 $
 */

/*!
 *  \namespace Beagle::Distrib::Island
 *  \brief     Namespace of all classes for island-model distribution.
 *  \ingroup   DBIM
 */

/*!
 *  \defgroup DBIM Island-model distribued EC
 *  \brief Specialized framework for island-model distributed evolutionary computations.
 */

#ifndef Beagle_Distrib_Island_hpp
#define Beagle_Distrib_Island_hpp

#include "Beagle/config.hpp"

#include "PACC/Util.hpp"
#include "PACC/XML.hpp"
#include "PACC/Math.hpp"
#include "PACC/Threading.hpp"
#ifdef BEAGLE_HAVE_SYS_SOCKET_H
#include "PACC/Socket.hpp"
#endif // BEAGLE_HAVE_SYS_SOCKET_H

// Beagle generic framework
#include "Beagle/Core.hpp"

// Generic island-model classes
#include "Beagle/Island/Communications.hpp"
#include "Beagle/Island/CommunicationsOp.hpp"
#include "Beagle/Island/SendOp.hpp"
#include "Beagle/Island/TermReceiveOp.hpp"
#include "Beagle/Island/TermSendOp.hpp"
#include "Beagle/Island/TransportComponent.hpp"
#include "Beagle/Island/TransportReceiveOp.hpp"
#include "Beagle/Island/TransportSendOp.hpp"
#include "Beagle/Island/Package.hpp"

// TCP/IP island-model classes
#ifdef BEAGLE_HAVE_SYS_SOCKET_H
#include "Beagle/Island/CommunicationsTCPIP.hpp"
#include "Beagle/Island/CommunicationsTCPIPReceiver.hpp"
#include "Beagle/Island/PackageTCPIP.hpp"
#endif // BEAGLE_HAVE_SYS_SOCKET_H

// MPI island-model classes
#ifdef BEAGLE_HAVE_MPI
#include "mpi.h"
#include "Beagle/Island/CommunicationsMPI.hpp"
#include "Beagle/Island/CommunicationsMPIReceiver.hpp"
#include "Beagle/Island/PackageMPI.hpp"
#endif // BEAGLE_HAVE_MPI

#endif // Beagle_Distrib_Island_hpp
