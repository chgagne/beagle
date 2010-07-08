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
 *  \file   Beagle/Core/DataSetRegression.hpp
 *  \brief  Definition of the type DataSetRegression.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_DataSetRegression_hpp
#define Beagle_Core_DataSetRegression_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Vector.hpp"
#include "Beagle/Core/Component.hpp"
#include <vector>


namespace Beagle
{

/*!
 *  \class DataSetRegression Beagle/Core/DataSetRegression.hpp "Beagle/Core/DataSetRegression.hpp"
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
