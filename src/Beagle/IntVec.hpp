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
 *  \file   Beagle/IntVec.hpp
 *  \brief  File to include to import all classes specific to the integer vector representation.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.30 $
 *  $Date: 2007/08/08 14:51:33 $
 */

/*!
 *  \namespace Beagle::IntVec
 *  \brief     Namespace of all the classes of the integer vector representation library.
 *  \ingroup IntVecF
 */

/*!
 *  \defgroup IntVecF Integer Vector Representation Framework
 *  \brief Specialized framework for integer vector genotype individuals.
 *
 *  This framework encompass all classes related to the integer vector representation.
 *  This include the usual variety of initialization, crossover, and mutation operators relevant
 *  to linear representation. It also includes the possibility to use the integer vector
 *  genotypes as permutation of indices individuals, where each individual represents a permutation
 *  of values between 0 and n-1, where n is the size of the integer vectors. Several crossover and
 *  mutation operators have been defined to support such a indices permutation genotypes.
 *
 */

#ifndef Beagle_IntVec_hpp
#define Beagle_IntVec_hpp

// Beagle generic components

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"

// IntVec framework

#include "Beagle/IntVec/IntegerVector.hpp"
#include "Beagle/IntVec/CrossoverCycleOp.hpp"
#include "Beagle/IntVec/CrossoverIndicesOp.hpp"
#include "Beagle/IntVec/CrossoverNWOXOp.hpp"
#include "Beagle/IntVec/CrossoverNonAbelOp.hpp"
#include "Beagle/IntVec/CrossoverOnePointOp.hpp"
#include "Beagle/IntVec/CrossoverOrderedOp.hpp"
#include "Beagle/IntVec/CrossoverPMXOp.hpp"
#include "Beagle/IntVec/CrossoverTwoPointsOp.hpp"
#include "Beagle/IntVec/CrossoverUPMXOp.hpp"
#include "Beagle/IntVec/CrossoverUniformOp.hpp"
#include "Beagle/IntVec/InitIndicesOp.hpp"
#include "Beagle/IntVec/InitializationOp.hpp"
#include "Beagle/IntVec/InitializationQROp.hpp"
#include "Beagle/IntVec/MutationMoveSequenceOp.hpp"
#include "Beagle/IntVec/MutationReverseSequenceOp.hpp"
#include "Beagle/IntVec/MutationShuffleOp.hpp"
#include "Beagle/IntVec/MutationShuffleSequenceOp.hpp"
#include "Beagle/IntVec/MutationUniformOp.hpp"
#include "Beagle/IntVec/MutationUniformQROp.hpp"
#include "Beagle/IntVec/Package.hpp"

#endif // Beagle_IntVec_hpp
