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
