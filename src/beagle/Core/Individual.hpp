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
 *  \file   beagle/Core/Individual.hpp
 *  \brief  Definition of the class Individual.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.21 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#ifndef Beagle_Core_Individual_hpp
#define Beagle_Core_Individual_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/Pointer.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/Allocator.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/Container.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/Genotype.hpp"
#include "beagle/Core/Fitness.hpp"
#include "beagle/Core/Member.hpp"
#include "beagle/Core/MemberMap.hpp"


namespace Beagle
{

// Forward declaration
class IndividualBag;
class System;

/*!
 *  \class Individual beagle/Core/Individual.hpp "beagle/Core/Individual.hpp"
 *  \brief An individual inherits from a container of genotypes and has a fitness member.
 *  \ingroup ECF
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
	inline const Fitness::Handle getFitness() const
	{
		Beagle_StackTraceBeginM();
		return mFitness;
		Beagle_StackTraceEndM("const Fitness::Handle Individual::getFitness() const");
	}

	/*!
	 *  \brief Get the fitness value of the individual.
	 *  \return Fitness value.
	 */
	inline Fitness::Handle getFitness()
	{
		Beagle_StackTraceBeginM();
		return mFitness;
		Beagle_StackTraceEndM("Fitness::Handle Individual::getFitness()");
	}

	/*!
	 *  \brief Get a member by its name.
	 *  \param inName Name of the member to obtain.
	 *  \return Reference to the member, a NULL pointer if there is no member with given name.
	 */
	inline const Member::Handle getMember(const std::string& inName) const
	{
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find(inName);
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Member>(lIterMap->second);
		Beagle_StackTraceEndM("const Member::Handle Individual::getMember(const std::string&) const");
	}

	/*!
	 *  \brief Get a member by its name.
	 *  \param inName Name of the member to obtain.
	 *  \return Reference to the member, a NULL pointer if there is no member with given name.
	 */
	inline Member::Handle getMember(const std::string& inName)
	{
		Beagle_StackTraceBeginM();
		MemberMap::const_iterator lIterMap = mMemberMap.find(inName);
		return (lIterMap == mMemberMap.end()) ? NULL : castHandleT<Member>(lIterMap->second);
		Beagle_StackTraceEndM("Member::Handle Individual::getMember(const std::string&)");
	}

	/*!
	 *  \brief Set the fitness value of the individual.
	 *  \param inFitness Set the fitness value.
	 */
	inline void setFitness(Fitness::Handle inFitness)
	{
		Beagle_StackTraceBeginM();
		mFitness = inFitness;
		Beagle_StackTraceEndM("void Individual::setFitness(Fitness::Handle inFitness)");
	}

protected:

	MemberMap        mMemberMap;     //!< Members of the individual.
	Fitness::Handle  mFitness;       //!< Fitness value.

public:

	virtual void read(PACC::XML::ConstIterator inIter);   // Undefined!

};

}

#endif // Beagle_Core_Individual_hpp
