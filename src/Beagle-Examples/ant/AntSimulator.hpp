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
*  \file   AntSimulator.hpp
*  \brief  Definition of the type AntSimulator.
*  \author Christian Gagne <cgagne@gmail.com>
*  $Revision: 1.3 $
*  $Date: 2007/08/09 21:43:12 $
*/

#ifndef AntSimulator_hpp
#define AntSimulator_hpp

#include "Beagle/EC.hpp"

#include <vector>


/*!
 *  \class AntSimulator AntSimulator.hpp "AntSimulator.hpp"
 *  \brief Simulator used for fitness evaluation in the artificial ant problem.
 *  \ingroup Ant
 */
class AntSimulator : public Beagle::Component
{

public:

	//! AntSimulator allocator type.
	typedef Beagle::AllocatorT<AntSimulator,Beagle::Component::Alloc> Alloc;
	//!< AntSimulator handle type.
	typedef Beagle::PointerT<AntSimulator,Beagle::Component::Handle> Handle;
	//!< AntSimulator bag type.
	typedef Beagle::ContainerT<AntSimulator,Beagle::Component::Bag> Bag;

	//!< Different trail case states possible.
	enum State {eStart='S', eEmpty='.', ePassed='x',
	            eFoodPiece='#', eEatenPiece='@',
	            eAntNorth='^', eAntEast='}', eAntSouth='v', eAntWest='{'};

	explicit AntSimulator(unsigned int inMaxMoves=400);

	bool moveForward();
	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	void reset();
	bool senseFood() const;
	bool turnLeft();
	bool turnRight();
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \return Number of columns of the trail.
	 */
	inline unsigned int getNumberColumns() const
	{
		return mOrigTrail.size();
	}

	/*!
	 *  \return Number of rows of the trail.
	 */
	inline unsigned int getNumberRows() const
	{
		Beagle_AssertM(mOrigTrail.size() > 0);
		return mOrigTrail.front().size();
	}

	/*!
	 *  \return Column index of the ant in the trail.
	 */
	inline unsigned int getColumnAnt() const
	{
		return mColAnt;
	}

	/*!
	 *  \return Row index of the ant in the trail.
	 */
	inline unsigned int getRowAnt() const
	{
		return mRowAnt;
	}

	/*!
	 *  \return Direction in which the ant is looking, see enum type AntSimulator::State.
	 */
	inline char getDirectionAnt() const
	{
		return mDirectionAnt;
	}

	/*!
	 *  \return Column index where the ant starts moving.
	 */
	inline unsigned int getColumnStart() const
	{
		return mColStart;
	}

	/*!
	 *  \return Row index where the ant starts moving.
	 */
	inline unsigned int getRowStart() const
	{
		return mRowStart;
	}

	/*!
	 *  \return Direction in which the ant is starting, see enum type AntSimulator::State.
	 */
	inline char getDirectionStart() const
	{
		return mDirectionStart;
	}

	/*!
	 *  \return Number of moves done by the ant in the trail.
	 */
	inline unsigned int getMovesDone() const
	{
		return mNbMovesAnt;
	}

	/*!
	 *  \return Maximum number of moves allowed to the ant.
	 *
	 *  A value of 0 means that an unlimited number of moves is allowed.
	 */
	inline unsigned int getMaxMoves() const
	{
		return mMaxMoves;
	}

	/*!
	 *  \return Number of food pieces eaten by the ant.
	 */
	inline unsigned int getPiecesEaten() const
	{
		return mNbPiecesEaten;
	}

	/*!
	 *  \return Number of food pieces available to be eaten.
	 */
	inline unsigned int getPiecesAvailable() const
	{
		return mNbPiecesAvail;
	}

	/*!
	 *  \brief Get the state of a case in the execution trail.
	 *  \param inRow Row index of the case to get state of.
	 *  \param inCol Column index of the case to get state of.
	 *  \return State of the case, see enum type AntSimulator::State.
	 */
	inline char getStateExecutionTrail(unsigned int inRow, unsigned int inCol) const
	{
		Beagle_AssertM(inRow < mExecTrail.size());
		Beagle_AssertM(inCol < mExecTrail[inRow].size());
		return mExecTrail[inRow][inCol];
	}

	/*!
	 *  \brief Get the state of a case in the original trail (before ant moves).
	 *  \param inRow Row index of the case to get state of.
	 *  \param inCol Column index of the case to get state of.
	 *  \return State of the case, see enum type AntSimulator::State.
	 */
	inline char getStateOriginalTrail(unsigned int inRow, unsigned int inCol) const
	{
		Beagle_AssertM(inRow < mOrigTrail.size());
		Beagle_AssertM(inCol < mOrigTrail[inRow].size());
		return mOrigTrail[inRow][inCol];
	}

	/*!
	 *  \brief Change the maximum number of moves allowed.
	 *  \param inMaxMoves New maximum number of moves allowed.
	 *
	 *  A value of 0 means that an unlimited number of moves is allowed.
	 */
	inline void setMaxMoves(unsigned int inMaxMoves)
	{
		mMaxMoves = inMaxMoves;
	}

protected:

	std::vector< std::vector<char> > mOrigTrail; //!< Initial trail set-up.
	unsigned int mMaxMoves;       //!< Maximum number of moves allowed.
	unsigned int mNbPiecesAvail;  //!< Number of food pieces available.
	unsigned int mRowStart;       //!< Row at which the ant starts collecting food.
	unsigned int mColStart;       //!< Column at which the ant starts collecting food.
	unsigned int mDirectionStart; //!< Direction at which the ant is looking when starting.

	std::vector< std::vector<char> > mExecTrail; //!< Execution trail set-up.
	unsigned int mNbMovesAnt;     //!< Number of moves done by the ant.
	unsigned int mNbPiecesEaten;  //!< Number of food pieces eaten.
	unsigned int mRowAnt;         //!< Row of the actual ant position.
	unsigned int mColAnt;         //!< Column of the actual ant position.
	char         mDirectionAnt;   //!< Direction in which the ant is looking.

};


#endif // AntSimulator_hpp
