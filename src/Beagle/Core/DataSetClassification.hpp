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
 *  \file   Beagle/Core/DataSetClassification.hpp
 *  \brief  Definition of the type DataSetClassification.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_DataSetClassification_hpp
#define Beagle_Core_DataSetClassification_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Component.hpp"
#include "Beagle/Core/Vector.hpp"
#include <vector>


namespace Beagle
{

/*!
 *  \class DataSetClassification Beagle/Core/DataSetClassification.hpp "Beagle/Core/DataSetClassification.hpp"
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
