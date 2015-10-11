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
 *  \file   beagle/GP/MutationEphemeralOpT.hpp
 *  \brief  Definition and implementation of template GP::MutationEphemeralOpT.
 *  \author Christian Gagne
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_GP_MutationEphemeralOpT_hpp
#define Beagle_GP_MutationEphemeralOpT_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Float.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Individual.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/EphemeralT.hpp"

namespace Beagle
{
namespace GP
{


/*!
 *  \class MutationEphemeralOpT beagle/GP/MutationEphemeralOpT.hpp
 *    "beagle/GP/MutationEphemeralOpT.hpp"
 *  \brief GP ephemeral mutation operator class, change value of ephemeral constants.
 *  \param T Type of the constants.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
template <class T>
class MutationEphemeralOpT : public Beagle::MutationOp
{

public:

	//! GP::MutationEphemeralOpT allocator type.
	typedef AllocatorT<Beagle::GP::MutationEphemeralOpT<T>,Beagle::MutationOp::Alloc>
	Alloc;
	//! GP::MutationEphemeralOpT handle type.
	typedef PointerT<Beagle::GP::MutationEphemeralOpT<T>,Beagle::MutationOp::Handle>
	Handle;
	//! GP::MutationEphemeralOpT bag type.
	typedef ContainerT<Beagle::GP::MutationEphemeralOpT<T>,Beagle::MutationOp::Bag>
	Bag;

	explicit MutationEphemeralOpT(std::string inMutationPbName="gp.muteph.indpb",
	                              std::string inEphemeralNameParamName="gp.muteph.primit",
	                              std::string inName="GP-MutationEphemeralOpT");
	virtual ~MutationEphemeralOpT()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	String::Handle mEphemeralName;           //!< Name of the ephemeral primitive mutated.
	std::string    mEphemeralNameParamName;  //!< Name of the ephemeral primitive name parameter.

};

}
}


/*!
 *  \brief Construct a GP ephemeral mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inEphemeralNameParamName Parameter name used to state the name of the ephemeral primitive
 *    that are mutated.
 *  \param inName Name of the operator.
 */
template <class T>
Beagle::GP::MutationEphemeralOpT<T>::MutationEphemeralOpT(std::string inMutationPbName,
        std::string inEphemeralNameParamName,
        std::string inName) :
		Beagle::MutationOp(inMutationPbName, inName),
		mEphemeralNameParamName(inEphemeralNameParamName)
{ }


/*!
 *  \brief Register the parameters of the GP ephemeral mutation operator.
 *  \param ioSystem System of the evolution.
 */
template <class T>
void Beagle::GP::MutationEphemeralOpT<T>::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();

	{
		std::ostringstream lOSS;
		lOSS << "Ephemeral mutation probability for an individual. ";
		lOSS << "A ephemeral mutation change the value of a specific primitive.";
		Register::Description lDescription(
		    "Ephemeral mutation probability",
		    "Double",
		    "0.05",
		    lOSS.str()
		);
		mMutationProba = castHandleT<Double>(
		                     ioSystem.getRegister().insertEntry(mMutationPbName, new Double(0.05f), lDescription));
	}
	Beagle::MutationOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Name of the ephemeral primitive for which the values are modified ";
		lOSS << "by the ephemeral mutation operator.";
		Register::Description lDescription(
		    "Ephemeral primitive name",
		    "String",
		    "E",
		    lOSS.str()
		);
		mEphemeralName = castHandleT<String>(
		                     ioSystem.getRegister().insertEntry(mEphemeralNameParamName, new String("E"), lDescription));
	}
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Mutating ephemerals values of a GP individual.
 *  \param ioIndividual GP individual to mutate ephemerals value.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
template <class T>
bool Beagle::GP::MutationEphemeralOpT<T>::mutate(Beagle::Individual& ioIndividual,
        Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Beagle_LogDetailedM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationEphemeralOpT",
	    std::string("Mutating ")+uint2ordinal(ioContext.getGenotypeIndex()+1)+
	    std::string(" individual with GP::MutationEphemeralOpT")
	);

	GP::Individual& lIndividual = castObjectT<GP::Individual&>(ioIndividual);
	GP::Context& lContext = castObjectT<GP::Context&>(ioContext);

	// Get index of potential primitives with parameters that can be selected for mutation.
	std::vector< std::pair<unsigned int,unsigned int> > lPotentialParam;
	for(unsigned int i=0; i<lIndividual.size(); ++i) {
		GP::Tree& lTree = *lIndividual[i];
		for(unsigned int j=0; j<lTree.size(); ++j) {
			if(lTree[j].mPrimitive->getName() == *mEphemeralName) {
				lPotentialParam.push_back(std::make_pair(i,j));
			}
		}
	}

	// Return if there is not potential parameters.
	if(lPotentialParam.empty()) return false;

	// Mutating a primitive
	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationEphemeralOpT",
	    "Individual before GP parameters mutation"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationEphemeralOpT",
	    ioIndividual
	);

	// Store original context values
	unsigned int lOldGenotypeIndex = lContext.getGenotypeIndex();
	GP::Tree::Handle lOldGenotypeHandle = lContext.getGenotypeHandle();

	// Get reference to primitive to mutate and other objects.
	const unsigned int lSelectedParam =
	    lContext.getSystem().getRandomizer().rollInteger(0,lPotentialParam.size()-1);
	GP::Tree::Handle lSelectedTree = lIndividual[lPotentialParam[lSelectedParam].first];
	lContext.setGenotypeIndex(lPotentialParam[lSelectedParam].first);
	lContext.setGenotypeHandle(lSelectedTree);
	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationEphemeralOpT",
	    std::string("Mutating the parameter of the ")+
	    uint2ordinal(lPotentialParam[lSelectedParam].second+1)+
	    std::string(" node in the ")+uint2ordinal(lPotentialParam[lSelectedParam].first+1)+
	    std::string(" tree")
	);

	// Mutate parameter value.
	GP::Primitive::Handle lSelectedPrimit =
	    (*lSelectedTree)[lPotentialParam[lSelectedParam].second].mPrimitive;
	typename GP::EphemeralT<T>::Handle lSelectedEphemeral =
	    castHandleT<typename GP::EphemeralT<T> >(lSelectedPrimit);
	GP::Primitive::Handle lGeneratedPrimit =
	    lSelectedEphemeral->generate(mEphemeralName->getWrappedValue(), lContext);
	(*lSelectedTree)[lPotentialParam[lSelectedParam].second].mPrimitive = lGeneratedPrimit;

	Beagle_LogVerboseM(
	    ioContext.getSystem().getLogger(),
	    "mutation", "Beagle::GP::MutationEphemeralOpT",
	    std::string("Changing the ephemeral from ")+lSelectedPrimit->serialize()+
	    std::string(" to ")+lGeneratedPrimit->serialize()
	);

	// Restore original context values
	lContext.setGenotypeIndex(lOldGenotypeIndex);
	lContext.setGenotypeHandle(lOldGenotypeHandle);

	Beagle_LogDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationEphemeralOpT",
	    "Individual after GP parameters mutation"
	);
	Beagle_LogObjectDebugM(
	    ioContext.getSystem().getLogger(),
	    "mutation",
	    "Beagle::GP::MutationEphemeralOpT",
	    ioIndividual
	);

	return true;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Read a mutation operator from XML subtree.
 *  \param inIter XML iterator to use to read crossover operator.
 *  \param ioSystem Evolutionary system.
 */
template <class T>
void Beagle::GP::MutationEphemeralOpT<T>::readWithSystem(PACC::XML::ConstIterator inIter,
        Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName())) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected!" << std::flush;
		throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
	}
	std::string lMutationPbReadName = inIter->getAttribute("mutationpb");
	if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
	std::string lEphemeralNameParamName = inIter->getAttribute("primitname");
	if(lEphemeralNameParamName.empty() == false) mEphemeralNameParamName = lEphemeralNameParamName;
	Beagle_StackTraceEndM();
}


/*!
 *  \brief Write mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
template <class T>
void Beagle::GP::MutationEphemeralOpT<T>::writeContent(PACC::XML::Streamer& ioStreamer,
        bool inIndent) const
{
	Beagle_StackTraceBeginM();
	Beagle::MutationOp::writeContent(ioStreamer, inIndent);
	ioStreamer.insertAttribute("primitname", mEphemeralNameParamName);
	Beagle_StackTraceEndM();
}


#endif // Beagle_GP_MutationEphemeralOpT_hpp
