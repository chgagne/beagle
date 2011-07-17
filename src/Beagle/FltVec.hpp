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
 *  \file   Beagle/FltVec.hpp
 *  \brief  File to include to import all the float vector specific classes.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.30 $
 *  $Date: 2007/08/08 14:51:33 $
 */

/*!
 *  \namespace Beagle::FltVec
 *  \brief     Namespace of all the classes of the Open BEAGLE float vector representation framework.
 *  \ingroup   FltVecF
 */

/*!
 *  \defgroup FltVecF Float Vector Representation Framework
 *  \brief Specialized framework for float vector representation.
 *
 *  Long description to be done...
 *
 */

#ifndef Beagle_FltVec_hpp
#define Beagle_FltVec_hpp

// Basic frameworks
#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"

// Float vector framework
#include "Beagle/FltVec/CrossoverBlendOp.hpp"
#include "Beagle/FltVec/CrossoverOnePointOp.hpp"
#include "Beagle/FltVec/CrossoverSBXOp.hpp"
#include "Beagle/FltVec/CrossoverTwoPointsOp.hpp"
#include "Beagle/FltVec/CrossoverUniformOp.hpp"
#include "Beagle/FltVec/FloatVector.hpp"
#include "Beagle/FltVec/InitGaussianOp.hpp"
#include "Beagle/FltVec/InitGaussianQROp.hpp"
#include "Beagle/FltVec/InitUniformOp.hpp"
#include "Beagle/FltVec/InitUniformQROp.hpp"
#include "Beagle/FltVec/MutationGaussianOp.hpp"
#include "Beagle/FltVec/MutationGaussianQROp.hpp"
#include "Beagle/FltVec/Package.hpp"

#endif // Beagle_FltVec_hpp

