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
 *  \file   Beagle/Core/TargetedException.cpp
 *  \brief  Definition of class TargetedException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/10 20:32:22 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new TargetedException using data given.
 *  \param inMessage     Message related to the exception.
 *  \param inFileName    File name where the exception is detected (throwed).
 *  \param inLineNumber  Line number where the exception is detected (throwed).
 */
TargetedException::TargetedException(std::string inMessage,
                                     std::string inFileName,
                                     unsigned int inLineNumber) :
		Exception(inMessage),
		mFileName(inFileName),
		mLineNumber(inLineNumber)
{ }


/*!
 *  \brief Explain the exception throwed in the stream given.
 *  \param ioES Output C++ stream where the explanation about the exception is inserted.
 */
void TargetedException::explain(std::ostream& ioES) throw()
{
	ioES << "From file '" << mFileName << "', line " << mLineNumber << " :";
	ioES << std::endl << std::flush;
	Exception::explain(ioES);
}
