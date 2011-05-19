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
 *  \file   Beagle/Core/Individual.hpp
 *  \brief  Definition of the class Individual.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.21 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#ifndef Beagle_Core_Individual_hpp
#define Beagle_Core_Individual_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Genotype.hpp"
#include "Beagle/Core/Fitness.hpp"
#include "Beagle/Core/Member.hpp"
#include "Beagle/Core/MemberMap.hpp"


namespace Beagle
{

// Forward declaration
class IndividualBag;
class System;

/*!
 *  \class Individual Beagle/Core/Individual.hpp "Beagle/Core/Individual.hpp"
 *  \brief An individual inherits from a container of genotypes and has a fitness member.
 *  \ingroup Core
 *  \ingroup Pop
 */
class Individual : public Genotype::Bag
{

public:

	//! Individual allocator type.
	typedef AllocatorT<Individual,Genotype::Bag::Alloc> Alloc;
	//! Individual handle type.
	typedef PointerT<Individual,Genotype::Bag::Handle> Handle;
	//! Individual bag type.
	typedef IndividualBag Bag;

	explicit Individual(unsigned int inN=0, Genotype::Handle inModel=NULL);
	virtual ~Individual()
	{ }

	virtual void                addMember(Member::Handle inMember);
	virtual void                copy(const Individual& inOriginal, System& ioSystem);
	virtual double              getDistanceGenotype(const Individual& inRightIndividual) const;
	virtual double              getDistancePhenotype(const Individual& inRightIndividual) const;
	virtual const std::string&  getName() const;
	virtual unsigned int        getSize() const;
	virtual const std::string&  getType() const;
	virtual bool                isEqual(const Object& inRightObj) const;
	virtual bool                isIdentical(const Individual& inRightIndividual) const;
	virtual bool                isLess(const Object& inRightObj) const;
	virtual bool                readFromFile(std::string inFileName, System& ioSystem);
	virtual void                readFitness(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                readGenotypes(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                readMembers(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual Member::Handle      removeMember(const std::string& inName);
	virtual void                write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void                writeFitness(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void                writeGenotypes(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void                writeMembers(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Get the fitness value of the individual.
	 *  \return Fitness value.
	 */
	inline const Fitness::Handle getFitness() const {
		Beagle_StackTraceBeginM();
		return mFitness;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get the fitness value of the individual.
	 *  \return Fitness value.
	 */
	inline Fitness::Handle getFitness() {
		Beagle_StackTraceBeginM();
		return mFitness;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get a member by its name.
	 *  \param inName Name of the member to obtain.
	 *  \return Reference to the member, a NULL pointer if there is no member with given name.
	 */
	inline const Member::Handle getMember(const std::string& inName) const {
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find(inName);
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Member>(lIterMap->second);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get a member by its name.
	 *  \param inName Name of the member to obtain.
	 *  \return Reference to the member, a NULL pointer if there is no member with given name.
	 */
	inline Member::Handle getMember(const std::string& inName) {
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find(inName);
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Member>(lIterMap->second);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the fitness value of the individual.
	 *  \param inFitness Set the fitness value.
	 */
	inline void setFitness(Fitness::Handle inFitness) {
		Beagle_StackTraceBeginM();
		mFitness = inFitness;
		Beagle_StackTraceEndM();
	}

protected:

	MemberMap        mMemberMap;     //!< Members of the individual.
	Fitness::Handle  mFitness;       //!< Fitness value.

public:

	virtual void read(PACC::XML::ConstIterator inIter);   // Undefined!

};

}

#endif // Beagle_Core_Individual_hpp
