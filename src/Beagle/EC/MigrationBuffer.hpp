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
 *  \file   Beagle/EC/MigrationBuffer.hpp
 *  \brief  Definition of the class MigrationBuffer.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.5 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_MigrationBuffer_hpp
#define Beagle_EC_MigrationBuffer_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{


/*!
 *  \class MigrationBuffer Beagle/EC/MigrationBuffer.hpp "Beagle/EC/MigrationBuffer.hpp"
 *  \brief A migration buffer is used to contain the state of individual migration between demes.
 *  \ingroup EC
 *  \ingroup Pop
 */
class MigrationBuffer : public Beagle::Member
{

public:

	//! MigrationBuffer allocator type.
	typedef Beagle::AllocatorT<MigrationBuffer,Beagle::Member::Alloc> Alloc;
	//! MigrationBuffer handle type.
	typedef Beagle::PointerT<MigrationBuffer,Beagle::Member::Handle> Handle;
	//! MigrationBuffer mixed bag type.
	typedef Beagle::ContainerT<MigrationBuffer,Beagle::Member::Bag> Bag;

	//! Construct migration buffer
	MigrationBuffer()
	{ }
	virtual ~MigrationBuffer()
	{ }

	void                        clear();
	virtual void                copy(const Beagle::Member& inOriginal, Beagle::System& ioSystem);
	virtual const std::string&  getName() const;
	virtual const std::string&  getType() const;
	void                        insertEmigrants(const std::vector<unsigned int>& inEmigrantIndices, Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	void                        insertReplaced(const std::vector<unsigned int>& inReplacedIndices, Beagle::Deme& ioDeme);
	void                        mergeImmigrantsToDeme(Beagle::Deme& ioDeme);
	void                        moveMigrants(unsigned int inNbMigrants, Beagle::Deme& ioDestDeme, Beagle::Context& ioContext);
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Beagle::Context& ioContext);
	virtual void                writeContent(PACC::XML::Streamer &ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Return number of emigrants available in current migration buffer.
	 *  \return Number of emigrants available in current migration buffer.
	 */
	inline unsigned int getNumberOfEmigrants() const {
		Beagle_StackTraceBeginM();
		return mEmigrants.size();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return number of immigrants available in current migration buffer.
	 *  \return Number of emigrants available in current migration buffer.
	 */
	inline unsigned int getNumberOfImmigrants() const {
		Beagle_StackTraceBeginM();
		return mImmigrants.size();
		Beagle_StackTraceEndM();
	}

protected:

	std::deque<Beagle::Individual::Handle> mEmigrants;         //!< Buffer of emigrant individuals.
	std::deque<Beagle::Individual::Handle> mImmigrants;        //!< Individuals waiting to be inserted in deme.
	std::deque<unsigned int>               mReplacedIndices;   //!< Individual to be replaced by immigrants.

};

}

}

#endif // Beagle_EC_MigrationBuffer_hpp
