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
 *  \file   Beagle/Core/RouletteT.hpp
 *  \brief  Definition of class template RouletteT.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.13 $
 *  $Date: 2007/09/14 20:19:50 $
 */

#ifndef Beagle_Core_RouletteT_hpp
#define Beagle_Core_RouletteT_hpp

#include <vector>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/Randomizer.hpp"


namespace Beagle
{

/*!
 *  \class RouletteT Beagle/Core/RouletteT.hpp "Beagle/Core/RouletteT.hpp"
 *  \brief Templated class defining a T-type roulette wheel.
 *  \param T Type of the values in the wheel.
 *  \ingroup Core
 *  \ingroup Utils
 *  \author Matthew Walker
 *  \author Christian Gagne
 */
template <class T>
class RouletteT : public Beagle::Object, public std::vector< std::pair<double,T> >
{

public:

	typedef std::vector< std::pair<double,T> > VectorType;

	//! RouletteT allocator type.
	typedef AllocatorT<RouletteT<T>,Object::Alloc> Alloc;
	//! RouletteT handle type.
	typedef PointerT<RouletteT<T>,Object::Handle> Handle;
	//! RouletteT bag type.
	typedef ContainerT<RouletteT<T>,Object::Bag> Bag;

	RouletteT()
	{ }
	virtual ~RouletteT()
	{ }

	/*!
	 *  \brief Insert a value into the roulette wheel.
	 *  \param inValue The value to be inserted.
	 *  \param inWeight The weighting to associate with the value.
	 */
	inline void insert(const T& inValue, double inWeight=1.0) {
		Beagle_StackTraceBeginM();
		Beagle_AssertM(inWeight>=0.0);
		if(VectorType::empty()==false) inWeight += VectorType::back().first;
		push_back(std::make_pair(inWeight,inValue));
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Optimize distribution of values in the roulette.
	 */
	inline void optimize() {
		Beagle_StackTraceBeginM();
		if(VectorType::size()<2) return;
		for(unsigned int i=(VectorType::size()-1); i>=1; --i)
			(*this)[i].first -= (*this)[i-1].first;
		std::sort(VectorType::begin(), VectorType::end(), std::greater< std::pair<double,T> >());
		for(unsigned int i=1; i<VectorType::size(); ++i)
			(*this)[i].first += (*this)[i-1].first;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Select a value from the roulette wheel
	 *  \param ioRandomizer Randomizer used to select element.
	 */
	inline const T& select(Beagle::Randomizer& ioRandomizer) const {
		Beagle_StackTraceBeginM();
		Beagle_AssertM(VectorType::empty()==false);
		if(VectorType::back().first == 0.) {
			const unsigned int lDice = ioRandomizer.rollInteger(0, VectorType::size()-1);
			return (*this)[lDice].second;
		}
		const double lDice = ioRandomizer.rollUniform(0., VectorType::back().first);
		unsigned int i;
		for(i=0; i<(VectorType::size()-1); ++i) if(lDice<(*this)[i].first) break;
		Beagle_AssertM(lDice<(*this)[i].first);
		return (*this)[i].second;
		Beagle_StackTraceEndM();
	}

};

}


#endif // Beagle_Core_RouletteT_hpp
