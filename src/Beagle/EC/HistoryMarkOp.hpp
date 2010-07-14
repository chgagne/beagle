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
 *  \file   Beagle/EC/HistoryMarkOp.hpp
 *  \brief  Definition of the class HistoryMarkOp.
 *  \author Alexandre Devert
 *  $Revision: 1.6 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#ifndef Beagle_EC_HistoryMarkOp_hpp
#define Beagle_EC_HistoryMarkOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class HistoryMarkOp Beagle/EC/HistoryMarkOp.hpp "Beagle/EC/HistoryMarkOp.hpp"
 *  \brief Flush history when writing a milestone operator class.
 *  \ingroup EC
 *  \ingroup Op
 *  \ingroup History
 */
class HistoryMarkOp : public Beagle::Operator
{

public:

	//! HistoryMarkOp allocator type
	typedef Beagle::AllocatorT<HistoryMarkOp,Beagle::Operator::Alloc> Alloc;
	//! HistoryMarkOp handle type.
	typedef Beagle::PointerT<HistoryMarkOp,Beagle::Operator::Handle> Handle;
	//! HistoryMarkOp bag type.
	typedef Beagle::ContainerT<HistoryMarkOp,Beagle::Operator::Bag> Bag;

	explicit HistoryMarkOp(std::string inName="HistoryMarkOp");
	virtual ~HistoryMarkOp()
	{ }

	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

protected:

	virtual void mark(Beagle::Individual::Bag& ioIndividuals, Beagle::Context& ioContext);

};

}

}

#endif // Beagle_EC_HistoryMarkOp_hpp
