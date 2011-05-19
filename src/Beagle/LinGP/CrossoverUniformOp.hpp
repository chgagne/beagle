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
 *  \file   beagle/LinGP/CrossoverUniformOp.hpp
 *  \brief  Definition of the class LinGP::CrossoverUniformOp.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/17 18:09:22 $
 */

#ifndef Beagle_LinGP_CrossoverUniformOp_hpp
#define Beagle_LinGP_CrossoverUniformOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/GA/CrossoverUniformOpT.hpp"
#include "beagle/LinGP/Program.hpp"

namespace Beagle
{
namespace LinGP
{

/*!
 *  \class CrossoverUniformOp beagle/LinGP/CrossoverUniformOp.hpp
 *    "beagle/LinGP/CrossoverUniformOp.hpp"
 *  \brief Linear GP uniform crossover operator class.
 *  \ingroup LinGPF
 */
class CrossoverUniformOp : public GA::CrossoverUniformOpT<Program>
{

public:

	//! LinGP::CrossoverUniformOp allocator type.
	typedef AllocatorT<CrossoverUniformOp,GA::CrossoverUniformOpT<Program>::Alloc> Alloc;
	//! LinGP::CrossoverUniformOp handle type.
	typedef PointerT<CrossoverUniformOp,GA::CrossoverUniformOpT<Program>::Handle> Handle;
	//! LinGP::CrossoverUniformOp bag type.
	typedef ContainerT<CrossoverUniformOp,GA::CrossoverUniformOpT<Program>::Bag> Bag;

	/*!
	 *  \brief Construct linear GP uniform crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inDistribPbName Name of the distribution probability parameter used in the register.
	 *  \param inName Name of the linear GP uniform crossover operator.
	 */
	explicit CrossoverUniformOp(std::string inMatingPbName="lingp.cxunif.prob",
	                            std::string inDistribPbName="lingp.cxunif.distribprob",
	                            std::string inName="LinGP-CrossoverUniformOp") :
			GA::CrossoverUniformOpT<Program>(inMatingPbName, inDistribPbName, inName)
	{ }

	virtual ~CrossoverUniformOp()
	{ }

};

}
}

#endif // Beagle_LinGP_CrossoverUniformOp_hpp
