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
 *  \file   Beagle/Core/IOException.cpp
 *  \brief  Implementation of class IOException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15 $
 *  $Date: 2007/08/10 20:32:22 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Construct the IOException using given values.
 *  \param inMessage     Message explaining the exception.
 *  \param inFileName    Name of the file where the error state had been detected.
 *  \param inLineNumber  Line number in the filename.
 */
IOException::IOException(std::string  inMessage,
                         std::string  inFileName,
                         unsigned int inLineNumber) :
		TargetedException(inMessage,inFileName,inLineNumber)
{ }


/*!
 *  \brief Construct the IOException using given values.
 *  \param inIter        XML node in which the exception occured.
 *  \param inMessage     Message explaining the exception.
 *  \param inFileName    Name of the file where the error state had been detected.
 *  \param inLineNumber  Line number in the filename.
 */
IOException::IOException(const PACC::XML::Node& inIter,
                         std::string inMessage,
                         std::string inFileName,
                         unsigned int inLineNumber) :
		TargetedException("",inFileName,inLineNumber)
{
	Beagle_StackTraceBeginM();
	std::ostringstream lOSS;
	lOSS << "Error with XML node '" << inIter.getValue();
	lOSS << "': " << inMessage;
	setMessage(lOSS.str());
	Beagle_StackTraceEndM("IOException::IOException(const PACC::XML::Node& inIter, string inMessage, string inFileName, unsigned int inLineNumber)");
}


/*!
 *  \brief  Get the name of the exception.
 *  \return Name of the exception.
 */
const char* IOException::getExceptionName() const throw()
{
	return "Beagle::IOException";
}

