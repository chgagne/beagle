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
 *  \file   beagle/Coev.hpp
 *  \brief  File to include to import all the co-evolution specific classes.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
 *  $Date: 2007/08/08 19:26:32 $
 */

/*!
 *  \defgroup COEVF Co-evolution Framework
 *  \brief Specialized framework for co-evolution.
 *
 *  Co-evolution framework is based on multi-threading programming, where each thread is associated to
 *  a population. The execution sequence of each thread is the same than what usually done in the
 *  main function of standard evolutions (i.e. build system, evaluation operator, vivarium, evolver,
 *  initialize evolver, and start evolution). The population in each thread evolves independently, 
 *  with inter-thread synchronization only in co-evolutionary fitness evaluation operator. This 
 *  operator behaves quite differently than usual mono-population evaluation operator. The
 *  co-evolution evaluation procedure starts by calling Coev::EvaluationOp::makeSets, which makes
 *  evaluation sets of the evolving population and add them into shared storage structure, using
 *  method Coev::EvaluationOp::addSet. When the desired number of evaluation sets is added  (the
 *  trigger value) the co-evolutionary fitness evaluation method defined in
 *  Coev::EvaluationOp::evaluateSets is called. Pure virtual methods Coev::EvaluationOp::makeSets 
 *  and Coev::EvaluationOp::evaluateSets are problem-specific and must be defined by the user in 
 *  its co-evolutionary fitness evaluation operators.
 *
 *  A trigger value is used to specify the number of evaluation sets needed to start a
 *  co-evolutionary evaluation. This value is usually equal to the number of threads/populations used,
 *  as usually each thread/population add one evaluation set before doing the co-evolutionary
 *  evaluation operation. But different trigger value can be used depending on the context.
 *
 *  PACC's C++ classes for multi-threading (defined in namespace PACC::Threading)
 *  are used by the co-evolution framework. Users are advised to use them for their
 *  co-evolutionary applications.
 *
 */

/*!
 *  \namespace Beagle::Coev
 *  \brief     Namespace of the co-evolution classes.
 *  \ingroup COEVF
 */

#ifndef Beagle_Coev_hpp
#define Beagle_Coev_hpp


// Beagle generic components

#include "beagle/Beagle.hpp"
#include "beagle/GA.hpp"
#include "beagle/GP.hpp"

// Mutli-threading classes

#include "PACC/Threading.hpp"

// Co-evolution classes

#include "beagle/Coev/EvaluationOp.hpp"
#include "beagle/Coev/GPEvaluationOp.hpp"
#include "beagle/Coev/Package.hpp"
#include "beagle/Coev/TermBroadcastOp.hpp"

#endif // Beagle_Coev_hpp

