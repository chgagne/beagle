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
 *  \file   beagle/Core/Fitness.hpp
 *  \brief  Definition of the class Fitness.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 20:58:00 $
 */

/*!
 *  \defgroup FitStats Fitness and Statistics
 *  \ingroup Core
 *  \brief Individual fitness value and statistics measures, part of the Core library.
 *
 *  The statistics of the fitness measurements of a population is held in the following structure.
 *  \code
 *  struct Measure {
 *    string mID;  //!< ID of the measure.
 *    float mAvg;  //!< Average of the statistic measure.
 *    float mStd;  //!< Standard deviation of the statistic measure.
 *    float mMax;  //!< Maximum of the statistic measure.
 *    float mMin;  //!< Minimum of the statistic measure.
 *  };
 *
 *  class Stats : public Object, public std::vector<Measure> {
 *  protected:
 *    string  mID;                  //!< ID of the stats.
 *    unsigned int mGeneration;     //!< Generation at which the measure had been taken.
 *    unsigned int mProcessed;      //!< Number of individual processed in the generation.
 *    unsigned int mTotalProcessed; //!< Total number of individual processed in the evolution.
 *    unsigned int mPopSize;        //!< Population size associated to the stats.
 *  };
 *
 *  \endcode
 *  For the usual EC algorithm, a generation statistics for a population is calculated every
 *  generation. The statistics are calculated by the method Beagle::Fitness::calculateStats.
 *
 *  The fitness holder of an individual is represented in an object derived of the type
 *  Beagle::Fitness.
 *  \code
 *  class Fitness : public Object {
 *  public:
 *    virtual void calculateStats(Deme& ioDeme, Context& ioContext) const =0;
 *    virtual bool isLess(const Object&) const = 0;
 *  };
 *  \endcode
 *  When the user implements a concrete evaluation class, the method Beagle::EvaluationOp::evaluate
 *  must returns a fitness object appropriate to the application. There is some of the predefined
 *  standard fitness measures, such Beagle::FitnessSimple or Beagle::GP::FitnessKoza, which defined
 *  respectively a fitness made of a single float or a more elaborated fitness taken from.
 *  A specialized fitness type could also be defined for more sophisticated evolution.
 *
 */


#ifndef Beagle_Core_Fitness_hpp
#define Beagle_Core_Fitness_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Core/Object.hpp"
#include "beagle/Core/AbstractAllocT.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/Stats.hpp"


namespace Beagle
{

// Forward declaration.
class Deme;
class Context;

/*!
 *  \class Fitness beagle/Core/Fitness.hpp "beagle/Core/Fitness.hpp"
 *  \brief Abstract individual's fitness class.
 *  \ingroup Core
 *  \ingroup FitStats
 */
class Fitness : public Member
{

public:

	//! Fitness allocator type.
	typedef AbstractAllocT<Fitness,Member::Alloc> Alloc;
	//! Fitness handle type.
	typedef PointerT<Fitness,Member::Handle> Handle;
	//! Fitness bag type.
	typedef ContainerT<Fitness,Member::Bag> Bag;

	explicit Fitness(bool inValid=false);
	virtual ~Fitness()
	{ }

	virtual double              getDistance(const Fitness& inRightFitness) const;
	virtual const std::string&  getName() const;
	virtual unsigned int        getNumberOfObjectives() const;
	virtual double              getObjective(unsigned int inObjIndex) const;
	virtual bool                isDominated(const Fitness& inRightFitness) const;
	virtual void                read(PACC::XML::ConstIterator inIter);
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Return validity of the fitness.
	 *  \return True if fitness is valid, false if not.
	 */
	inline bool isValid() const
	{
		Beagle_StackTraceBeginM();
		return mValid;
		Beagle_StackTraceEndM("bool Fitness::isValid() const");
	}

	/*!
	 *  \brief Invalidate the fitness.
	 */
	inline void setInvalid()
	{
		Beagle_StackTraceBeginM();
		mValid = false;
		Beagle_StackTraceEndM("void Fitness::setInvalid()");
	}

	/*!
	 *  \brief Validate the fitness.
	 */
	inline void setValid()
	{
		Beagle_StackTraceBeginM();
		mValid = true;
		Beagle_StackTraceEndM("void Fitness::setValid()");
	}

private:

	bool mValid;  //!< Flag set if the fitness is valid (unset if not).

};

}

#endif // Beagle_Core_Fitness_hpp
