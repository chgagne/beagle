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
 *  \file   Beagle/BitStr.hpp
 *  \brief  File to include to import all the bit string representation classes.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.30 $
 *  $Date: 2007/08/08 14:51:33 $
 */

/*!
 *  \namespace Beagle::BitStr
 *  \brief     Namespace of all the classes of the Open BEAGLE bit string representation framework.
 *  \ingroup BitStrF
 */

/*!
 *  \defgroup BitStrF Bit String Representation Framework
 *  \brief Specialized framework for bit string representation.
 *
 *  Long description to be done...
 *
 */

#ifndef Beagle_BitStr_hpp
#define Beagle_BitStr_hpp

// Basic frameworks
#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"

// BitStr framework
#include "Beagle/BitStr/BitString.hpp"
#include "Beagle/BitStr/CrossoverOnePointOp.hpp"
#include "Beagle/BitStr/CrossoverTwoPointsOp.hpp"
#include "Beagle/BitStr/CrossoverUniformOp.hpp"
#include "Beagle/BitStr/InitRampedOp.hpp"
#include "Beagle/BitStr/InitUniformOp.hpp"
#include "Beagle/BitStr/MutationFlipBitOp.hpp"
#include "Beagle/BitStr/Package.hpp"

#endif // Beagle_BitStr_hpp
