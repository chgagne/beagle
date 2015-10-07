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

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"

// GP framework

#include "Beagle/GP/ExecutionException.hpp"
#include "Beagle/GP/ResourcesExceededExecutionException.hpp"
#include "Beagle/GP/MaxNodesExecutionException.hpp"
#include "Beagle/GP/MaxTimeExecutionException.hpp"
#include "Beagle/GP/Context.hpp"
#include "Beagle/GP/Datum.hpp"
#include "Beagle/GP/Individual.hpp"
#include "Beagle/GP/Primitive.hpp"
#include "Beagle/GP/PrimitiveMap.hpp"
#include "Beagle/GP/PrimitiveSet.hpp"
#include "Beagle/GP/PrimitiveSuperSet.hpp"
#include "Beagle/GP/Tree.hpp"
#include "Beagle/GP/PrimitiveInline.hpp"
#include "Beagle/GP/FitnessKoza.hpp"
#include "Beagle/GP/StatsCalcFitnessSimpleOp.hpp"
#include "Beagle/GP/StatsCalcFitnessKozaOp.hpp"
#include "Beagle/GP/PrimitiveUsageStatsOp.hpp"
#include "Beagle/GP/CrossoverOp.hpp"
#include "Beagle/GP/MutationShrinkOp.hpp"
#include "Beagle/GP/MutationSwapOp.hpp"
#include "Beagle/GP/MutationInsertOp.hpp"
#include "Beagle/GP/MutationStandardOp.hpp"
#include "Beagle/GP/MutationSwapSubtreeOp.hpp"
#include "Beagle/GP/MutationEphemeralOpT.hpp"
#include "Beagle/GP/MutationEphemeralDoubleOp.hpp"
#include "Beagle/GP/InitializationOp.hpp"
#include "Beagle/GP/InitFullOp.hpp"
#include "Beagle/GP/InitGrowOp.hpp"
#include "Beagle/GP/InitHalfOp.hpp"
#include "Beagle/GP/EvaluationOp.hpp"
#include "Beagle/GP/TermMaxHitsOp.hpp"
#include "Beagle/GP/PackageBase.hpp"
#include "Beagle/GP/ModuleVectorComponent.hpp"
#include "Beagle/GP/ModuleCompressOp.hpp"
#include "Beagle/GP/ModuleExpandOp.hpp"

// GP Primitives

#include "Beagle/GP/TokenT.hpp"
#include "Beagle/GP/Invoker.hpp"
#include "Beagle/GP/Argument.hpp"
#include "Beagle/GP/ArgumentT.hpp"
#include "Beagle/GP/ADF.hpp"
#include "Beagle/GP/Module.hpp"
#include "Beagle/GP/EphemeralT.hpp"
#include "Beagle/GP/EphemeralDouble.hpp"
#include "Beagle/GP/AddT.hpp"
#include "Beagle/GP/SubtractT.hpp"
#include "Beagle/GP/MultiplyT.hpp"
#include "Beagle/GP/DivideT.hpp"
#include "Beagle/GP/Sin.hpp"
#include "Beagle/GP/Cos.hpp"
#include "Beagle/GP/Exp.hpp"
#include "Beagle/GP/Log.hpp"
#include "Beagle/GP/And.hpp"
#include "Beagle/GP/Or.hpp"
#include "Beagle/GP/Xor.hpp"
#include "Beagle/GP/Nand.hpp"
#include "Beagle/GP/Nor.hpp"
#include "Beagle/GP/Not.hpp"
#include "Beagle/GP/IfThenElseT.hpp"
#include "Beagle/GP/AbsT.hpp"
#include "Beagle/GP/BitwiseAnd.hpp"
#include "Beagle/GP/BitwiseOr.hpp"
#include "Beagle/GP/BitwiseXor.hpp"
#include "Beagle/GP/BitwiseNand.hpp"
#include "Beagle/GP/BitwiseNor.hpp"
#include "Beagle/GP/BitwiseNot.hpp"

#endif // Beagle_GP_hpp

