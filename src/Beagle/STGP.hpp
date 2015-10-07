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
 *  \file   beagle/GP.hpp
 *  \brief  File to include to import all the GP specific Beagle classes.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.26 $
 *  $Date: 2007/08/10 20:32:21 $
 */

/*!
 *  \namespace Beagle::GP
 *  \brief     Namespace of all the classes of the Open Beagle GP framework.
 *  \ingroup GPF
 */

/*!
 *  \defgroup GPF GP Framework
 *  \brief Specialized framework for genetic programming.
 */

#ifndef Beagle_STGP_hpp
#define Beagle_STGP_hpp


// Beagle generic components

#include "Beagle/Core.hpp"
#include "Beagle/GP.hpp"

// STGP framework

#include "Beagle/STGP/CrossoverConstrainedOp.hpp"
#include "Beagle/STGP/InitFullConstrainedOp.hpp"
#include "Beagle/STGP/InitGrowConstrainedOp.hpp"
#include "Beagle/STGP/InitHalfConstrainedOp.hpp"
#include "Beagle/STGP/MutationInsertConstrainedOp.hpp"
#include "Beagle/STGP/MutationShrinkConstrainedOp.hpp"
#include "Beagle/STGP/MutationStandardConstrainedOp.hpp"
#include "Beagle/STGP/MutationSwapConstrainedOp.hpp"
#include "Beagle/STGP/MutationSwapSubtreeConstrainedOp.hpp"
#include "Beagle/STGP/PackageConstrained.hpp"
#include "Beagle/STGP/ValidateOp.hpp"


#endif // Beagle_GP_hpp

