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

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"

// GA framework

#include "Beagle/EMO/AlgoNSGA2.hpp"
#include "Beagle/EMO/FitnessMultiObj.hpp"
#include "Beagle/EMO/FitnessMultiObjMin.hpp"
#include "Beagle/EMO/NPGA2Op.hpp"
#include "Beagle/EMO/NSGA2Op.hpp"
#include "Beagle/EMO/PackageMultiObj.hpp"
#include "Beagle/EMO/ParetoFrontCalculateOp.hpp"
#include "Beagle/EMO/ParetoFrontHOF.hpp"
#include "Beagle/EMO/StatsCalcFitnessMultiObjOp.hpp"

#endif // Beagle_GA_hpp

