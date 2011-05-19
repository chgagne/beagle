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
 *  \file   beagle/HPC/ProcessingBuffer.hpp
 *  \brief  Definition of the class ProcessingBuffer.
 *  \author Felix-Antoine Fortin
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: $
 */

#ifndef Beagle_ProcessingBuffer_hpp
#define Beagle_ProcessingBuffer_hpp

namespace Beagle
{

// Forward declaration
class Deme;

namespace HPC
{
/*!
 *  \class ProcessingBuffer beagle/HPC/ProcessingBuffer.hpp "beagle/HPC/ProcessingBuffer.hpp"
 *  \brief A processing buffer is used to contain the state of individuals that are going to be evaluated.
 *  \ingroup ECF
 *  \ingroup Pop
 *  \ingroup HPC
 */
class ProcessingBuffer : public Member, public std::deque<Individual::Handle>
{

public:

	//! MigrationBuffer allocator type.
	typedef AllocatorT<ProcessingBuffer,Member::Alloc> Alloc;
	//! MigrationBuffer handle type.
	typedef PointerT<ProcessingBuffer,Member::Handle> Handle;
	//! MigrationBuffer mixed bag type.
	typedef ContainerT<ProcessingBuffer,Member::Bag> Bag;

	//! Construct migration buffer
	ProcessingBuffer()
	{ }
	virtual ~ProcessingBuffer()
	{ }

	inline unsigned int size() const
	{
		return std::deque<Individual::Handle>::size();
	}
	
	inline void clear()
	{
		mIndividualIndex.clear();
		std::deque<Individual::Handle>::clear();
	}

	inline void push_back(Individual::Handle inIndiv, unsigned int inIndex)
	{
		std::deque<Individual::Handle>::push_back(inIndiv);
		mIndividualIndex.push_back(inIndex);
	}

	inline void copy(const Member& inOriginal, System& ioSystem)
	{
		Beagle_StackTraceBeginM();
		throw Beagle_UndefinedMethodInternalExceptionM("ProcessingBuffer", "ProcessingBuffer", "ProcessingBuffer");
		Beagle_StackTraceEndM();
	}

	inline const std::string& getName() const
	{
		Beagle_StackTraceBeginM();
		const static std::string lName("ProcessingBuffer");
		return lName;
		Beagle_StackTraceEndM();
	}

	inline const std::string& getType() const
	{
		Beagle_StackTraceBeginM();
		const static std::string lType("ProcessingBuffer");
		return lType;
		Beagle_StackTraceEndM();
	}

	inline const UIntArray& getIndex() const
	{
		return mIndividualIndex;
	}

protected:
	void push_back(Individual::Handle) ;
	UIntArray mIndividualIndex;

};
}
}

#endif
