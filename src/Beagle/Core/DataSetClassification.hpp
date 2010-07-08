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
 *  \file   beagle/Core/DataSetClassification.hpp
 *  \brief  Definition of the type DataSetClassification.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_DataSetClassification_hpp
#define Beagle_Core_DataSetClassification_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Component.hpp"
#include "beagle/Core/Vector.hpp"
#include <vector>


namespace Beagle {

/*!
 *  \class DataSetClassification beagle/Core/DataSetClassification.hpp "beagle/Core/DataSetClassification.hpp"
 *  \brief Component of a data set useful for classification problems.
 *  \ingroup Core
 */
class DataSetClassification : public Component, public std::vector< std::pair<int,Beagle::Vector> >
{

public:

	//! DataSetClassification allocator type.
	typedef AllocatorT< DataSetClassification, Component::Alloc > Alloc;
	//!< DataSetClassification handle type.
	typedef PointerT< DataSetClassification, Component::Handle > Handle;
	//!< DataSetClassification bag type.
	typedef ContainerT< DataSetClassification, Component::Bag > Bag;

	explicit DataSetClassification(const std::string& inName=std::string("DataSetClassification"));
	virtual ~DataSetClassification()
	{ }

	        void readCSV(std::istream& ioIS);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}

#endif // Beagle_Core_DataSetClassification_hpp
