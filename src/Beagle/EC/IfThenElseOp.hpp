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
 *  \file   Beagle/EC/IfThenElseOp.hpp
 *  \brief  Definition of the class IfThenElseOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_EC_IfThenElseOp_hpp
#define Beagle_EC_IfThenElseOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class IfThenElseOp Beagle/EC/IfThenElseOp.hpp "Beagle/EC/IfThenElseOp.hpp"
 *  \brief If-then-else operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class IfThenElseOp : public Beagle::Operator
{

public:

	//! IfThenElseOp allocator type
	typedef Beagle::AllocatorT<IfThenElseOp,Beagle::Operator::Alloc>
	Alloc;
	//! IfThenElseOp handle type.
	typedef Beagle::PointerT<IfThenElseOp,Beagle::Operator::Handle>
	Handle;
	//! IfThenElseOp bag type.
	typedef Beagle::ContainerT<IfThenElseOp,Beagle::Operator::Bag>
	Bag;

	explicit IfThenElseOp(std::string inConditionTag="",
	                      std::string inConditionValue="",
	                      std::string inName="IfThenElseOp");
	virtual ~IfThenElseOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	virtual void init(Beagle::System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \return Parameter tag of condition to test in if-then-else operator.
	 */
	inline std::string getConditionTag() const {
		Beagle_StackTraceBeginM();
		return mConditionTag;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Condition value tested for positive condition in if-then-else operator.
	 */
	inline std::string getConditionValue() const {
		Beagle_StackTraceBeginM();
		return mConditionValue;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a constant reference to the positive operator set.
	 *  \return Positive operator set constant reference.
	 */
	inline const Beagle::Operator::Bag& getPositiveSet() const {
		Beagle_StackTraceBeginM();
		return mPositiveOpSet;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a reference to the positive operator set.
	 *  \return Positive operator set reference.
	 */
	inline Beagle::Operator::Bag& getPositiveSet() {
		Beagle_StackTraceBeginM();
		return mPositiveOpSet;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a constant reference to the negative operator set.
	 *  \return Negative operator set constant reference.
	 */
	inline const Beagle::Operator::Bag& getNegativeSet() const {
		Beagle_StackTraceBeginM();
		return mNegativeOpSet;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a reference to the negative operator set.
	 *  \return Negative operator set reference.
	 */
	inline Beagle::Operator::Bag& getNegativeSet() {
		Beagle_StackTraceBeginM();
		return mNegativeOpSet;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Insert an operator in the positive set of the if-then-else operator.
	 *  \param inOperator Operator to insert.
	 *  \warning Insert your operators before the initialization!
	 */
	inline void insertPositiveOp(Beagle::Operator::Handle inOperator) {
		Beagle_StackTraceBeginM();
		mPositiveOpSet.push_back(inOperator);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Insert an operator in the negative set of the if-then-else operator.
	 *  \param inOperator Operator to insert.
	 *  \warning Insert your operators before the initialization!
	 */
	void insertNegativeOp(Beagle::Operator::Handle inOperator) {
		Beagle_StackTraceBeginM();
		mNegativeOpSet.push_back(inOperator);
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set parameter tag of condition to use.
	 */
	inline void setConditionTag(std::string inTag) {
		Beagle_StackTraceBeginM();
		mConditionTag = inTag;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set parameter value of condition to use.
	 */
	inline void setConditionValue(std::string inValue) {
		Beagle_StackTraceBeginM();
		mConditionValue = inValue;
		Beagle_StackTraceEndM();
	}

private:

	Beagle::Operator::Bag  mPositiveOpSet;   //!< Operator set to execute when the condition is true.
	Beagle::Operator::Bag  mNegativeOpSet;   //!< Operator set to execute when the condition is false.
	std::string            mConditionTag;    //!< Parameter tag to test.
	std::string            mConditionValue;  //!< Parameter value for a positive condition.

};

}

}

#endif // Beagle_EC_IfThenElseOp_hpp
