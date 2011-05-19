/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/Context.hpp
 *  \brief  Definition of the type LinGP::Context.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/17 18:09:22 $
 */

#ifndef Beagle_LinGP_Context_hpp
#define Beagle_LinGP_Context_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/AssertException.hpp"
#include "beagle/Context.hpp"
#include "beagle/LinGP/Program.hpp"
#include "beagle/LinGP/Memory.hpp"

namespace Beagle
{
namespace LinGP
{


/*!
 *  \class Context beagle/LinGP/Context.hpp "beagle/LinGP/Context.hpp"
 *  \brief The linear GP context class.
 *  \ingroup LinGPF
 *  \ingroup LinGPSys
 */
class Context : public Beagle::Context
{

public:

	//! LinGP::Context allocator type.
	typedef AllocatorT<Context,Beagle::Context::Alloc> Alloc;
	//! LinGP::Context handle type.
	typedef PointerT<Context,Beagle::Context::Handle> Handle;
	//! LinGP::Context bag type.
	typedef ContainerT<Context,Beagle::Context::Bag> Bag;

	/*!
	 *  \brief Construct a linear GP context.
	 */
	Context() :
			mExecIndex(0),
			mExecCount(0),
			mExecLimit(0)
	{ }

	virtual ~Context()
	{ }

	/*!
	 *  \brief Return execution index in the genotype.
	 *  \return Index to the instruction in the genotype actually executed.
	 */
	inline unsigned int getExecIndex() const
	{
		return mExecIndex;
	}

	/*!
	 *  \brief Return number of instructions executed.
	 *  \return Number of instructions actually executed.
	 */
	inline unsigned int getExecCount() const
	{
		return mExecCount;
	}

	/*!
	 *  \brief Return maximum number of instructions allowed.
	 *  \return Maximum number of instructions allowed.
	 */
	inline unsigned int getExecLimit() const
	{
		return mExecLimit;
	}

	/*!
	 *  \brief Return a constant reference to the actual linear GP program.
	 *  \return Actual linear GP program constant reference.
	 */
	inline const LinGP::Program& getGenotype() const
	{
		return castObjectT<const LinGP::Program&>(*mGenotypeHandle);
	}

	/*!
	 *  \brief Return a reference to the actual linear GP program.
	 *  \return Actual linear GP program reference.
	 */
	inline LinGP::Program& getGenotype()
	{
		return castObjectT<LinGP::Program&>(*mGenotypeHandle);
	}

	/*!
	 *  \brief Return a handle to the actual linear GP program.
	 *  \return Actual linear GP program handle.
	 */
	LinGP::Program::Handle getGenotypeHandle()
	{
		return castHandleT<LinGP::Program>(mGenotypeHandle);
	}

	/*!
	 *  \brief Return a constant reference to the shared memories.
	 *  \return Constant reference to the shared memories.
	 */
	inline const Memory::Bag& getSharedMemories() const
	{
		return mSharedMemories;
	}

	/*!
	 *  \brief Return a reference to the shared memories.
	 *  \return Reference to the shared memories.
	 */
	inline Memory::Bag& getSharedMemories()
	{
		return mSharedMemories;
	}

	/*!
	 *  \brief Set index of instruction actually executed in genotype.
	 *  \param inExecIndex Execution index value.
	 */
	inline void setExecIndex(unsigned int inExecIndex)
	{
		mExecIndex = inExecIndex;
	}

	/*!
	 *  \brief Number of instructions actually executed.
	 *  \param inExecCount Number of instructions actually executed.
	 */
	inline void setExecCount(unsigned int inExecCount)
	{
		mExecCount = inExecCount;
	}

	/*!
	 *  \brief Maximum number of instruction executions.
	 *  \param inExecLimit Maximum number of instruction executions.
	 */
	inline void setExecLimit(unsigned int inExecLimit)
	{
		mExecLimit = inExecLimit;
	}

	/*!
	 *  \brief Set an handle to the actual linear GP program.
	 *  \param inTreeHandle Actual linear GP program handle.
	 */
	inline void setGenotypeHandle(LinGP::Program::Handle inProgramHandle)
	{
		mGenotypeHandle = inProgramHandle;
	}

protected:

	Memory::Bag  mSharedMemories;    // Memories shared in the context.
	unsigned int mExecIndex;         // Index of the instruction actually executed in the genotype
	unsigned int mExecCount;         // Number of instructions executed
	unsigned int mExecLimit;         // Maximum number of instructions allowed.

};

}
}

#endif // Beagle_LinGP_Context_hpp



