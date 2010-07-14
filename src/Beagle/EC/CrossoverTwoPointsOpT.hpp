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
 *  \file   Beagle/EC/CrossoverTwoPointsOpT.hpp
 *  \brief  Definition of the template class CrossoverTwoPointsOpT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#ifndef Beagle_EC_CrossoverTwoPointsOpT_hpp
#define Beagle_EC_CrossoverTwoPointsOpT_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC/CrossoverOp.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class CrossoverTwoPointsOpT Beagle/EC/CrossoverTwoPointsOpT.hpp
 *    "Beagle/EC/CrossoverTwoPointsOpT.hpp"
 *  \brief Generic two points crossover operator class.
 *  \param T Representation type used in crossover.
 *  \ingroup EC
 *
 *  The genotype type T must fullfil the std::vector template interface.
 */
template <class T>
class CrossoverTwoPointsOpT : public Beagle::EC::CrossoverOp
{

public:

	//! CrossoverTwoPointsOpT allocator type.
	typedef Beagle::AllocatorT<CrossoverTwoPointsOpT<T>,Beagle::EC::CrossoverOp::Alloc>
	Alloc;
	//! CrossoverTwoPointsOpT handle type.
	typedef Beagle::PointerT<CrossoverTwoPointsOpT<T>,Beagle::EC::CrossoverOp::Handle>
	Handle;
	//! CrossoverTwoPointsOpT bag type.
	typedef Beagle::ContainerT<CrossoverTwoPointsOpT<T>,Beagle::EC::CrossoverOp::Bag>
	Bag;

	explicit CrossoverTwoPointsOpT(std::string inMatingPbName="ec.cx2p.prob",
	                               std::string inName="CrossoverTwoPointsOpT");
	virtual ~CrossoverTwoPointsOpT()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool mate(Beagle::Individual& ioIndiv1, Beagle::Context& ioContext1,
	                  Beagle::Individual& ioIndiv2, Beagle::Context& ioContext2);

};

}

}


/*!
 *  \brief Construct a two points crossover operator.
 *  \param inMatingPbName Mating probability parameter name.
 *  \param inName Name of the operator.
 */
template <class T>
Beagle::EC::CrossoverTwoPointsOpT<T>::CrossoverTwoPointsOpT(std::string inMatingPbName,
        std::string inName) :
		Beagle::EC::CrossoverOp(inMatingPbName, inName)
{ }


/*!
 *  \brief Register the arameters of the two points crossover operator.
 *  \param ioSystem System of the evolution.
 */
template <class T>
void Beagle::EC::CrossoverTwoPointsOpT<T>::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
		    "Individual 2-points crossover pb.",
		    "Double",
		    "0.3",
		    "GA two-points crossover probability of a single individual."
		);
		mMatingProba = castHandleT<Double>(
		                   ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.3f), lDescription));
	}
	Beagle::EC::CrossoverOp::registerParams(ioSystem);
	Beagle_StackTraceEndM("void GA::CrossoverTwoPointsOpT<T>::registerParams(System& ioSystem)");
}


/*!
 *  \brief Mate two individuals for two points crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
template <class T>
bool Beagle::EC::CrossoverTwoPointsOpT<T>::mate(Beagle::Individual& ioIndiv1,
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
		unsigned int lMatingPoint1 = ioContext1.getSystem().getRandomizer().rollInteger(1, lSize);
		unsigned int lMatingPoint2 = ioContext1.getSystem().getRandomizer().rollInteger(1, (lSize-1));
		if(lMatingPoint2 >= lMatingPoint1) ++lMatingPoint2;
		else {
			unsigned int lTemp = lMatingPoint1;
			lMatingPoint1 = lMatingPoint2;
			lMatingPoint2 = lTemp;
		}

		Beagle_LogDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    "Genotypes mated (before GA two points crossover)"
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    *lGenotype1
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    *lGenotype2
		);
		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    std::string("Mating individuals after the ")+
		    uint2ordinal(lMatingPoint1+1)+std::string(" element and before the")+
		    uint2ordinal(lMatingPoint2+1)+std::string(" element")
		);

		for(unsigned int i=lMatingPoint1; i<lMatingPoint2; ++i) {
			typename T::value_type lTemp = (*lGenotype1)[i];
			(*lGenotype1)[i] = (*lGenotype2)[i];
			(*lGenotype2)[i] = lTemp;
		}

		Beagle_LogDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    "Genotypes mated (after GA two points crossover)"
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    *lGenotype1
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    *lGenotype2
		);
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
		unsigned int lMatingPoint1 =
		    ioContext1.getSystem().getRandomizer().rollInteger(1, lTotalSize);
		unsigned int lMatingGenotype = 0;
		for(; (lMatingGenotype+1)<lNbGenotypes; lMatingGenotype++) {
			if(lMatingPoint1 <= lSizes[lMatingGenotype]) break;
			else lMatingPoint1 -= lSizes[lMatingGenotype];
		}
		typename T::Handle lGenotype1 = castHandleT<T>(ioIndiv1[lMatingGenotype]);
		typename T::Handle lGenotype2 = castHandleT<T>(ioIndiv2[lMatingGenotype]);
		unsigned int lSize = minOf<unsigned int>(lGenotype1->size(), lGenotype2->size());
		if(lSize < 2) return false;
		unsigned int lMatingPoint2 = ioContext1.getSystem().getRandomizer().rollInteger(1, (lSize-1));
		if(lMatingPoint2 >= lMatingPoint1) ++lMatingPoint2;
		else {
			unsigned int lTemp = lMatingPoint1;
			lMatingPoint1 = lMatingPoint2;
			lMatingPoint2 = lTemp;
		}

		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    std::string("Mating the ")+uint2ordinal(lMatingGenotype+1)+
		    std::string(" genotypes after the ")+uint2ordinal(lMatingPoint1+1)+
		    std::string(" element and before the ")+uint2ordinal(lMatingPoint2+1)+
		    std::string(" element")
		);

		Beagle_LogDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    "Genotypes mated (before GA two points crossover)"
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    *lGenotype1
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    *lGenotype2
		);

		for(unsigned int i=lMatingPoint1; i<lMatingPoint2; ++i) {
			typename T::value_type lTemp = (*lGenotype1)[i];
			(*lGenotype1)[i] = (*lGenotype2)[i];
			(*lGenotype2)[i] = lTemp;
		}

		Beagle_LogDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    "Genotypes mated (after GA two points crossover)"
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    *lGenotype1
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverTwoPointsOpT",
		    *lGenotype2
		);
	}

	return true;
	Beagle_StackTraceEndM("bool GA::CrossoverTwoPointsOpT<T>::mate(Individual& ioIndiv1,Context& ioContext1, Individual& ioIndiv2, Context& ioContext2)");
}


#endif // Beagle_EC_CrossoverTwoPointsOpT_hpp
