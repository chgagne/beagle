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

#ifndef Beagle_GP_hpp
#define Beagle_GP_hpp


// Beagle generic components

#include "beagle/Beagle.hpp"
#include "beagle/GA.hpp"

// GP framework

#include "beagle/GP/ExecutionException.hpp"
#include "beagle/GP/ResourcesExceededExecutionException.hpp"
#include "beagle/GP/MaxNodesExecutionException.hpp"
#include "beagle/GP/MaxTimeExecutionException.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Datum.hpp"
#include "beagle/GP/Individual.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveMap.hpp"
#include "beagle/GP/PrimitiveSet.hpp"
#include "beagle/GP/PrimitiveSuperSet.hpp"
#include "beagle/GP/Tree.hpp"
#include "beagle/GP/PrimitiveInline.hpp"
#include "beagle/GP/FitnessKoza.hpp"
#include "beagle/GP/StatsCalcFitnessSimpleOp.hpp"
#include "beagle/GP/StatsCalcFitnessKozaOp.hpp"
#include "beagle/GP/PrimitiveUsageStatsOp.hpp"
#include "beagle/GP/CrossoverOp.hpp"
#include "beagle/GP/MutationShrinkOp.hpp"
#include "beagle/GP/MutationSwapOp.hpp"
#include "beagle/GP/MutationInsertOp.hpp"
#include "beagle/GP/MutationStandardOp.hpp"
#include "beagle/GP/MutationSwapSubtreeOp.hpp"
#include "beagle/GP/MutationEphemeralOpT.hpp"
#include "beagle/GP/MutationEphemeralDoubleOp.hpp"
#include "beagle/GP/InitializationOp.hpp"
#include "beagle/GP/InitFullOp.hpp"
#include "beagle/GP/InitGrowOp.hpp"
#include "beagle/GP/InitHalfOp.hpp"
#include "beagle/GP/EvaluationOp.hpp"
#include "beagle/GP/TermMaxHitsOp.hpp"
#include "beagle/GP/CrossoverConstrainedOp.hpp"
#include "beagle/GP/MutationShrinkConstrainedOp.hpp"
#include "beagle/GP/MutationSwapConstrainedOp.hpp"
#include "beagle/GP/MutationInsertConstrainedOp.hpp"
#include "beagle/GP/MutationStandardConstrainedOp.hpp"
#include "beagle/GP/MutationSwapSubtreeConstrainedOp.hpp"
#include "beagle/GP/InitFullConstrainedOp.hpp"
#include "beagle/GP/InitGrowConstrainedOp.hpp"
#include "beagle/GP/InitHalfConstrainedOp.hpp"
#include "beagle/GP/ValidateOp.hpp"
#include "beagle/GP/PackageBase.hpp"
#include "beagle/GP/PackageConstrained.hpp"
#include "beagle/GP/ModuleVectorComponent.hpp"
#include "beagle/GP/ModuleCompressOp.hpp"
#include "beagle/GP/ModuleExpandOp.hpp"

// GP Primitives

#include "beagle/GP/TokenT.hpp"
#include "beagle/GP/Invoker.hpp"
#include "beagle/GP/Argument.hpp"
#include "beagle/GP/ArgumentT.hpp"
#include "beagle/GP/ADF.hpp"
#include "beagle/GP/Module.hpp"
#include "beagle/GP/EphemeralT.hpp"
#include "beagle/GP/EphemeralDouble.hpp"
#include "beagle/GP/AddT.hpp"
#include "beagle/GP/SubtractT.hpp"
#include "beagle/GP/MultiplyT.hpp"
#include "beagle/GP/DivideT.hpp"
#include "beagle/GP/Sin.hpp"
#include "beagle/GP/Cos.hpp"
#include "beagle/GP/Exp.hpp"
#include "beagle/GP/Log.hpp"
#include "beagle/GP/And.hpp"
#include "beagle/GP/Or.hpp"
#include "beagle/GP/Xor.hpp"
#include "beagle/GP/Nand.hpp"
#include "beagle/GP/Nor.hpp"
#include "beagle/GP/Not.hpp"
#include "beagle/GP/IfThenElseT.hpp"
#include "beagle/GP/AbsT.hpp"
#include "beagle/GP/BitwiseAnd.hpp"
#include "beagle/GP/BitwiseOr.hpp"
#include "beagle/GP/BitwiseNand.hpp"
#include "beagle/GP/BitwiseNor.hpp"
#include "beagle/GP/BitwiseNot.hpp"
#include "beagle/GP/BitwiseXor.hpp"

#endif // Beagle_GP_hpp

