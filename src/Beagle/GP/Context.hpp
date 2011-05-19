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
 *  \file   beagle/GP/Context.hpp
 *  \brief  Definition of the type GP::Context.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16 $
 *  $Date: 2007/08/09 21:43:10 $
 */

#ifndef Beagle_GP_Context_hpp
#define Beagle_GP_Context_hpp

#include "PACC/Util.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/AssertException.hpp"
#include "beagle/Context.hpp"
#include "beagle/GP/Individual.hpp"
#include "beagle/GP/Tree.hpp"
#include "beagle/GP/MaxNodesExecutionException.hpp"
#include "beagle/GP/MaxTimeExecutionException.hpp"


namespace Beagle
{
namespace GP
{


/*!
 *  \class Context beagle/GP/Context.hpp "beagle/GP/Context.hpp"
 *  \brief The GP context class.
 *  \ingroup GPF
 *  \ingroup GPSys
 */
class Context : public Beagle::Context
{

public:

	//! GP::Context allocator type.
	typedef AllocatorT<Context,Beagle::Context::Alloc>
	Alloc;
	//! GP::Context handle type.
	typedef PointerT<Context,Beagle::Context::Handle>
	Handle;
	//! GP::Context bag type.
	typedef ContainerT<Context,Beagle::Context::Bag>
	Bag;

	Context();
	virtual ~Context()
	{ }

	/*!
	 *  \brief Return the nth element of the call stack.
	 *  \param inN Index of the element to return.
	 *  \return Value of the nth element.
	 *  \throw Beagle::AssertException If the index is out of bound.
	 */
	inline unsigned int operator[](unsigned int inN) const
	{
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,mCallStack.size()-1);
		return mCallStack[inN];
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Check if execution time is more or less than allowed execution time. Test is done
	 *    only when allowed execution time is greater than 0.
	 *  \throw Beagle::GP::MaxTimeExecutionException If execution time is more than allowed time.
	 */
	inline void checkExecutionTime() const
	{
		Beagle_StackTraceBeginM();
		if(mAllowedExecutionTime > 0.0) {
			const double lTimeElapsed = mExecutionTimer.getValue();
			if(lTimeElapsed > mAllowedExecutionTime) {
				throw GP::MaxTimeExecutionException(
				    std::string("Elapsed for the individual execution is more than allowed time"),
				    lTimeElapsed,
				    mAllowedExecutionTime
				);
			}
		}
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Empty the call stack.
	 */
	inline void emptyCallStack()
	{
		Beagle_StackTraceBeginM();
		mCallStack.clear();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return time allowed to individual evaluation, used to interrupt individuals evaluation.
	 *  \return Time allowed to execute a GP individual (in seconds).
	 */
	inline double getAllowedExecutionTime() const
	{
		Beagle_StackTraceBeginM();
		return mAllowedExecutionTime;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return value of maximum execution counter, used to interrupt individuals evaluation.
	 *  \return Maximum number of primitives allowed to be evaluated.
	 */
	inline unsigned int getAllowedNodesExecution() const
	{
		Beagle_StackTraceBeginM();
		return mAllowedNodesExecution;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return the call stack.
	 *  \return Cosntant reference to the call stack.
	 */
	inline const std::vector<unsigned int>& getCallStack() const
	{
		Beagle_StackTraceBeginM();
		return mCallStack;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return the call stack.
	 *  \return Reference to the call stack.
	 */
	inline std::vector<unsigned int>& getCallStack()
	{
		Beagle_StackTraceBeginM();
		return mCallStack;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return the nth element of the call stack.
	 *  \param inN Index of the element to return.
	 *  \return Value of the nth element.
	 *  \throw Beagle::AssertException If the index is out of bound.
	 */
	inline unsigned int getCallStackElement(unsigned int inN) const
	{
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,mCallStack.size()-1);
		return mCallStack[inN];
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return the call stack size.
	 *  \return Call stack size.
	 */
	inline unsigned int getCallStackSize() const
	{
		Beagle_StackTraceBeginM();
		return mCallStack.size();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get the top element of the call stack.
	 *  \return Top element of the call stack.
	 *  \throw Beagle::AssertException If the call stack is empty.
	 */
	inline unsigned int getCallStackTop() const
	{
		Beagle_StackTraceBeginM();
		Beagle_AssertM(mCallStack.empty() == false);
		return mCallStack.back();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Reference to context execution timer, used to time execution time of GP individuals.
	 */
	inline const PACC::Timer& getExecutionTimer() const
	{
		Beagle_StackTraceBeginM();
		return mExecutionTimer;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Reference to context execution timer, used to time execution time of GP individuals.
	 */
	inline PACC::Timer& getExecutionTimer()
	{
		Beagle_StackTraceBeginM();
		return mExecutionTimer;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a constant reference to the actual GP tree.
	 *  \return Actual GP tree constant reference.
	 */
	inline const GP::Tree& getGenotype() const
	{
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mGenotypeHandle == NULL) throw Beagle_RunTimeExceptionM(
			    "The context does not have a handle to a genotype.  Consider Beagle::Context::setGenotypeHandle().");
#endif // BEAGLE_NDEBUG
		return castObjectT<const GP::Tree&>(*mGenotypeHandle);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a reference to the actual GP tree.
	 *  \return Actual GP tree reference.
	 */
	inline GP::Tree& getGenotype()
	{
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mGenotypeHandle == NULL) throw Beagle_RunTimeExceptionM(
			    "The context does not have a handle to a genotype.  Consider Beagle::Context::setGenotypeHandle().");
#endif // BEAGLE_NDEBUG
		return castObjectT<GP::Tree&>(*mGenotypeHandle);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a handle to the actual GP tree.
	 *  \return Actual GP tree handle.
	 */
	GP::Tree::Handle getGenotypeHandle()
	{
		Beagle_StackTraceBeginM();
		return castHandleT<GP::Tree>(mGenotypeHandle);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a constant reference to the actual individual.
	 *  \return Actual individual constant reference.
	 */
	inline const GP::Individual& getIndividual() const
	{
		Beagle_StackTraceBeginM();
		return castObjectT<const GP::Individual&>(Beagle::Context::getIndividual());
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a reference to the actual GP individual.
	 *  \return Actual GP individual reference.
	 */
	inline GP::Individual& getIndividual()
	{
		Beagle_StackTraceBeginM();
		return castObjectT<GP::Individual&>(Beagle::Context::getIndividual());
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a handle to the actual GP individual.
	 *  \return Actual GP individual handle.
	 */
	inline GP::Individual::Handle getIndividualHandle()
	{
		Beagle_StackTraceBeginM();
		return castHandleT<GP::Individual>(mIndividualHandle);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return value of execution counter, used to interrupt individuals evaluation.
	 *  \return Number of nodes evaluated.
	 */
	inline unsigned int getNodesExecutionCount() const
	{
		Beagle_StackTraceBeginM();
		return mNodesExecutionCount;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Increment executed nodes counter.
	 *  \throw GP::MaxNodesExecutionException If the maximum number of nodes execution is exceeded.
	 */
	inline void incrementNodesExecuted()
	{
		Beagle_StackTraceBeginM();
		if((++mNodesExecutionCount) > mAllowedNodesExecution) {
			throw Beagle::GP::MaxNodesExecutionException(
			    std::string("Number of GP nodes executed exceeded maximum allowed"),
			    mNodesExecutionCount,
			    mAllowedNodesExecution
			);
		}
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Pop the call stack.
	 *  \throw Beagle::AssertException If the call stack is empty.
	 */
	inline void popCallStack()
	{
		Beagle_StackTraceBeginM();
		Beagle_AssertM(mCallStack.empty() == false);
		mCallStack.pop_back();
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Push a node index on the call stack.
	 *  \param inNodeIndex Node index to push.
	 */
	inline void pushCallStack(unsigned int inNodeIndex)
	{
		Beagle_StackTraceBeginM();
		mCallStack.push_back(inNodeIndex);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the execution time allowed for an individual execution.
	 *    Allowed execution time of 0 means there is no execution time limit.
	 *  \param inAllowedExecutionTime Allowed execution time (in seconds).
	 */
	inline void setAllowedExecutionTime(double inAllowedExecutionTime)
	{
		Beagle_StackTraceBeginM();
		mAllowedExecutionTime = inAllowedExecutionTime;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the limits on the number of nodes executed.
	 *  \param inAllowedNodesExecution Maximum number of nodes execution allowed.
	 */
	inline void setAllowedNodesExecution(unsigned int inAllowedNodesExecution)
	{
		Beagle_StackTraceBeginM();
		mAllowedNodesExecution = inAllowedNodesExecution;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set an handle to the actual GP tree.
	 *  \param inTreeHandle Actual GP tree handle.
	 */
	inline void setGenotypeHandle(GP::Tree::Handle inTreeHandle)
	{
		Beagle_StackTraceBeginM();
		mGenotypeHandle = inTreeHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set an handle to the actual GP individual.
	 *  \param inIndividualHandle Actual GP individual handle.
	 */
	inline void setIndividualHandle(GP::Individual::Handle inIndividualHandle)
	{
		Beagle_StackTraceBeginM();
		mIndividualHandle = inIndividualHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set number of nodes executed value.
	 *  \param inNodesExecutionCount Count of the number of nodes executed.
	 */
	inline void setNodesExecutionCount(unsigned int inNodesExecutionCount)
	{
		Beagle_StackTraceBeginM();
		mNodesExecutionCount = inNodesExecutionCount;
		Beagle_StackTraceEndM();
	}

protected:

	std::vector<unsigned int> mCallStack;             //!< The GP execution call stack.
	PACC::Timer               mExecutionTimer;        //!< Individual execution timer.
	double                    mAllowedExecutionTime;  //!< Allowed execution time for ind. execution.
	unsigned int              mNodesExecutionCount;   //!< Count the number of GP nodes executed.
	unsigned int              mAllowedNodesExecution; //!< Maximum allowed of GP nodes execution.

};

}
}

#endif // Beagle_GP_Context_hpp
