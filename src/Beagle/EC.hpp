/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/EC/.hpp
 *  \brief  File to include to import generic EC library classes.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.53 $
 *  $Date: 2007/08/29 17:38:31 $
 */

/*!
 *  \namespace Beagle::EC
 *  \brief     Classes of the generic EC library.
 *  \ingroup EC
 */

/*!
 *  \defgroup EC Generic EC library
 *  \brief Open BEAGLE generic evolutionary computation library.
 *
 *  The EC library include set of general representation-independent operators and structures for evolutionary
 *  computation.
 *
 */


#ifndef Beagle_EC_hpp
#define Beagle_EC_hpp

// Include Core library
#include "Beagle/Core.hpp"

// Generic EC library headers
#include "Beagle/EC/AdaptBreedingProbaOp.hpp"
#include "Beagle/EC/AlgoGenerational.hpp"
#include "Beagle/EC/AlgoSequential.hpp"
#include "Beagle/EC/AlgoSteadyState.hpp"
#include "Beagle/EC/CrossoverOp.hpp"
#include "Beagle/EC/CrossoverOnePointOpT.hpp"
#include "Beagle/EC/CrossoverTwoPointsOpT.hpp"
#include "Beagle/EC/CrossoverUniformOpT.hpp"
#include "Beagle/EC/DecimateOp.hpp"
#include "Beagle/EC/EvaluationMultipleOp.hpp"
#include "Beagle/EC/FitnessSimple.hpp"
#include "Beagle/EC/FitnessSimpleMin.hpp"
#include "Beagle/EC/GenerationalOp.hpp"
#include "Beagle/EC/HierarchicalFairCompetitionOp.hpp"
#include "Beagle/EC/HistoryFlushOp.hpp"
#include "Beagle/EC/HistoryMarkOp.hpp"
#include "Beagle/EC/IfThenElseOp.hpp"
#include "Beagle/EC/IndividualSizeFrequencyStatsOp.hpp"
#include "Beagle/EC/InitializationOp.hpp"
#include "Beagle/EC/InvalidateFitnessOp.hpp"
#include "Beagle/EC/MigrationBuffer.hpp"
#include "Beagle/EC/MigrationOp.hpp"
#include "Beagle/EC/MigrationGridOp.hpp"
#include "Beagle/EC/MigrationMapOp.hpp"
#include "Beagle/EC/MigrationRingOp.hpp"
#include "Beagle/EC/MilestoneWriteOp.hpp"
#include "Beagle/EC/MuCommaLambdaOp.hpp"
#include "Beagle/EC/MuPlusLambdaOp.hpp"
#include "Beagle/EC/MutationOp.hpp"
#include "Beagle/EC/OneCommaLambdaAdaptedOp.hpp"
#include "Beagle/EC/OversizeOp.hpp"
#include "Beagle/EC/Package.hpp"
#include "Beagle/EC/RandomShuffleDemeOp.hpp"
#include "Beagle/EC/RecombinationOp.hpp"
#include "Beagle/EC/RegisterReadOp.hpp"
#include "Beagle/EC/SelectionOp.hpp"
#include "Beagle/EC/SelectBestOp.hpp"
#include "Beagle/EC/SelectFirstOp.hpp"
#include "Beagle/EC/SelectParsimonyTournOp.hpp"
#include "Beagle/EC/SelectRandomOp.hpp"
#include "Beagle/EC/SelectRouletteOp.hpp"
#include "Beagle/EC/SelectTournamentOp.hpp"
#include "Beagle/EC/SelectWorstOp.hpp"
#include "Beagle/EC/SelectWorstTournOp.hpp"
#include "Beagle/EC/StatsCalculateOp.hpp"
#include "Beagle/EC/StatsCalcFitnessSimpleOp.hpp"
#include "Beagle/EC/SteadyStateOp.hpp"
#include "Beagle/EC/TerminationOp.hpp"
#include "Beagle/EC/TermMaxEvalsOp.hpp"
#include "Beagle/EC/TermMaxFitnessOp.hpp"
#include "Beagle/EC/TermMaxGenOp.hpp"
#include "Beagle/EC/TermMinFitnessOp.hpp"

#endif // Beagle_EC_hpp
