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
 *  \file   Beagle/Core.hpp
 *  \brief  File to include to import core Beagle classes.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.53 $
 *  $Date: 2007/08/29 17:38:31 $
 */

/*!
 *  \namespace Beagle
 *  \brief     Namespace of Core classes and global functions.
 *  \ingroup   Core
 */

/*!
 *  \mainpage Open BEAGLE Reference Manual
 *
 *  You can use the actual manual in different ways. If you are looking for documentation to get
 *  information on specific element of the Open BEAGLE framework, you can check in the
 *  <A HREF="annotated.html">compound list</A> for a specific class, or the
 *  <A HREF="functions.html">compound member index</A> for a specific method. If you intent to use the
 *  manual in order to get familiar with the framework, start by browsing the
 *  <A HREF="modules.html">modules</A>, especially the examples modules. The different components are
 *  there logically regrouped to facilitate assimilation of the internal framework organization.
 */

/*!
 *  \defgroup Core Core classes and global functions
 *  \brief This is the basic classes and functions of the Open BEAGLE architecture.
 */

#ifndef Beagle_Core_hpp
#define Beagle_Core_hpp

// PACC classes
#include "PACC/Math.hpp"
#include "PACC/Util.hpp"
#include "PACC/XML.hpp"

// Configuration and macros
#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"

// Open MP helper functions
#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
#include "Beagle/Core/OpenMP.hpp"
#endif

// Headers of Open BEAGLE core classes and functions
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/castObjectT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AbstractAllocT.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/NamedObject.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/HashString.hpp"
#include "Beagle/Core/Map.hpp"
#include "Beagle/Core/Matrix.hpp"
#include "Beagle/Core/Vector.hpp"
#include "Beagle/Core/IsEqualMapPairPredicate.hpp"
#include "Beagle/Core/IsEqualPointerPredicate.hpp"
#include "Beagle/Core/IsLessMapPairPredicate.hpp"
#include "Beagle/Core/IsLessPointerPredicate.hpp"
#include "Beagle/Core/IsMorePointerPredicate.hpp"
#include "Beagle/Core/Exception.hpp"
#include "Beagle/Core/AssertException.hpp"
#include "Beagle/Core/BadCastException.hpp"
#include "Beagle/Core/InternalException.hpp"
#include "Beagle/Core/IOException.hpp"
#include "Beagle/Core/ObjectException.hpp"
#include "Beagle/Core/RunTimeException.hpp"
#include "Beagle/Core/TargetedException.hpp"
#include "Beagle/Core/ValidationException.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/ArrayT.hpp"
#include "Beagle/Core/Bool.hpp"
#include "Beagle/Core/Char.hpp"
#include "Beagle/Core/Double.hpp"
#include "Beagle/Core/Float.hpp"
#include "Beagle/Core/Int.hpp"
#include "Beagle/Core/Long.hpp"
#include "Beagle/Core/Short.hpp"
#include "Beagle/Core/String.hpp"
#include "Beagle/Core/UChar.hpp"
#include "Beagle/Core/UInt.hpp"
#include "Beagle/Core/ULong.hpp"
#include "Beagle/Core/UShort.hpp"
#include "Beagle/Core/Member.hpp"
#include "Beagle/Core/MemberMap.hpp"
#include "Beagle/Core/Genotype.hpp"
#include "Beagle/Core/Fitness.hpp"
#include "Beagle/Core/Individual.hpp"
#include "Beagle/Core/IndividualBag.hpp"
#include "Beagle/Core/HallOfFame.hpp"
#include "Beagle/Core/Stats.hpp"
#include "Beagle/Core/Deme.hpp"
#include "Beagle/Core/Context.hpp"
#include "Beagle/Core/Vivarium.hpp"
#include "Beagle/Core/Component.hpp"
#include "Beagle/Core/ComponentMap.hpp"
#include "Beagle/Core/Factory.hpp"
#include "Beagle/Core/System.hpp"
#include "Beagle/Core/QuasiRandom.hpp"
#include "Beagle/Core/Randomizer.hpp"
#include "Beagle/Core/Register.hpp"
#include "Beagle/Core/Logger.hpp"
#include "Beagle/Core/LoggerXML.hpp"
#include "Beagle/Core/DataSetClassification.hpp"
#include "Beagle/Core/DataSetRegression.hpp"
#include "Beagle/Core/RouletteT.hpp"
#include "Beagle/Core/History.hpp"
#include "Beagle/Core/HistoryEntry.hpp"
#include "Beagle/Core/HistoryID.hpp"
#include "Beagle/Core/HistoryTrace.hpp"
#include "Beagle/Core/Operator.hpp"
#include "Beagle/Core/OperatorMap.hpp"
#include "Beagle/Core/BreederNode.hpp"
#include "Beagle/Core/BreederOp.hpp"
#include "Beagle/Core/ReplacementStrategyOp.hpp"
#include "Beagle/Core/EvaluationOp.hpp"
#include "Beagle/Core/Evolver.hpp"
#include "Beagle/Core/Algorithm.hpp"
#include "Beagle/Core/PackageAbstract.hpp"
#include "Beagle/Core/Package.hpp"

#ifdef BEAGLE_HAVE_OPENMP
#include "Beagle/Core/OpenMP.hpp"
#include "Beagle/Core/RandomizerMulti.hpp"
#include <omp.h>
#endif //BEAGLE_HAVE_OPENMP

#endif //Beagle_Core_hpp
