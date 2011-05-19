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
 *  \file   beagle/LinGP/Initialization.hpp
 *  \brief  Definition of the class LinGP::InitializationOp.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/17 18:09:22 $
 */

#ifndef Beagle_LinGP_InitializationOp_hpp
#define Beagle_LinGP_InitializationOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/Float.hpp"
#include "beagle/Int.hpp"

namespace Beagle
{
namespace LinGP
{

/*!
 *  \class InitializationOp beagle/LinGP/InitializationOp.hpp "beagle/LinGP/InitializationOp.hpp"
 *  \brief Linear GP initialization operator class.
 *  \ingroup LinGPF
 */
class InitializationOp : public Beagle::InitializationOp
{

public:

	//! LinGP::InitializationOp allocator type.
	typedef AllocatorT<InitializationOp,Beagle::InitializationOp::Alloc> Alloc;
	//! LinGP::InitializationOp handle type.
	typedef PointerT<InitializationOp,Beagle::InitializationOp::Handle> Handle;
	//! LinGP::InitializationOp bag type.
	typedef ContainerT<InitializationOp,Beagle::InitializationOp::Bag> Bag;

	explicit InitializationOp(IntArray inMinInitSize=IntArray(1,2),
	                          IntArray inMaxInitSize=IntArray(1,10),
	                          std::string inName="LinGP-InitializationOp");
	virtual ~InitializationOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void initIndividual(Beagle::Individual& outIndividual, Beagle::Context& ioContext);

	/*!
	 *  \return Minimum individual initialization size.
	 */
	const IntArray& getMinInitSize() const
	{
		return *mMinInitSize;
	}

	/*!
	 *  \return Minimum individual initialization size.
	 */
	IntArray& getMinInitSize()
	{
		return *mMinInitSize;
	}

	/*!
	 *  \return Maximum individual initialization size.
	 */
	const IntArray& getMaxInitSize() const
	{
		return *mMaxInitSize;
	}

	/*!
	 *  \return Maximum individual initialization size.
	 */
	IntArray& getMaxInitSize()
	{
		return *mMaxInitSize;
	}

protected:

	IntArray::Handle mMinInitSize;  //!< Minimum program size at initialization.
	IntArray::Handle mMaxInitSize;  //!< Maximum program size at initialization.

};

}
}

#endif // Beagle_LinGP_InitializationOp_hpp
