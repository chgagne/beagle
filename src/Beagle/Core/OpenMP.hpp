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
 *  \file   Beagle/Core/OpenMP.hpp
 *  \brief  Definition of the class OpenMP.
 *	\author Francois-Michel De Rainville
 *	\author Felix-Antoine Fortin
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_Core_OpenMp_hpp
#define Beagle_Core_OpenMp_hpp

#include <omp.h>

#include "Beagle/Core.hpp"

namespace Beagle
{

/*!
 *  \class OpenMP Beagle/Core/OpenMP.hpp "Beagle/Core/OpenMP.hpp"
 *  \brief OpenMP function wrapper class.
 *  \ingroup Core
 *  \ingroup Sys
 */
class OpenMP : public Component
{

public:

	//! OpenMP Component allocator type.
	typedef AllocatorT<OpenMP,Component::Alloc> Alloc;

	//! OpenMP Component handle type.
	typedef PointerT<OpenMP,Component::Handle> Handle;

	//! OpenMP Component bag type.
	typedef ContainerT<OpenMP,Component::Bag> Bag;

	explicit OpenMP ();
	virtual ~OpenMP ()
	{ }

	virtual void init(System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void registerParams(System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!	\brief
	 *	\return
	 */
	inline unsigned int getNumThreads() const {
		Beagle_StackTraceBeginM();
		return omp_get_num_threads();
		Beagle_StackTraceEndM();
	}

	/*!	\brief
	 *	\return
	 */
	inline unsigned int getMaxNumThreads() const {
		Beagle_StackTraceBeginM();
#if defined(BEAGLE_USE_OMP_NR)
		return omp_get_max_threads();
#elif defined(BEAGLE_USE_OMP_R)
		if(mNumThreads == NULL)
			return omp_get_max_threads();
		return mNumThreads->getWrappedValue();
#endif
		Beagle_StackTraceEndM();
	}

	/*!	\brief
	 *	\return
	 */
	inline unsigned int getThreadNum() const {
		Beagle_StackTraceBeginM();
		return omp_get_thread_num();
		Beagle_StackTraceEndM();
	}

	/*!	\brief Ask OpenMP how many processors are available.
	 *	\return The number of processors on this machine.
	 */
	inline unsigned int getNumProcs() const {
		Beagle_StackTraceBeginM();
		return omp_get_num_procs();
		Beagle_StackTraceEndM();
	}

	/*!	\brief
	 *	\return
	 */
	inline bool	inParallelSection() const {
		Beagle_StackTraceBeginM();
		return (omp_in_parallel()>0)?true:false;
		Beagle_StackTraceEndM();
	}

protected:
	/*!	\brief Set the number of threads that OpenMP will use in the parallel sections.
	 *	\param iNumThreads is the number of threads to use.
	 */
	inline virtual void	setNumThreads(unsigned int inNumThreads) {
		Beagle_StackTraceBeginM();
		omp_set_num_threads(inNumThreads);
		Beagle_StackTraceEndM();
	}

	UInt::Handle mNumThreads;
};

}

#endif // Beagle_Core_OpenMP_hpp
