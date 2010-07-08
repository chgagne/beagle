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
 *  \file   beagle/Core/Context.cpp
 *  \brief  Source code of class Context.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:45 $
 */

#include "beagle/Core.hpp"

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

