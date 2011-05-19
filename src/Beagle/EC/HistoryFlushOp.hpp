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
 *  \file   Beagle/EC/HistoryFlushOp.hpp
 *  \brief  Definition of the class Beagle::EC::HistoryFlushOp.
 *  \author Alexandre Devert
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#ifndef Beagle_EC_HistoryFlushOp_hpp
#define Beagle_EC_HistoryFlushOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class HistoryFlushOp Beagle/EC/HistoryFlushOp.hpp "Beagle/EC/HistoryFlushOp.hpp"
 *  \brief Flush history when writing a milestone operator class.
 *  \ingroup EC
 *  \ingroup Op
 *  \ingroup History
 */
class HistoryFlushOp : public Beagle::Operator
{

public:

	//! HistoryFlushOp allocator type
	typedef Beagle::AllocatorT<HistoryFlushOp,Beagle::Operator::Alloc> Alloc;
	//! HistoryFlushOp handle type.
	typedef Beagle::PointerT<HistoryFlushOp,Beagle::Operator::Handle> Handle;
	//! HistoryFlushOp bag type.
	typedef Beagle::ContainerT<HistoryFlushOp,Beagle::Operator::Bag> Bag;

	explicit HistoryFlushOp(std::string inName="HistoryFlushOp");
	virtual ~HistoryFlushOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

private:

	Beagle::String::Handle     mMilestonePrefix;     //!< Prefix of the milestone filenames.
	Beagle::UInt::Handle       mWritingInterval;     //!< Milestone's writing interval (in generation).
	Beagle::Bool::Handle       mPerDemeMilestone;    //!< True: write a milestone for every deme.
	Beagle::UIntArray::Handle  mPopSize;             //!< Population size for the evolution.

};

}

}

#endif // Beagle_EC_HistoryFlushOp_hpp
