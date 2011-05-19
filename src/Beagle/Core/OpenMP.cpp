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
 *  \file   Beagle/Core/OpenMP.cpp
 *  \brief  Source code of class OpenMP.
 *	\author Francois-Michel De Rainville
 *	\author Felix-Antoine Fortin
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: $
 *  $Date: $
 */

#include "Beagle/Core.hpp"


//!	\brief Construct the OpenMP component.
Beagle::OpenMP::OpenMP() :
	Beagle::Component("OpenMP"), mNumThreads(NULL)
{}

//!	\brief Initialize the OpenMP component.
void Beagle::OpenMP::init(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
#if defined(BEAGLE_USE_OMP_R)
	omp_set_dynamic(0);
	setNumThreads(mNumThreads->getWrappedValue());
#endif
	Beagle_StackTraceEndM();
}

void Beagle::OpenMP::readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
#if defined(BEAGLE_USE_OMP_R)
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="OpenMP"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <OpenMP> expected!");

	std::string lNumThreads = inIter->getAttribute("threads");
	if(lNumThreads.empty())
		throw Beagle_IOExceptionNodeM(*inIter, "expected number of threads!");

	mNumThreads->getWrappedValue() = Beagle::str2uint(lNumThreads);
#endif
	Beagle_StackTraceEndM();
}

//!	\brief Register the parameters of the OpenMP component.
void Beagle::OpenMP::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Beagle::Component::registerParams(ioSystem);
#if defined(BEAGLE_USE_OMP_R)
	const unsigned int lNumThreads = getMaxNumThreads();
	Beagle::Register::Description lDescription(
	    "Number of threads to use in parallel sections.",
	    "UInt",
	    uint2str(lNumThreads),
	    std::string("Selection, Crossover, Mutation and Evaluation are paralellized with OpenMP, this ") +
	    std::string("variable set the number of threads to use. In order to produce reproductible ") +
	    std::string("results, this variable may be set to any number even if the machine used have not ") +
	    std::string("this amount of processors. If not set, the default value is the number of processors.")
	);
	mNumThreads = Beagle::castHandleT<Beagle::UInt>(
	                  ioSystem.getRegister().insertEntry("ec.omp.threads", new Beagle::UInt(lNumThreads), lDescription));
#endif
	Beagle_StackTraceEndM();
}

void Beagle::OpenMP::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent /*=true*/) const
{
	Beagle_StackTraceBeginM();
#if defined(BEAGLE_USE_OMP_R)
	ioStreamer.insertAttribute("threads", Beagle::uint2str(getMaxNumThreads()));
#endif
	Beagle_StackTraceEndM();
}
