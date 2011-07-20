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
 *  \file   Beagle/SAES.hpp
 *  \brief  File to include to import all the SA-ES specific Beagle classes.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.30 $
 *  $Date: 2007/08/08 14:51:33 $
 */

/*!
 *  \namespace Beagle::SAES
 *  \brief     Namespace of all the classes of the self-adaptive evolution strategy framework.
 *  \ingroup SAESF
 */

/*!
 *  \defgroup SAES Anisotropic self-adaptive evolution strategy Framework
 *  \brief Specialized framework for SA-ES.
 *
 *  Long description to be done...
 *
 */

#ifndef Beagle_SAES_hpp
#define Beagle_SAES_hpp


// Beagle generic components

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"

// SA-ES framework
#include "Beagle/SAES/Algorithm.hpp"
#include "Beagle/SAES/InitializationOp.hpp"
#include "Beagle/SAES/InitializationQROp.hpp"
#include "Beagle/SAES/MutationOp.hpp"
#include "Beagle/SAES/MutationQROp.hpp"
#include "Beagle/SAES/Package.hpp"
#include "Beagle/SAES/PairVector.hpp"
#include "Beagle/SAES/RecombinationOp.hpp"
#include "Beagle/SAES/RecombinationWeightedOp.hpp"

#endif // Beagle_SAES_hpp

