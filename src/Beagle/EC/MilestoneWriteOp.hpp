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
 *  \file   Beagle/EC/MilestoneWriteOp.hpp
 *  \brief  Definition of the class MilestoneWriteOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_EC_MilestoneWriteOp_hpp
#define Beagle_EC_MilestoneWriteOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class MilestoneWriteOp Beagle/EC/MilestoneWriteOp.hpp "Beagle/EC/MilestoneWriteOp.hpp"
 *  \brief Write a milestone operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class MilestoneWriteOp : public Beagle::Operator
{

public:

	//! MilestoneWriteOp allocator type
	typedef Beagle::AllocatorT<MilestoneWriteOp,Beagle::Operator::Alloc>
	Alloc;
	//! MilestoneWriteOp handle type.
	typedef Beagle::PointerT<MilestoneWriteOp,Beagle::Operator::Handle>
	Handle;
	//! MilestoneWriteOp bag type.
	typedef Beagle::ContainerT<MilestoneWriteOp,Beagle::Operator::Bag>
	Bag;

	explicit MilestoneWriteOp(std::string inName="MilestoneWriteOp");
	virtual ~MilestoneWriteOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void writeMilestone(std::string inFilename, Beagle::Context& ioContext);

private:
	Beagle::String::Handle    mMilestonePrefix;     //!< Prefix of the milestone filenames.
	Beagle::UInt::Handle      mWritingInterval;     //!< Milestone's writing interval (in generation).
	Beagle::Bool::Handle      mPerDemeMilestone;    //!< True: write a milestone for every deme.
	Beagle::Bool::Handle      mOverwriteMilestone;  //!< True: Overwrite file. False: Backup everything.
	Beagle::UIntArray::Handle mPopSize;             //!< Population size for the evolution.

#ifdef BEAGLE_HAVE_LIBZ
	Beagle::Bool::Handle      mCompressMilestone;   //!< True: compress file, false: keep as clear text.
#endif // BEAGLE_HAVE_LIBZ

};

}

}

#endif // Beagle_EC_MilestoneWriteOp_hpp
