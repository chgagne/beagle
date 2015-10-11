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
 *  \file   beagle/GP/src/MaxTimeExecutionException.cpp
 *  \brief  Implementation of class GP::MaxTimeExecutionException.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  $Revision: 1.6 $
 *  $Date: 2007/08/08 19:26:40 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new MaxTimeExecutionException object.
 *  \param inMessage Message describing the exception.
 *  \param inTimeElapsed Time elapsed in individual evaluation before the exception is thrown.
 *  \param inMaxTimeAllowed Maximum time allowed for individual execution.
 */
GP::MaxTimeExecutionException::MaxTimeExecutionException(std::string inMessage,
        double inTimeElapsed,
        double inMaxTimeAllowed) :
		GP::ResourcesExceededExecutionException(inMessage),
		mTimeElapsed(inTimeElapsed),
		mMaxTimeAllowed(inMaxTimeAllowed)
{ }


/*!
 *  \brief  Get the name of the exception.
 *  \return Name of the exception.
 */
const char* GP::MaxTimeExecutionException::getExceptionName() const throw()
{
	return "Beagle::GP::MaxTimeExecutionException";
}

