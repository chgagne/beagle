/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP.hpp
 *  \brief  File to include to import all the linear GP specific Beagle classes.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.3 $
 *  $Date: 2007/08/17 18:09:22 $
 */

/*!
 *  \namespace Beagle::LinGP
 *  \brief Namespace of all the classes of the Open Beagle linear GP framework.
 *  \ingroup LinGPF
 */

/*!
 *  \defgroup LinGPF Linear GP Framework
 *  \brief Specialized framework for linear genetic programming.
 */

#ifndef Beagle_LinGP_hpp
#define Beagle_LinGP_hpp


// Beagle generic components

#include "beagle/Beagle.hpp"

// GA framework

#include "beagle/GA.hpp"

// Linear GP framework

#include "beagle/LinGP/Instruction.hpp"
#include "beagle/LinGP/Context.hpp"
#include "beagle/LinGP/Memory.hpp"
#include "beagle/LinGP/InstructionInline.hpp"
#include "beagle/LinGP/InstructionSet.hpp"
#include "beagle/LinGP/InstructionSuperSet.hpp"
#include "beagle/LinGP/Program.hpp"
#include "beagle/LinGP/EvaluationOp.hpp"
#include "beagle/LinGP/CrossoverOnePointOp.hpp"
#include "beagle/LinGP/CrossoverTwoPointsOp.hpp"
#include "beagle/LinGP/CrossoverUniformOp.hpp"
#include "beagle/LinGP/MutationOp.hpp"
#include "beagle/LinGP/InitializationOp.hpp"
#include "beagle/LinGP/Package.hpp"

#endif // Beagle_LinGP_hpp

