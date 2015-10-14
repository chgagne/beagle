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

#include "Beagle/Core.hpp"

// GA framework

#include "Beagle/EC.hpp"

// Linear GP framework

#include "Beagle/LinGP/Instruction.hpp"
#include "Beagle/LinGP/Context.hpp"
#include "Beagle/LinGP/Memory.hpp"
#include "Beagle/LinGP/InstructionInline.hpp"
#include "Beagle/LinGP/InstructionSet.hpp"
#include "Beagle/LinGP/InstructionSuperSet.hpp"
#include "Beagle/LinGP/Program.hpp"
#include "Beagle/LinGP/EvaluationOp.hpp"
#include "Beagle/LinGP/CrossoverOnePointOp.hpp"
#include "Beagle/LinGP/CrossoverTwoPointsOp.hpp"
#include "Beagle/LinGP/CrossoverUniformOp.hpp"
#include "Beagle/LinGP/MutationOp.hpp"
#include "Beagle/LinGP/InitializationOp.hpp"
#include "Beagle/LinGP/Package.hpp"

#endif // Beagle_LinGP_hpp

