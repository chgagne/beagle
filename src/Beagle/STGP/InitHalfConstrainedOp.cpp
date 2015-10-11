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
 *  \file   beagle/GP/src/InitHalfConstrainedOp.cpp
 *  \brief  Source code of class GP::InitHalfConstrainedOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/08/08 19:26:40 $
 */

#include "beagle/GP.hpp"

#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct a constrained GP tree "half-and-half" initialization operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register. 
 *  \param inName Name of the operator.
 */
GP::InitHalfConstrainedOp::InitHalfConstrainedOp(std::string inReproProbaName,
        std::string inName) :
		Beagle::GP::InitializationOp(inReproProbaName, inName),
		mInitFullOp(inReproProbaName),
		mInitGrowOp(inReproProbaName)
{ }


/*!
 *  \brief Register the parameters of the constrained GP tree "half-and-half" intialization operator.
 *  \param ioSystem System of the evolution.
 */
void GP::InitHalfConstrainedOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle::GP::InitializationOp::registerParams(ioSystem);
	mInitFullOp.registerParams(ioSystem);
	mInitGrowOp.registerParams(ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Initialize a tree.
 *  \param outTree Tree to initialize.
 *  \param inMinDepth Minimum depth to make tree.
 *  \param inMaxDepth Maximum depth to make tree.
 *  \param ioContext Evolution context.
 */
unsigned int GP::InitHalfConstrainedOp::initTree(GP::Tree &outTree,
        unsigned int inMinDepth,
        unsigned int inMaxDepth,
        GP::Context &ioContext) const
{
	Beagle_StackTraceBeginM();
	if(ioContext.getSystem().getRandomizer().rollUniform(0.0, 1.0) < 0.5)
		return mInitFullOp.initTree(outTree, inMinDepth, inMaxDepth, ioContext);
	return mInitGrowOp.initTree(outTree, inMinDepth, inMaxDepth, ioContext);
	Beagle_StackTraceEndM();
}
