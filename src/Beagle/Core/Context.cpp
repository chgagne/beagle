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
 *  \file   Beagle/Core/Context.cpp
 *  \brief  Source code of class Context.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a context.
 */
Context::Context() :
	mSystemHandle(NULL),
	mEvolverHandle(NULL),
	mVivariumHandle(NULL),
	mDemeHandle(NULL),
	mDemeIndex(0),
	mIndividualHandle(NULL),
	mIndividualIndex(0),
	mGenotypeHandle(NULL),
	mGenotypeIndex(0),
	mGeneration(0),
	mSelectionIndex(0),
	mProcessedDeme(0),
	mTotalProcDeme(0),
	mProcessedViva(0),
	mTotalProcViva(0),
	mContinueFlag(true),
	mSuccessfulTermination(false)
{ }

