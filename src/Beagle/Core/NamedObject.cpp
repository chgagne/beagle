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
 *  \file   Beagle/Core/NamedObject.cpp
 *  \brief  Source code of class NamedObject.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:46 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Constructor with arguments of a named object.
 *  \param inName Name of the object.
 */
NamedObject::NamedObject(const std::string& inName) :
		mName(inName)
{ }


/*!
 *  \brief Returns the name of the object
 */
const std::string& NamedObject::getName() const
{
	Beagle_StackTraceBeginM();
	return mName;
	Beagle_StackTraceEndM("const string& NamedObject::getName() const");
}


/*!
 *  \brief Sets the name of the object
 *  \param inName Name of the object
 */
void NamedObject::setName(const std::string& inName)
{
	Beagle_StackTraceBeginM();
	mName = inName;
	Beagle_StackTraceEndM("void NamedObject::setName(const string& inName)");
}

