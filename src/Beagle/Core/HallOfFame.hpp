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
 *  \file   Beagle/Core/HallOfFame.hpp
 *  \brief  Definition of the class HallOfFame.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: $
 *  $Date: 2007/09/12 14:23:47 $
 */

#ifndef Beagle_Core_HallOfFame_hpp
#define Beagle_Core_HallOfFame_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Individual.hpp"
#include "Beagle/Core/Member.hpp"
#include "Beagle/Core/Logger.hpp"


namespace Beagle
{

/*!
 *  \class HallOfFame Beagle/Core/HallOfFame.hpp "Beagle/Core/HallOfFame.hpp"
 *  \brief A hall-of-fame is a set containing the best-of-run individuals.
 *  \warning The individuals in the hall-of-fame are not used at all in the evolution process.
 *  \ingroup Core
 *  \ingroup Pop
 */
class HallOfFame : public Member
{

public:

	/*!
	 *  \struct Entry Beagle/HallOfFame.hpp "Beagle/HallOfFame.hpp"
	 *  \brief An entry of the hall-of-fame, that is a best-of-run individual.
	 *  \ingroup Core
	 *  \ingroup Pop
	 */
	struct Entry {
		Individual::Handle mIndividual;    //!< Individual member of the hall-of-fame.
		unsigned int       mGeneration;    //!< Generation of introduction in the hall-of-fame.
		unsigned int       mDemeIndex;     //!< Deme index to which the member belong.

		Entry(Individual::Handle inIndividual=NULL,
		      unsigned int inGeneration=0,
		      unsigned int inDemeIndex=0);
		bool operator==(const HallOfFame::Entry& inRightEntry) const;
		bool operator<(const HallOfFame::Entry& inRightEntry) const;
		bool operator>(const HallOfFame::Entry& inRightEntry) const;
	};

	//! HallOfFame allocator type.
	typedef AllocatorT<HallOfFame,Member::Alloc> Alloc;
	//! HallOfFame handle type.
	typedef PointerT<HallOfFame,Member::Handle> Handle;
	//! HallOfFame bag type.
	typedef ContainerT<HallOfFame,Member::Bag> Bag;

	explicit HallOfFame(unsigned int inN=0);
	explicit HallOfFame(unsigned int inN, const HallOfFame::Entry& inModel);
	virtual ~HallOfFame()
	{ }

	virtual void                copy(const Member& inOriginal, System& ioSystem);
	virtual const std::string&  getName() const;
	virtual const std::string&  getType() const;
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	void                        sort();
	virtual bool                updateWithDeme(unsigned int inSizeHOF, const Deme& inDeme, Context& ioContext);
	virtual bool                updateWithIndividual(unsigned int inSizeHOF, const Individual& inIndividual, Context& ioContext);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief  Access to a constant reference to the Nth member of the hall-of-fame.
	 *  \param  inN Indice of the member to get.
	 *  \return Constant reference to the member at the indice N.
	 */
	inline const HallOfFame::Entry& operator[](unsigned int inN) const {
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,mEntries.size()-1);
		return mEntries[inN];
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Access to a reference to the Nth member of the hall-of-fame.
	 *  \param  inN Indice of the member to get.
	 *  \return Reference to the member at the indice N.
	 */
	inline HallOfFame::Entry& operator[](unsigned int inN) {
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,mEntries.size()-1);
		return mEntries[inN];
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Clear individuals from hall of fame
	 */
	inline void clear() {
		Beagle_StackTraceBeginM();
		mEntries.clear();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Number of members in the hall-of-fame.
	 */
	inline unsigned int size() const {
		Beagle_StackTraceBeginM();
		return mEntries.size();
		Beagle_StackTraceEndM();
	}

protected:

	std::vector< HallOfFame::Entry > mEntries;   //!< Vector of members of the hall-of-fame.

};

}


#endif // Beagle_Core_HallOfFame_hpp

