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
 *  \file   beagle/LinGP/CrossoverOnePointBitStrOp.hpp
 *  \brief  Definition of the class LinGP::CrossoverOnePointBitStrOp.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/17 18:09:22 $
 */

#ifndef Beagle_LinGP_CrossoverOnePointOp_hpp
#define Beagle_LinGP_CrossoverOnePointOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/GA/CrossoverOnePointOpT.hpp"
#include "beagle/LinGP/Program.hpp"

namespace Beagle
{
namespace LinGP
{

/*!
 *  \class CrossoverOnePointOp beagle/LinGP/CrossoverOnePointOp.hpp
 *    "beagle/LinGP/CrossoverOnePointOp.hpp"
 *  \brief Lineat GP one point crossover operator class.
 *  \ingroup LinGPF
 */
class CrossoverOnePointOp : public GA::CrossoverOnePointOpT<Program>
{

public:

	//! LinGP::CrossoverOnePointOp allocator type.
	typedef AllocatorT<CrossoverOnePointOp,GA::CrossoverOnePointOpT<Program>::Alloc> Alloc;
	//! LinGP::CrossoverOnePointOp handle type.
	typedef PointerT<CrossoverOnePointOp,GA::CrossoverOnePointOpT<Program>::Handle> Handle;
	//! LinGP::CrossoverOnePointOp bag type.
	typedef ContainerT<CrossoverOnePointOp,GA::CrossoverOnePointOpT<Program>::Bag> Bag;

	/*!
	 *  \brief Construct linear GP one point crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inName Name of the linear GP one point crossover operator.
	 */
	explicit CrossoverOnePointOp(std::string inMatingPbName="lingp.cx1p.prob",
	                             std::string inName="LinGP-CrossoverOnePointOp") :
			GA::CrossoverOnePointOpT<Program>(inMatingPbName, inName)
	{ }

	virtual ~CrossoverOnePointOp()
	{ }

};

}
}

#endif // Beagle_LinGP_CrossoverOnePointOp_hpp
