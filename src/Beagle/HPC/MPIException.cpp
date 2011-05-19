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
 *  \file   beagle/HPC/MPIException.cpp
 *  \brief  Implementation of class MPIException.
 *  \author Felix-Antoine Fortin
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: $
 *  $Date: $
 */

#include "beagle/HPC.hpp"

using namespace Beagle;


/*!
 *  \brief Construct the MPIException using given values.
 *  \param inMessage     Message explaining the exception.
 *  \param inFileName    Name of the file where the error state had been detected.
 *  \param inLineNumber  Line number in the filename.
 */
HPC::MPIException::MPIException(std::string  inMessage,
                           std::string  inFileName,
                           unsigned int inLineNumber) :
			   TargetedException(inMessage,inFileName,inLineNumber)
{ }


/*!
 *  \brief  Get the name of the exception.
 *  \return Name of the exception.
 */
const char* HPC::MPIException::getExceptionName() const throw()
{
	return "Beagle::MPIException";
}

