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
 *  \file   Beagle/EC/CrossoverOnePointOpT.hpp
 *  \brief  Definition of the template class CrossoverOnePointOpT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#ifndef Beagle_EC_CrossoverOnePointOpT_hpp
#define Beagle_EC_CrossoverOnePointOpT_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC/CrossoverOp.hpp"

namespace Beagle
{

namespace EC
{

/*!
 *  \class CrossoverOnePointOpT Beagle/EC/CrossoverOnePointOpT.hpp
 *    "Beagle/EC/CrossoverOnePointOpT.hpp"
 *  \brief One point crossover generic operator class.
 *  \param T Representation type used in crossover.
 *  \ingroup EC
 *
 *  The genotype type T must fullfil the std::vector template interface.
 */
template <class T>
class CrossoverOnePointOpT : public Beagle::EC::CrossoverOp
{

public:

	//! CrossoverOnePointOpT allocator type.
	typedef Beagle::AllocatorT<CrossoverOnePointOpT<T>,Beagle::EC::CrossoverOp::Alloc>
	Alloc;
	//! CrossoverOnePointOpT handle type.
	typedef Beagle::PointerT<CrossoverOnePointOpT<T>,Beagle::EC::CrossoverOp::Handle>
	Handle;
	//! CrossoverOnePointOpT bag type.
	typedef Beagle::ContainerT<CrossoverOnePointOpT<T>,Beagle::EC::CrossoverOp::Bag>
	Bag;

	explicit CrossoverOnePointOpT(std::string inMatingPbName="ec.cx1p.prob",
	                              std::string inName="CrossoverOnePointOpT");
	virtual ~CrossoverOnePointOpT()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool mate(Beagle::Individual& ioIndiv1, Beagle::Context& ioContext1,
	                  Beagle::Individual& ioIndiv2, Beagle::Context& ioContext2);

};

}

}


/*!
 *  \brief Construct an one point crossover operator.
 *  \param inMatingPbName Mating probability parameter name.
 *  \param inName Name of the operator.
 */
template <class T>
Beagle::EC::CrossoverOnePointOpT<T>::CrossoverOnePointOpT(std::string inMatingPbName,
        std::string inName) :
	Beagle::EC::CrossoverOp(inMatingPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the GA one point crossover operator.
 *  \param ioSystem System of the evolution.
 */
template <class T>
void Beagle::EC::CrossoverOnePointOpT<T>::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lProbaDescription(
		    "Individual 1-point crossover pb.",
		    "Double",
		    "0.3",
		    "GA one-point crossover probability of a single individual."
		);
		mMatingProba = castHandleT<Double>(
		                   ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.3f), lProbaDescription));
	}
	EC::CrossoverOp::registerParams(ioSystem);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Mate two GA individuals for one point crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
template <class T>
bool Beagle::EC::CrossoverOnePointOpT<T>::mate(Beagle::Individual& ioIndiv1,
        Beagle::Context&    ioContext1,
        Beagle::Individual& ioIndiv2,
        Beagle::Context&    ioContext2)
{
	Beagle_StackTraceBeginM();
	unsigned int lNbGenotypes = minOf<unsigned int>(ioIndiv1.size(), ioIndiv2.size());
	if(lNbGenotypes == 0) return false;
	if(lNbGenotypes == 1) {
		typename T::Handle lGenotype1 = castHandleT<T>(ioIndiv1[0]);
		typename T::Handle lGenotype2 = castHandleT<T>(ioIndiv2[0]);
		unsigned int lSize = minOf<unsigned int>(lGenotype1->size(), lGenotype2->size());
		if(lSize < 2) return false;
		unsigned int lMatingPoint = ioContext1.getSystem().getRandomizer().rollInteger(1, (lSize-1));

		Beagle_LogDebugM(ioContext1.getSystem().getLogger(), *lGenotype1);
		Beagle_LogDebugM(ioContext1.getSystem().getLogger(), *lGenotype2);
		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "Mating individuals before the " << uint2ordinal(lMatingPoint+1) << " element"
		);

		for(unsigned int i=0; i<lMatingPoint; ++i) {
			typename T::value_type lTemp = (*lGenotype1)[i];
			(*lGenotype1)[i] = (*lGenotype2)[i];
			(*lGenotype2)[i] = lTemp;
		}

		Beagle_LogDebugM(ioContext1.getSystem().getLogger(), *lGenotype1);
		Beagle_LogDebugM(ioContext1.getSystem().getLogger(), *lGenotype2);
	} else {
		unsigned int lTotalSize = 0;
		std::vector<unsigned int> lSizes;
		for(unsigned int i=0; i<lNbGenotypes; i++) {
			typename T::Handle lGenotype1 = castHandleT<T>(ioIndiv1[i]);
			typename T::Handle lGenotype2 = castHandleT<T>(ioIndiv2[i]);
			unsigned int lSizeI = minOf<unsigned int>(lGenotype1->size(), lGenotype2->size());
			lSizes.push_back(lSizeI);
			lTotalSize += lSizeI;
		}
		if(lTotalSize < 2) return false;
		unsigned int lMatingPoint =
		    ioContext1.getSystem().getRandomizer().rollInteger(1, (lTotalSize-1));
		unsigned int lMatingGenotype = 0;
		for(; (lMatingGenotype+1)<lNbGenotypes; lMatingGenotype++) {
			if(lMatingPoint < lSizes[lMatingGenotype]) break;
			else lMatingPoint -= lSizes[lMatingGenotype];
		}

		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "Mating the " << uint2ordinal(lMatingGenotype+1) <<
		    " genotypes before the " << uint2ordinal(lMatingPoint+1) << " element"
		);

		typename T::Handle lGenotype1 = castHandleT<T>(ioIndiv1[lMatingGenotype]);
		typename T::Handle lGenotype2 = castHandleT<T>(ioIndiv2[lMatingGenotype]);

		Beagle_LogDebugM(ioContext1.getSystem().getLogger(), *lGenotype1);
		Beagle_LogDebugM(ioContext1.getSystem().getLogger(), *lGenotype2);

		for(unsigned int i=0; i<lMatingPoint; ++i) {
			typename T::value_type lTemp = (*lGenotype1)[i];
			(*lGenotype1)[i] = (*lGenotype2)[i];
			(*lGenotype2)[i] = lTemp;
		}

		Beagle_LogDebugM(ioContext1.getSystem().getLogger(), *lGenotype1);
		Beagle_LogDebugM(ioContext1.getSystem().getLogger(), *lGenotype2);
	}

	return true;
	Beagle_StackTraceEndM();

}

#endif // Beagle_EC_CrossoverOnePointOpT_hpp
