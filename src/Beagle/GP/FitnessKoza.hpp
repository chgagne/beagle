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
 *  \file   beagle/GP/FitnessKoza.hpp
 *  \brief  Definition of the class GP/FitnessKoza.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:11 $
 */

/*!
 *  \defgroup GPFit GP Fitness
 *  \ingroup GPF
 *  \brief Genetic programming specific fitness measure and related elements, part of the GP
 *  framework.
 */

#ifndef Beagle_GP_FitnessKoza_hpp
#define Beagle_GP_FitnessKoza_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Stats.hpp"
#include "beagle/FitnessSimple.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class FitnessKoza beagle/GP/FitnessKoza.hpp "beagle/GP/FitnessKoza.hpp"
 *  \brief Koza's fitness class.
 *  \ingroup GPF
 *  \ingroup GPFit
 *  \par Note:
 *    Fitness measure express by Koza in Genetic Programming I. Only Normalized fitness
 *    is used in the natural selection process, others fitness are only indications.
 */
class FitnessKoza : public FitnessSimple
{

public:

	//! GP::FitnessKoza allocator type.
	typedef AllocatorT<FitnessKoza,FitnessSimple::Alloc>
	Alloc;
	//! GP::FitnessKoza handle type.
	typedef PointerT<FitnessKoza,FitnessSimple::Handle>
	Handle;
	//! GP::FitnessKoza bag type.
	typedef ContainerT<FitnessKoza,FitnessSimple::Bag>
	Bag;

	FitnessKoza();
	explicit FitnessKoza(float inNormalizedFitness,
	                     float inAdjustedFitness,
	                     float inStandardizedFitness,
	                     float inRawFitness,
	                     unsigned int inHits);

	virtual const std::string&  getType() const;
	virtual void                read(PACC::XML::ConstIterator inIter);
	virtual void                setFitness(float inNormalizedFitness,
	                                       float inAdjustedFitness,
	                                       float inStandardizedFitness,
	                                       float inRawFitness,
	                                       unsigned int inHits);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Return normalized fitness value.
	 *  \return Normalized fitness value.
	 */
	inline float getNormalizedFitness() const
	{
		Beagle_StackTraceBeginM();
		return mValue;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return adjusted fitness value.
	 *  \return Adjusted fitness value.
	 */
	inline float getAdjustedFitness() const
	{
		Beagle_StackTraceBeginM();
		return mAdjustedFitness;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return standardized fitness value.
	 *  \return Standardized fitness value.
	 */
	inline float getStandardizedFitness() const
	{
		Beagle_StackTraceBeginM();
		return mStandardizedFitness;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return raw fitness value.
	 *  \return Raw fitness value.
	 */
	inline float getRawFitness() const
	{
		Beagle_StackTraceBeginM();
		return mRawFitness;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return hits measure value.
	 *  \return Hits measure value.
	 */
	inline unsigned int getHits() const
	{
		Beagle_StackTraceBeginM();
		return mHits;
		Beagle_StackTraceEndM();
	}

private:

	float        mAdjustedFitness;      //!< Adjusted fitness measure.
	float        mStandardizedFitness;  //!< Standardized fitness measure.
	float        mRawFitness;           //!< Standardized fitness measure.
	unsigned int mHits;                 //!< Number of hits.

};

}
}

#endif // Beagle_GP_FitnessKoza_hpp

