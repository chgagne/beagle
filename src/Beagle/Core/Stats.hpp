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
 *  \file   Beagle/Core/Stats.hpp
 *  \brief  Definition of class Stats.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18 $
 *  $Date: 2007/09/21 17:45:46 $
 */

#ifndef Beagle_Core_Stats_hpp
#define Beagle_Core_Stats_hpp

#include <string>

#include "PACC/XML.hpp"

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Pointer.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/Allocator.hpp"
#include "Beagle/Core/Container.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/Member.hpp"
#include "Beagle/Core/HashString.hpp"
#include "Beagle/Core/AssertException.hpp"
#include "Beagle/Core/RunTimeException.hpp"

#ifdef BEAGLE_HAVE_STDTR1_UNORDEREDMAP
#include <tr1/unordered_map>
#else
#ifdef BEAGLE_HAVE_STD_UNORDEREDMAP
#include <unordered_map>
#endif // BEAGLE_HAVE_STD_UNORDEREDMAP
#endif //BEAGLE_HAVE_STDTR1_UNORDEREDMAP

namespace Beagle
{


/*!
 *  \struct Measure Beagle/Core/Stats.hpp "Beagle/Core/Stats.hpp"
 *  \brief Statistical measure of an evolution.
 *  \ingroup Core
 *  \ingroup FitStats
 */
struct Measure {
	std::string mID;    //!< ID of the measure.
	double      mAvg;   //!< Average of the statistic measure.
	double      mStd;   //!< Standard deviation of the statistic measure.
	double      mMax;   //!< Maximum of the statistic measure.
	double      mMin;   //!< Minimum of the statistic measure.

	explicit Measure(std::string inID = "",
	                 double inAvg = 0.0,
	                 double inStd = 0.0,
	                 double inMax = 0.0,
	                 double inMin = 0.0);
};


/*!
 *  \class Stats Beagle/Core/Stats.hpp "Beagle/Core/Stats.hpp"
 *  \brief Generation statistics measure.
 *  \ingroup Core
 *  \ingroup FitStats
 */
class Stats : public Member, public std::vector<Measure>
{

public:

	//! Stats allocator type.
	typedef AllocatorT<Stats,Member::Alloc> Alloc;
	//! Stats handle type.
	typedef PointerT<Stats,Member::Handle> Handle;
	//! Stats bag type.
	typedef ContainerT<Stats,Member::Bag> Bag;

	explicit Stats(std::string  inID="",
	               unsigned int inGeneration=0,
	               unsigned int inPopSize=0,
	               bool         inValid=false);
	virtual ~Stats()
	{ }

	virtual void                copy(const Member& inOriginal, System& ioSystem);
	virtual const std::string&  getName() const;
	virtual const std::string&  getType() const;
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Add an item to the statistics.
	 *  \param inTag Tag of the statistics item.
	 *  \param inValue Value of item to add.
	 *  \throw RunTimeException If the item is already in the stats.
	 */
	inline void addItem(std::string inTag, double inValue) {
		Beagle_StackTraceBeginM();
		ItemMap::const_iterator lIterObj = mItemMap.find(inTag);
		if(lIterObj != mItemMap.end()) {
			throw Beagle_RunTimeExceptionM("Could not add item to statistics as it already exist!");
		}
		mItemMap.insert(std::make_pair(inTag,inValue));
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Clear items of the statistics.
	 */
	inline void clearItems() {
		Beagle_StackTraceBeginM();
		mItemMap.clear();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Delete an item from the statistics.
	 *  \param  inTag Tag of the statistics item.
	 *  \return Value item deleted.
	 *  \throw  RunTimeException If the item is not in the stats.
	 */
	inline double deleteItem(std::string inTag) {
		Beagle_StackTraceBeginM();
		ItemMap::iterator lIterObj = mItemMap.find(inTag);
		if(lIterObj == mItemMap.end()) {
			std::ostringstream lOSS;
			lOSS << "Item named '" << inTag << "' is not in the statistics!";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
		double lValue = lIterObj->second;
		mItemMap.erase(lIterObj);
		return lValue;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Get value of an item of the statistics.
	 *  \param  inTag Item of the statistics item.
	 *  \return Value of item.
	 *  \throw  RunTimeException if there is no entry associated with the tag.
	 */
	inline double& getItem(std::string inTag) {
		Beagle_StackTraceBeginM();
		ItemMap::iterator lIterObj = mItemMap.find(inTag);
		if(lIterObj == mItemMap.end()) {
			std::ostringstream lOSS;
			lOSS << "Item named '" << inTag << "' is not in the statistics!";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
		return lIterObj->second;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief  Get value of an item of the statistics.
	 *  \param  inTag Item of the statistics item.
	 *  \return Value of item.
	 *  \throw  RunTimeException if there is no entry associated with the tag.
	 */
	inline const double& getItem(std::string inTag) const {
		Beagle_StackTraceBeginM();
		ItemMap::const_iterator lIterObj = mItemMap.find(inTag);
		if(lIterObj == mItemMap.end()) {
			std::ostringstream lOSS;
			lOSS << "Item named '" << inTag << "' is not in the statistics!";
			throw Beagle_RunTimeExceptionM(lOSS.str());
		}
		return lIterObj->second;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Id of the stats.
	 */
	inline std::string getID() const {
		Beagle_StackTraceBeginM();
		return mID;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Generation of the stats.
	 */
	inline unsigned int getGeneration() const {
		Beagle_StackTraceBeginM();
		return mGeneration;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Population size associated to the stats.
	 */
	inline unsigned int getPopSize() const {
		Beagle_StackTraceBeginM();
		return mPopSize;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Is given item is in the statistics.
	 *  \param inTag Tag of the statistics item.
	 */
	inline bool existItem(std::string inTag) const {
		Beagle_StackTraceBeginM();
		ItemMap::const_iterator lIterObj = mItemMap.find(inTag);
		return (lIterObj != mItemMap.end());
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return validity of the stats.
	 *  \return True if stats are valid, false if not.
	 */
	inline bool isValid() const {
		Beagle_StackTraceBeginM();
		return mValid;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Add an item to the statistics.
	 *  \param inTag Tag of the statistics item.
	 *  \param inValue Value of item to modify.
	 */
	inline void modifyItem(std::string inTag, double inValue) {
		Beagle_StackTraceBeginM();
		mItemMap[inTag] = inValue;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Invalidate the stats.
	 */
	inline void setInvalid() {
		Beagle_StackTraceBeginM();
		mValid = false;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Validate the stats.
	 */
	inline void setValid() {
		Beagle_StackTraceBeginM();
		mValid = true;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the generational values of the stats.
	 *  \param inID Stats id.
	 *  \param inGeneration Generation value.
	 *  \param inPopSize Population size.
	 *  \param inValid If the stats are valid or not.
	 */
	inline void setGenerationValues(std::string inID,
	                                unsigned int inGeneration,
	                                unsigned int inPopSize,
	                                bool inValid=true) {
		Beagle_StackTraceBeginM();
		mID = inID;
		mGeneration = inGeneration;
		mPopSize = inPopSize;
		mValid = inValid;
		Beagle_StackTraceEndM();
	}

protected:

#if defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP)
	typedef std::tr1::unordered_map< std::string,double,HashString > ItemMap;
#elif defined(BEAGLE_HAVE_STD_UNORDEREDMAP)
	typedef std::unordered_map< std::string,double,HashString> > ItemMap;
#else // No hash_map found
	typedef std::map< std::string,double,std::less<std::string> > ItemMap;
#endif

	ItemMap         mItemMap;     //!< Statistics item evaluated.
	std::string     mID;          //!< ID of the stats.
	unsigned int    mGeneration;  //!< Generation at which the measure had been taken.
	unsigned int    mPopSize;     //!< Population size.
	bool            mValid;       //!< Flag that indicate whether the stats are valids.

};

}

#endif // Beagle_Core_Stats_hpp
