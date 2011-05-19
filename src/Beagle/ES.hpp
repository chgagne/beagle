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
 *  \file   beagle/GA.hpp
 *  \brief  File to include to import all the GA specific Beagle classes.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.30 $
 *  $Date: 2007/08/08 14:51:33 $
 */

/*!
 *  \namespace Beagle::GA
 *  \brief     Namespace of all the classes of the Open Beagle GA framework.
 *  \ingroup GAF
 */

/*!
 *  \defgroup GAF GA Framework
 *  \brief Specialized framework for genetic algorithms.
 *
 *  Genetic algorithm framework encompass all the vector-based representations of
 *  fixed and variable length. This actually includes three representations:
 *  classical bitstring GA, real-valued GA and evolution strategy (even if historically
 *  this is a distinct EC flavor). Several generic crossover operators are defined as
 *  template, with class instantiation for the three actual representations. An evolver is
 *  also defined for each representation. The framework is defined to be the futur repository
 *  of linear EC representation, except those related to linear GP, which will belong to a different
 *  framework.
 *
 */

#ifndef Beagle_GA_hpp
#define Beagle_GA_hpp


// Beagle generic components

#include "beagle/Beagle.hpp"

// GA framework

#include "beagle/GA/BitString.hpp"
#include "beagle/GA/FloatVector.hpp"
#include "beagle/GA/ESVector.hpp"
#include "beagle/GA/IntegerVector.hpp"
#include "beagle/GA/CMAHolder.hpp"
#include "beagle/GA/CrossoverBlendESVecOp.hpp"
#include "beagle/GA/CrossoverBlendFltVecOp.hpp"
#include "beagle/GA/CrossoverCycleOp.hpp"
#include "beagle/GA/CrossoverIndicesIntVecOp.hpp"
#include "beagle/GA/CrossoverNonAbelOp.hpp"
#include "beagle/GA/CrossoverNWOXOp.hpp"
#include "beagle/GA/CrossoverOnePointBitStrOp.hpp"
#include "beagle/GA/CrossoverOnePointESVecOp.hpp"
#include "beagle/GA/CrossoverOnePointFltVecOp.hpp"
#include "beagle/GA/CrossoverOnePointIntVecOp.hpp"
#include "beagle/GA/CrossoverOnePointOpT.hpp"
#include "beagle/GA/CrossoverOrderedOp.hpp"
#include "beagle/GA/CrossoverPMXOp.hpp"
#include "beagle/GA/CrossoverSBXFltVecOp.hpp"
#include "beagle/GA/CrossoverTwoPointsBitStrOp.hpp"
#include "beagle/GA/CrossoverTwoPointsESVecOp.hpp"
#include "beagle/GA/CrossoverTwoPointsFltVecOp.hpp"
#include "beagle/GA/CrossoverTwoPointsIntVecOp.hpp"
#include "beagle/GA/CrossoverTwoPointsOpT.hpp"
#include "beagle/GA/CrossoverUniformBitStrOp.hpp"
#include "beagle/GA/CrossoverUniformESVecOp.hpp"
#include "beagle/GA/CrossoverUniformFltVecOp.hpp"
#include "beagle/GA/CrossoverUniformIntVecOp.hpp"
#include "beagle/GA/CrossoverUniformOpT.hpp"
#include "beagle/GA/CrossoverUPMXOp.hpp"
#include "beagle/GA/InitBitStrOp.hpp"
#include "beagle/GA/InitBitStrRampedOp.hpp"
#include "beagle/GA/InitCMAFltVecOp.hpp"
#include "beagle/GA/InitESVecOp.hpp"
#include "beagle/GA/InitFltVecOp.hpp"
#include "beagle/GA/InitIndicesIntVecOp.hpp"
#include "beagle/GA/InitIntVecOp.hpp"
#include "beagle/GA/InitQRESVecOp.hpp"
#include "beagle/GA/InitQRFltVecOp.hpp"
#include "beagle/GA/InitQRIntVecOp.hpp"
#include "beagle/GA/MutationCMAFltVecOp.hpp"
#include "beagle/GA/MutationESVecOp.hpp"
#include "beagle/GA/MutationFlipBitStrOp.hpp"
#include "beagle/GA/MutationGaussianFltVecOp.hpp"
#include "beagle/GA/MutationMoveSequenceOp.hpp"
#include "beagle/GA/MutationShuffleIntVecOp.hpp"
#include "beagle/GA/MutationUniformIntVecOp.hpp"
#include "beagle/GA/MutationQRCMAFltVecOp.hpp"
#include "beagle/GA/MutationQRGaussianFltVecOp.hpp"
#include "beagle/GA/MutationQRESVecOp.hpp"
#include "beagle/GA/MutationQRUniformIntVecOp.hpp"
#include "beagle/GA/MutationReverseSequenceOp.hpp"
#include "beagle/GA/MutationShuffleSequenceOp.hpp"
#include "beagle/GA/RecombinationFltVecOp.hpp"
#include "beagle/GA/RecombinationWeightedFltVecOp.hpp"
#include "beagle/GA/RecombinationESVecOp.hpp"
#include "beagle/GA/RecombinationWeightedESVecOp.hpp"
#include "beagle/GA/MuWCommaLambdaCMAFltVecOp.hpp"
#include "beagle/GA/TermCMAOp.hpp"
#include "beagle/GA/AdaptOneFifthRuleFltVecOp.hpp"
#include "beagle/GA/AlgoCMAES.hpp"
#include "beagle/GA/AlgoESOneFifthRule.hpp"
#include "beagle/GA/AlgoSAESComma.hpp"
#include "beagle/GA/AlgoSAESPlus.hpp"
#include "beagle/GA/PackageBitString.hpp"
#include "beagle/GA/PackageCMAES.hpp"
#include "beagle/GA/PackageES.hpp"
#include "beagle/GA/PackageFloatVector.hpp"
#include "beagle/GA/PackageIntegerVector.hpp"

#endif // Beagle_GA_hpp

