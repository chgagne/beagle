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

#include "Beagle/EC.hpp"

#include <iostream>
#include <map>
#include <string>

using namespace Beagle;
using namespace Beagle::EC;


// This restrict the scope of the class to the actual file only.
namespace
{

typedef std::map< unsigned,unsigned int,std::less<unsigned int> >
CountMap;

/*!
 *  \brief Internal class used to log individual size frequency usage.
 */
class UsageCount : public Beagle::Object, public CountMap
{
public:
	explicit UsageCount(unsigned int inDemeID=0, unsigned int inGeneration=0);
	virtual ~UsageCount()
	{ }
	void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	unsigned int mDemeID;
	unsigned int mGeneration;
};


/*!
 *  \brief Construct usage count class.
 */
UsageCount::UsageCount(unsigned int inDemeID,
                       unsigned int inGeneration) :
		mDemeID(inDemeID),
		mGeneration(inGeneration)
{ }


/*!
 *  \brief Write usage count to the streamer.
 *  \param ioStreamer XML streamer to write usage count to.
 *  \param inIndent Whether XML output should be indented.
 */
void UsageCount::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("IndividualSizeFrequencyUsageStats", inIndent);
	ioStreamer.insertAttribute("deme", uint2str(mDemeID));
	ioStreamer.insertAttribute("generation", uint2str(mGeneration));
	for(const_iterator lIter=begin(); lIter!=end(); ++lIter) {
		ioStreamer.openTag("IndividualSize", inIndent);
		ioStreamer.insertAttribute("size", uint2str(lIter->first));
		ioStreamer.insertAttribute("count", uint2str(lIter->second));
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
	Beagle_StackTraceEndM("void UsageCount::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

}


/*!
 *  \brief Construct individual size frequency display operator.
 *  \param inName Name of the primitive.
 */
IndividualSizeFrequencyStatsOp::IndividualSizeFrequencyStatsOp(std::string inName) :
		Beagle::Operator(inName)
{ }


/*!
 *  \brief Compute primitive usage statistics from deme.
 *  \param ioDeme Deme to compute individual frequency on.
 *  \param ioContext Evolutionary context.
 */
void IndividualSizeFrequencyStatsOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	// Compute usage statistics
	UsageCount lCounter(ioContext.getDemeIndex(), ioContext.getGeneration());
	for(unsigned int i=0; i<ioDeme.size(); ++i) {
		CountMap::iterator lIter = lCounter.find(ioDeme[i]->size());
		if(lIter == lCounter.end()) lCounter[ioDeme[i]->size()] = 1;
		else ++(lIter->second);
	}

	// Display usage statistics
	Beagle_LogObjectM(
	    ioContext.getSystem().getLogger(),
	    Logger::eStats,
	    "stats", "Beagle::IndividualSizeFrequencyStatsOp",
	    lCounter
	);
	Beagle_StackTraceEndM("void IndividualSizeFrequencyStatsOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)");
}
