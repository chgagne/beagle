/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
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
 *  \file   beagle/Core/DataSetRegression.hpp
 *  \brief  Definition of the type DataSetRegression.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_DataSetRegression_hpp
#define Beagle_Core_DataSetRegression_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Vector.hpp"
#include "beagle/Core/Component.hpp"
#include <vector>


namespace Beagle {

/*!
 *  \class DataSetRegression beagle/Core/DataSetRegression.hpp "beagle/Core/DataSetRegression.hpp"
 *  \brief Component of a data set useful for regression problems.
 *  \ingroup Core
 */
class DataSetRegression : public Component, public std::vector< std::pair<double,Beagle::Vector> >
{

public:

	//! DataSetRegression allocator type.
	typedef AllocatorT< DataSetRegression, Component::Alloc > Alloc;
	//!< DataSetRegression handle type.
	typedef PointerT< DataSetRegression, Component::Handle > Handle;
	//!< DataSetRegression bag type.
	typedef ContainerT< DataSetRegression, Component::Bag > Bag;

	explicit DataSetRegression(const std::string& inName=std::string("DataSetRegression"));
	virtual ~DataSetRegression()
	{ }

	        void readCSV(std::istream& ioIS);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}

#endif // Beagle_Core_DataSetRegression_hpp
