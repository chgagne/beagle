/*
 *  Artifical ant (ant)
 *  Copyright (C) 2006 by Christian Gagne
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
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
 *  \file   AntSimulator.cpp
 *  \brief  Implementation of class AntSimulator.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/10 20:32:24 $
 */

#include "AntSimulator.hpp"

using namespace Beagle;


/*!
 *  \brief Construct an ant simulator.
 *  \param inMaxMoves Maximum number of moves allowed to the ant.
 */
AntSimulator::AntSimulator(unsigned int inMaxMoves) :
		Component("AntSimulator"),
		mMaxMoves(inMaxMoves),
		mNbPiecesAvail(0),
		mRowStart(0),
		mColStart(0),
		mDirectionStart(AntSimulator::eAntEast),
		mNbMovesAnt(0),
		mNbPiecesEaten(0),
		mRowAnt(0),
		mColAnt(0),
		mDirectionAnt(AntSimulator::eAntEast)
{ }


/*!
 *  \brief Move the ant one case in the direction it is oriented.
 *  \return True if the move have been done, false is maximum number of moves reached.
 */
bool AntSimulator::moveForward()
{
	if(mNbMovesAnt >= mMaxMoves) return false;
	++mNbMovesAnt;
	switch(mDirectionAnt) {
	case eAntNorth: {
		Beagle_AssertM(mExecTrail.size() > 0);
		if(mRowAnt == 0) mRowAnt = (mExecTrail.size()-1);
		else --mRowAnt;
		break;
	}
	case eAntEast: {
		Beagle_AssertM(mExecTrail.size() > 0);
		Beagle_AssertM(mExecTrail.front().size() > 0);
		++mColAnt;
		if(mColAnt >= mExecTrail.front().size()) mColAnt = 0;
		break;
	}
	case eAntSouth: {
		Beagle_AssertM(mExecTrail.size() > 0);
		++mRowAnt;
		if(mRowAnt >= mExecTrail.size()) mRowAnt = 0;
		break;
	}
	case eAntWest: {
		Beagle_AssertM(mExecTrail.size() > 0);
		Beagle_AssertM(mExecTrail.front().size() > 0);
		if(mColAnt == 0) mColAnt = (mExecTrail.front().size()-1);
		else --mColAnt;
		break;
	}
	default: {
		throw Beagle_RunTimeExceptionM("Invalid value of ant direction in ant simulator.");
	}
	}
	switch(mExecTrail[mRowAnt][mColAnt]) {
	case eStart:
	case ePassed:
	case eEatenPiece:
		break;
	case eEmpty: {
		mExecTrail[mRowAnt][mColAnt] = ePassed;
		break;
	}
	case eFoodPiece: {
		mExecTrail[mRowAnt][mColAnt] = eEatenPiece;
		++mNbPiecesEaten;
		break;
	}
	default: {
		throw Beagle_RunTimeExceptionM("Invalid case value in ant simulator.");
	}
	}
	return true;
}


/*!
 *  \brief Read the ant simulator from a XML iterator.
 *  \param inIter XML iterator pointing to the structure to read the simulator from.
 */
void AntSimulator::read(PACC::XML::ConstIterator inIter)
{
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="AntSimulator"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <AntSimulator> expected!");

	std::string lNbRowsStr = inIter->getAttribute("rows");
	if(lNbRowsStr.empty()) {
		throw Beagle_IOExceptionNodeM(*inIter, "expected attribute 'rows'");
	} else {
		const unsigned int lNbRows = str2uint(lNbRowsStr);
		mOrigTrail.resize(lNbRows);
		mExecTrail.resize(lNbRows);
	}

	std::string lNbColsStr = inIter->getAttribute("cols");
	if(lNbColsStr.empty()) {
		throw Beagle_IOExceptionNodeM(*inIter, "expected attribute 'cols'");
	} else {
		const unsigned int lNbCols = str2uint(lNbColsStr);
		for(unsigned int i=0; i<mOrigTrail.size(); ++i) {
			mOrigTrail[i].resize(lNbCols);
			mExecTrail[i].resize(lNbCols);
		}
	}

	std::string lRowStartStr = inIter->getAttribute("row-start");
	if(lRowStartStr.empty()) mRowStart = 0;
	else mRowStart = str2uint(lRowStartStr);

	std::string lColStartStr = inIter->getAttribute("col-start");
	if(lColStartStr.empty()) mColStart = 0;
	else mColStart = str2uint(lColStartStr);

	std::string lDirStartStr = inIter->getAttribute("dir-start");
	if(lDirStartStr.empty()) mDirectionStart = eAntEast;
	else if(lDirStartStr == "north") mDirectionStart = eAntNorth;
	else if(lDirStartStr == "east")  mDirectionStart = eAntEast;
	else if(lDirStartStr == "south") mDirectionStart = eAntSouth;
	else if(lDirStartStr == "west")  mDirectionStart = eAntWest;
	else throw Beagle_IOExceptionNodeM(*inIter, "invalid value of attribute 'dir-start'");

	std::string lMaxMovesStr = inIter->getAttribute("moves-max");
	if(lMaxMovesStr.empty()) mMaxMoves = 0;
	else mMaxMoves = str2uint(lMaxMovesStr);

	std::string lMovesStr = inIter->getAttribute("moves-ant");
	if(lMovesStr.empty()) mNbMovesAnt = 0;
	else mNbMovesAnt = str2uint(lMovesStr);

	std::string lEatenStr = inIter->getAttribute("eaten-ant");
	if(lEatenStr.empty()) mNbPiecesEaten = 0;
	else mNbPiecesEaten = str2uint(lEatenStr);

	std::string lRowAntStr = inIter->getAttribute("row-ant");
	if(lRowAntStr.empty()) mRowAnt = mRowStart;
	else mRowAnt = str2uint(lRowAntStr);

	std::string lColAntStr = inIter->getAttribute("col-ant");
	if(lColAntStr.empty()) mColAnt = mColStart;
	else mColAnt = str2uint(lColAntStr);

	std::string lDirAntStr = inIter->getAttribute("dir-ant");
	if(lDirAntStr.empty()) mDirectionAnt = mDirectionStart;
	else if(lDirAntStr == "north") mDirectionAnt = eAntNorth;
	else if(lDirAntStr == "east")  mDirectionAnt = eAntEast;
	else if(lDirAntStr == "south") mDirectionAnt = eAntSouth;
	else if(lDirAntStr == "west")  mDirectionAnt = eAntWest;
	else throw Beagle_IOExceptionNodeM(*inIter, "invalid value of attribute 'dir-ant'");

	PACC::XML::ConstIterator lChild=inIter->getFirstChild();
	if(!lChild || (lChild->getType()!=PACC::XML::eString))
		throw Beagle_IOExceptionNodeM(*inIter, "expected string content!");
	std::istringstream lISS(lChild->getValue());
	unsigned int lNbEaten = 0;
	for(unsigned int i=0; i<mOrigTrail.size(); ++i) {
		for(unsigned int j=0; j<mOrigTrail[i].size(); ++j) {
			char lBuffer=0;
			lISS >> lBuffer;
			switch(lBuffer) {
			case eStart: {
				if((i!=mRowStart) || (j!=mColStart)) {
					std::ostringstream lOSS;
					lOSS << "start position given by trail ( " << i << "," << j;
					lOSS << ") and start position given by attributes (";
					lOSS << mRowStart << "," << mColStart <<") mismatch!";
					throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
				}
				mOrigTrail[i][j] = eStart;
				mExecTrail[i][j] = eStart;
				break;
			}
			case eEmpty:
			case eFoodPiece: {
				mOrigTrail[i][j] = lBuffer;
				mExecTrail[i][j] = lBuffer;
				break;
			}
			case ePassed: {
				mOrigTrail[i][j] = eEmpty;
				mExecTrail[i][j] = ePassed;
				break;
			}
			case eEatenPiece: {
				mOrigTrail[i][j] = eFoodPiece;
				mExecTrail[i][j] = eEatenPiece;
				++lNbEaten;
				break;
			}
			case eAntNorth:
			case eAntEast:
			case eAntSouth:
			case eAntWest: {
				mOrigTrail[i][j] = eEmpty;
				mExecTrail[i][j] = lBuffer;
				break;
			}
			case 0: {
				std::ostringstream lOSS;
				lOSS << "premature end of trail string reached at position (";
				lOSS << i << "," << j << "), expected some more characters!";
				throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
			}
			default: {
				std::ostringstream lOSS;
				lOSS << "invalid character \'" << lBuffer << "\' at position (";
				lOSS << i << "," << j << ") of the string trail description!";
				throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
			}
			}
		}
	}
	if((mNbPiecesEaten!=0) && (lNbEaten!=mNbPiecesEaten)) {
		std::ostringstream lOSS;
		lOSS << "number of food pieces given by XML attribute (" << mNbPiecesEaten;
		lOSS << " mismatch number of food pieces computed from the string trail description (";
		lOSS << lNbEaten << ")!";
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
}


/*!
 *  \brief Read the ant simulator from a XML iterator.
 *  \param inIter XML iterator pointing to the structure to read the simulator from.
 *  \param ioSystem Evolutionary system.
 */
void AntSimulator::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	read(inIter);
}


/*!
 *  \brief Reset the state of the ant simulator.
 */
void AntSimulator::reset()
{
	mExecTrail = mOrigTrail;
	mNbMovesAnt = 0;
	mNbPiecesEaten = 0;
	mRowAnt = mRowStart;
	mColAnt = mColStart;
	mDirectionAnt = mDirectionStart;
}


/*!
 *  \brief Sense if there is food in the neighbor case in the direction than ant is oriented.
 *  \return True if the sensing have been done, false is maximum number of moves reached.
 */
bool AntSimulator::senseFood() const
{
	unsigned int lAheadRow = mRowAnt;
	unsigned int lAheadCol = mColAnt;
	switch(mDirectionAnt) {
	case eAntNorth: {
		Beagle_AssertM(mExecTrail.size() > 0);
		if(lAheadRow == 0) lAheadRow = (mExecTrail.size()-1);
		else --lAheadRow;
		break;
	}
	case eAntEast: {
		Beagle_AssertM(mExecTrail.size() > 0);
		Beagle_AssertM(mExecTrail.front().size() > 0);
		++lAheadCol;
		if(lAheadCol >= mExecTrail.front().size()) lAheadCol = 0;
		break;
	}
	case eAntSouth: {
		Beagle_AssertM(mExecTrail.size() > 0);
		++lAheadRow;
		if(lAheadRow >= mExecTrail.size()) lAheadRow = 0;
		break;
	}
	case eAntWest: {
		Beagle_AssertM(mExecTrail.size() > 0);
		Beagle_AssertM(mExecTrail.front().size() > 0);
		if(lAheadCol == 0) lAheadCol = (mExecTrail.front().size()-1);
		else --lAheadCol;
		break;
	}
	default: {
		throw Beagle_RunTimeExceptionM("Invalid value of ant direction in ant simulator.");
	}
	}
	return mExecTrail[lAheadRow][lAheadCol] == eFoodPiece;
}


/*!
 *  \brief  Turn the ant 90 degrees counter-clockwize.
 *  \return True if the turn move have been done, false is maximum number of moves reached.
 */
bool AntSimulator::turnLeft()
{
	if(mNbMovesAnt >= mMaxMoves) return false;
	++mNbMovesAnt;
	switch(mDirectionAnt) {
	case eAntNorth: {
		mDirectionAnt = eAntWest;
		break;
	}
	case eAntEast: {
		mDirectionAnt = eAntNorth;
		break;
	}
	case eAntSouth: {
		mDirectionAnt = eAntEast;
		break;
	}
	case eAntWest: {
		mDirectionAnt = eAntSouth;
		break;
	}
	default: {
		throw Beagle_RunTimeExceptionM("Invalid value of ant direction in ant simulator.");
	}
	}
	return true;
}


/*!
 *  \brief  Turn the ant 90 degrees clockwize.
 *  \return True if the turn move have been done, false is maximum number of moves reached.
 */
bool AntSimulator::turnRight()
{
	if(mNbMovesAnt >= mMaxMoves) return false;
	++mNbMovesAnt;
	switch(mDirectionAnt) {
	case eAntNorth: {
		mDirectionAnt = eAntEast;
		break;
	}
	case eAntEast: {
		mDirectionAnt = eAntSouth;
		break;
	}
	case eAntSouth: {
		mDirectionAnt = eAntWest;
		break;
	}
	case eAntWest: {
		mDirectionAnt = eAntNorth;
		break;
	}
	default: {
		throw Beagle_RunTimeExceptionM("Invalid value of ant direction in ant simulator.");
	}
	}
	return true;
}


/*!
 *  \brief Write the ant simulator into an XML streamer.
 *  \param ioStreamer XML streamer to write into.
 *  \param inIndent Indent content.
 */
void AntSimulator::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	ioStreamer.openTag("AntSimulator", inIndent);
	ioStreamer.insertAttribute("rows", uint2str(mExecTrail.size()));
	if(mExecTrail.size() == 0) ioStreamer.insertAttribute("cols", "0");
	else ioStreamer.insertAttribute("cols", uint2str(mExecTrail.front().size()));
	ioStreamer.insertAttribute("row-start", uint2str(mRowStart));
	ioStreamer.insertAttribute("col-start", uint2str(mColStart));
	switch(mDirectionStart) {
	case eAntNorth: {
		ioStreamer.insertAttribute("dir-start", "north");
		break;
	}
	case eAntEast: {
		ioStreamer.insertAttribute("dir-start", "east");
		break;
	}
	case eAntSouth: {
		ioStreamer.insertAttribute("dir-start", "south");
		break;
	}
	case eAntWest: {
		ioStreamer.insertAttribute("dir-start", "west");
		break;
	}
	default: {
		throw Beagle_RunTimeExceptionM("Invalid value of start direction in ant simulator.");
	}
	}
	if(mMaxMoves!=0) ioStreamer.insertAttribute("moves-max", uint2str(mMaxMoves));
	if(mNbMovesAnt!=0) ioStreamer.insertAttribute("moves-ant", uint2str(mNbMovesAnt));
	if(mNbPiecesEaten!=0) ioStreamer.insertAttribute("eaten-ant", uint2str(mNbPiecesEaten));
	if(mRowAnt!=0) ioStreamer.insertAttribute("row-ant", uint2str(mRowAnt));
	if(mColAnt!=0) ioStreamer.insertAttribute("col-ant", uint2str(mColAnt));
	switch(mDirectionAnt) {
	case eAntNorth: {
		ioStreamer.insertAttribute("dir-ant", "north");
		break;
	}
	case eAntEast: {
		ioStreamer.insertAttribute("dir-ant", "east");
		break;
	}
	case eAntSouth: {
		ioStreamer.insertAttribute("dir-ant", "south");
		break;
	}
	case eAntWest: {
		ioStreamer.insertAttribute("dir-ant", "west");
		break;
	}
	default: {
		throw Beagle_RunTimeExceptionM("Invalid value of ant direction in ant simulator.");
	}
	}
	for(unsigned int i=0; i<mExecTrail.size(); ++i) {
		std::ostringstream lOSS;
		for(unsigned int j=0; j<mExecTrail[i].size(); ++j) {
			if((mNbMovesAnt > 0) && (i == mRowAnt) && (j == mColAnt)) {
				lOSS << mDirectionAnt;
			} else lOSS << mExecTrail[i][j];
		}
		ioStreamer.insertStringContent(lOSS.str());
	}
	ioStreamer.closeTag();
}
