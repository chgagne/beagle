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
 *  \file   Beagle/Core/Context.hpp
 *  \brief  Definition of the class Context.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_Core_Context_hpp
#define Beagle_Core_Context_hpp

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/Genotype.hpp"
#include "Beagle/Core/Individual.hpp"
#include "Beagle/Core/Deme.hpp"
#include "Beagle/Core/Vivarium.hpp"
#include "Beagle/Core/System.hpp"
#include "Beagle/Core/Evolver.hpp"
#include "Beagle/Core/RunTimeException.hpp"

namespace Beagle
{

/*!
 *  \class Context Beagle/Core/Context.hpp "Beagle/Core/Context.hpp"
 *  \brief Evolutionary context.
 *  \ingroup Core
 *  \ingroup Sys
 */
class Context : public Object
{

public:

	//! Context allocator type.
	typedef AllocatorT<Context,Object::Alloc>
	Alloc;
	//! Context handle type.
	typedef PointerT<Context,Object::Handle>
	Handle;
	//! Context bag type.
	typedef ContainerT<Context,Object::Bag>
	Bag;

	Context();
	virtual ~Context()
	{ }

	/*!
	 *  \brief Return the continuing flag value.
	 *  \return Continuing flag value.
	 */
	inline bool getContinueFlag() const {
		Beagle_StackTraceBeginM();
		return mContinueFlag;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a constant reference to the actual deme.
	 *  \return Actual deme constant reference.
	 */
	inline const Deme& getDeme() const {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mDemeHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to a deme. ")+
			    std::string("Consider Beagle::Context::setDemeHandle() and setDemeIndex()."));
#endif // BEAGLE_NDEBUG
		return *mDemeHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a reference to the actual deme.
	 *  \return Actual deme reference.
	 */
	inline Deme& getDeme() {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mDemeHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to a deme. ")+
			    std::string("Consider Beagle::Context::setDemeHandle() and setDemeIndex()."));
#endif // BEAGLE_NDEBUG
		return *mDemeHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a handle to the actual deme.
	 *  \return Actual deme handle.
	 */
	inline Deme::Handle getDemeHandle() {
		Beagle_StackTraceBeginM();
		return mDemeHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return the actual deme index.
	 *  \return Actual deme index.
	 */
	inline unsigned int getDemeIndex() const {
		Beagle_StackTraceBeginM();
		return mDemeIndex;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a constant reference to the actual evolver.
	 *  \return Actual evolver constant reference.
	 */
	inline const Evolver& getEvolver() const {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mEvolverHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to an evolver. ")+
			    std::string("Consider Beagle::Context::setEvolverHandle()."));
#endif // BEAGLE_NDEBUG
		return *mEvolverHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a reference to the actual evolver.
	 *  \return Actual evolver reference.
	 */
	inline Evolver& getEvolver() {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mEvolverHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to an evolver. ")+
			    std::string("Consider Beagle::Context::setEvolverHandle()."));
#endif // BEAGLE_NDEBUG
		return *mEvolverHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a handle to the actual evolver.
	 *  \return Actual evolver handle.
	 */
	inline Evolver::Handle getEvolverHandle() {
		Beagle_StackTraceBeginM();
		return mEvolverHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a constant reference to the actual individual.
	 *  \return Actual individual constant reference.
	 */
	inline const Individual& getIndividual() const {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mIndividualHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to an individual. ")+
			    std::string("Consider Beagle::Context::setIndividualHandle() and setIndividualIndex()."));
#endif // BEAGLE_NDEBUG
		return *mIndividualHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a reference to the actual individual.
	 *  \return Actual individual reference.
	 */
	inline Individual& getIndividual() {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mIndividualHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to an individual. ")+
			    std::string("Consider Beagle::Context::setIndividualHandle() and setIndividualIndex()."));
#endif // BEAGLE_NDEBUG
		return *mIndividualHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a handle to the actual individual.
	 *  \return Actual individual handle.
	 */
	inline Individual::Handle getIndividualHandle() {
		Beagle_StackTraceBeginM();
		return mIndividualHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return the actual individual index.
	 *  \return Actual individual index.
	 */
	inline unsigned int getIndividualIndex() const {
		Beagle_StackTraceBeginM();
		return mIndividualIndex;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a constant reference to the actual genotype.
	 *  \return Actual genotype constant reference.
	 */
	inline const Genotype& getGenotype() const {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mGenotypeHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to a genotype. ")+
			    std::string("Consider Beagle::Context::setGenotypeHandle()."));
#endif // BEAGLE_NDEBUG
		return *mGenotypeHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a reference to the actual genotype.
	 *  \return Actual genotype reference.
	 */
	inline Genotype& getGenotype() {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mGenotypeHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to a genotype. ")+
			    std::string("Consider Beagle::Context::setGenotypeHandle()."));
#endif // BEAGLE_NDEBUG
		return *mGenotypeHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a handle to the actual genotype.
	 *  \return Actual genotype handle.
	 */
	inline Genotype::Handle getGenotypeHandle() {
		Beagle_StackTraceBeginM();
		return mGenotypeHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return the actual genotype index.
	 *  \return Actual genotype index.
	 */
	inline unsigned int getGenotypeIndex() const {
		Beagle_StackTraceBeginM();
		return mGenotypeIndex;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return the actual generation number.
	 *  \return Actual generation number.
	 */
	inline unsigned int getGeneration() const {
		Beagle_StackTraceBeginM();
		return mGeneration;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return the actual selection index value.
	 *  \return Actual selection index value.
	 */
	inline unsigned int getSelectionIndex() const {
		Beagle_StackTraceBeginM();
		return mSelectionIndex;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Number of individuals processed by the actual deme in this generation.
	 */
	inline unsigned int getProcessedDeme() const {
		Beagle_StackTraceBeginM();
		return mProcessedDeme;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Total number of individuals processed by the actual deme in the evolution.
	 */
	inline unsigned int getTotalProcessedDeme() const {
		Beagle_StackTraceBeginM();
		return mTotalProcDeme;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Number of individuals processed by the actual vivarium in this generation.
	 */
	inline unsigned int getProcessedVivarium() const {
		Beagle_StackTraceBeginM();
		return mProcessedViva;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Total number of individuals processed by the actual vivarium in the evolution.
	 */
	inline unsigned int getTotalProcessedVivarium() const {
		Beagle_StackTraceBeginM();
		return mTotalProcViva;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a constant reference to the system.
	 *  \return Evolution system constant reference.
	 */
	inline const System& getSystem() const {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mSystemHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to a system. ")+
			    std::string("Consider Beagle::Context::setSystemHandle()."));
#endif // BEAGLE_NDEBUG
		return *mSystemHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a reference to the system.
	 *  \return Evolution system reference.
	 */
	inline System& getSystem() {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mSystemHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to a system. ")+
			    std::string("Consider Beagle::Context::setSystemHandle()."));
#endif // BEAGLE_NDEBUG
		return *mSystemHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a handle to the system.
	 *  \return Evolution system handle.
	 */
	inline System::Handle getSystemHandle() {
		Beagle_StackTraceBeginM();
		return mSystemHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a constant reference to the vivarium.
	 *  \return Evolution vivarium constant reference.
	 */
	inline const Vivarium& getVivarium() const {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mVivariumHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to a vivarium. ")+
			    std::string("Consider Beagle::Context::setVivariumHandle()."));
#endif // BEAGLE_NDEBUG
		return *mVivariumHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a reference to the vivarium.
	 *  \return Evolution vivarium reference.
	 */
	inline Vivarium& getVivarium() {
		Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
		if(mVivariumHandle == NULL) throw Beagle_RunTimeExceptionM(
			    std::string("The context does not have a handle to a vivarium. ")+
			    std::string("Consider Beagle::Context::setVivariumHandle()."));
#endif // BEAGLE_NDEBUG
		return *mVivariumHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Return a handle to the vivarium.
	 *  \return Evolution vivarium handle.
	 */
	inline Vivarium::Handle getVivariumHandle() {
		Beagle_StackTraceBeginM();
		return mVivariumHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Whether evolution terminated on success (e.g. individual is adequate fitness).
	 *  \return True is terminated successfuly, false if not.
	 */
	inline bool isTerminationSuccessful() const {
		Beagle_StackTraceBeginM();
		return mSuccessfulTermination;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the evolution continuing flag.
	 *  \param inContinueFlag Continuing flag value.
	 */
	inline void setContinueFlag(bool inContinueFlag) {
		Beagle_StackTraceBeginM();
		mContinueFlag = inContinueFlag;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set an handle to the actual deme.
	 *  \param inDemeHandle Actual deme handle.
	 */
	inline void setDemeHandle(Deme::Handle inDemeHandle) {
		Beagle_StackTraceBeginM();
		mDemeHandle = inDemeHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the actual deme index.
	 *  \param inDemeIndex Actual deme index.
	 */
	inline void setDemeIndex(unsigned int inDemeIndex) {
		Beagle_StackTraceBeginM();
		mDemeIndex = inDemeIndex;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set an handle to the evolver.
	 *  \param inEvolverHandle Evolver handle.
	 */
	inline void setEvolverHandle(Evolver::Handle inEvolverHandle) {
		Beagle_StackTraceBeginM();
		mEvolverHandle = inEvolverHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set an handle to the actual individual.
	 *  \param inIndividualHandle Actual individual handle.
	 */
	inline void setIndividualHandle(Individual::Handle inIndividualHandle) {
		Beagle_StackTraceBeginM();
		mIndividualHandle = inIndividualHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the actual individual index.
	 *  \param inIndividualIndex Actual individual index.
	 */
	inline void setIndividualIndex(unsigned int inIndividualIndex) {
		Beagle_StackTraceBeginM();
		mIndividualIndex = inIndividualIndex;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set an handle to the actual genotype.
	 *  \param inGenotypeHandle Actual genotype handle.
	 */
	inline void setGenotypeHandle(Genotype::Handle inGenotypeHandle) {
		Beagle_StackTraceBeginM();
		mGenotypeHandle = inGenotypeHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the actual genotype index.
	 *  \param inGenotypeIndex Actual genotype index.
	 */
	inline void setGenotypeIndex(unsigned int inGenotypeIndex) {
		Beagle_StackTraceBeginM();
		mGenotypeIndex = inGenotypeIndex;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the actual generation number.
	 *  \param inGeneration Actual generation number.
	 */
	inline void setGeneration(unsigned int inGeneration) {
		Beagle_StackTraceBeginM();
		mGeneration = inGeneration;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the actual selection index value.
	 *  \param inGeneration Actual selection index.
	 */
	inline void setSelectionIndex(unsigned int inSelectionIndex) {
		Beagle_StackTraceBeginM();
		mSelectionIndex = inSelectionIndex;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the number of individuals processed by the deme for the actual generation.
	 *  \param inProcessedDeme Number of individuals processed by the deme.
	 */
	inline void setProcessedDeme(unsigned int inProcessedDeme) {
		Beagle_StackTraceBeginM();
		mProcessedDeme = inProcessedDeme;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the total number of individuals processed by the deme for the evolution.
	 *  \param inTotalProcDeme Total number of individuals processed by the deme.
	 */
	inline void setTotalProcessedDeme(unsigned int inTotalProcDeme) {
		Beagle_StackTraceBeginM();
		mTotalProcDeme = inTotalProcDeme;
		Beagle_StackTraceEndM();
	}


	/*!
	 *  \brief Set the number of individuals processed by the vivarium for the actual generation.
	 *  \param inProcessedViva Number of individuals processed by the vivarium.
	 */
	inline void setProcessedVivarium(unsigned int inProcessedViva) {
		Beagle_StackTraceBeginM();
		mProcessedViva = inProcessedViva;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the total number of individuals processed by the vivarium for the evolution.
	 *  \param inTotalProcViva Total number of individuals processed by the vivarium.
	 */
	inline void setTotalProcessedVivarium(unsigned int inTotalProcViva) {
		Beagle_StackTraceBeginM();
		mTotalProcViva = inTotalProcViva;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the system handle.
	 *  \param inSystemHandle Handle to the system.
	 */
	inline void setSystemHandle(System::Handle inSystemHandle) {
		Beagle_StackTraceBeginM();
		mSystemHandle = inSystemHandle;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set whether the termination has been successful.
	 *  \param inSuccessfulTermination State of termination, true if successful, false if not.
	 */
	inline void setTerminationSuccessful(bool inSuccessfulTermination=true) {
		Beagle_StackTraceBeginM();
		mSuccessfulTermination = inSuccessfulTermination;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the vivarium handle.
	 *  \param inVivariumHandle Handle to the vivarium.
	 */
	inline void setVivariumHandle(Vivarium::Handle inVivariumHandle) {
		Beagle_StackTraceBeginM();
		mVivariumHandle = inVivariumHandle;
		Beagle_StackTraceEndM();
	}

protected:

	System::Handle     mSystemHandle;           //!< Handle to the system of the evolution.
	Evolver::Handle    mEvolverHandle;          //!< Handle to the evolver of the evolution.
	Vivarium::Handle   mVivariumHandle;         //!< Handle to the vivarium of the evolution.
	Deme::Handle       mDemeHandle;             //!< Handle to the actual deme of the evolution.
	unsigned int       mDemeIndex;              //!< Index of the actual deme of the evolution.
	Individual::Handle mIndividualHandle;       //!< Handle to the actual individual of the evolution.
	unsigned int       mIndividualIndex;        //!< Index of the actual individual of the evolution.
	Genotype::Handle   mGenotypeHandle;         //!< Handle to the actual genotype of the evolution.
	unsigned int       mGenotypeIndex;          //!< Index of the actual genotype of the evolution.
	unsigned int       mGeneration;             //!< Actual generation number of the evolution.
	unsigned int       mSelectionIndex;         //!< Index used to keep some state during selection.
	unsigned int       mProcessedDeme;          //!< # of indiv. processed by deme during actual gen.
	unsigned int       mTotalProcDeme;          //!< Total # of indiv. processed by the deme.
	unsigned int       mProcessedViva;          //!< # of indiv. processed by vivarium during actual gen.
	unsigned int       mTotalProcViva;          //!< Total # of indiv. processed by the vivarium.
	bool               mContinueFlag;           //!< Flag set if the evolution must continue.
	bool               mSuccessfulTermination;  //!< Flag set if evolution terminated on success.

};

}

#endif // Beagle_Core_Context_hpp
