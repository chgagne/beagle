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
 *  \file   Beagle/BitStr/CrossoverOnePointOp.hpp
 *  \brief  Definition of the class BitStr::CrossoverOnePointOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:34 $
 */


#ifndef Beagle_BitStr_CrossoverOnePointOp_hpp
#define Beagle_BitStr_CrossoverOnePointOp_hpp

#include <string>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"
#include "Beagle/BitStr/BitString.hpp"


namespace Beagle
{
namespace BitStr
{

/*!
 *  \class CrossoverOnePointOp Beagle/BitStr/CrossoverOnePointOp.hpp
 *    "Beagle/BitStr/CrossoverOnePointOp.hpp"
 *  \brief Bit string one point crossover operator class.
 *  \ingroup BitStrF
 */
class CrossoverOnePointOp : public EC::CrossoverOnePointOpT<BitStr::BitString>
{

public:

	//! BitStr::CrossoverOnePointOp allocator type.
	typedef AllocatorT<CrossoverOnePointOp,EC::CrossoverOnePointOpT<BitStr::BitString>::Alloc>
	Alloc;
	//! BitStr::CrossoverOnePointOp handle type.
	typedef PointerT<CrossoverOnePointOp,EC::CrossoverOnePointOpT<BitStr::BitString>::Handle>
	Handle;
	//! BitStr::CrossoverOnePointOp bag type.
	typedef ContainerT<CrossoverOnePointOp,EC::CrossoverOnePointOpT<BitStr::BitString>::Bag>
	Bag;

	/*!
	 *  \brief Construct bit string one point crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inName Name of the bit string one point crossover operator.
	 */
	explicit CrossoverOnePointOp(std::string inMatingPbName="bitstr.cx1p.prob",
	                             std::string inName="BitStr-CrossoverOnePointOp") :
			EC::CrossoverOnePointOpT<BitStr::BitString>(inMatingPbName, inName)
	{ }

	virtual ~CrossoverOnePointOp()
	{ }

};

}
}

#endif // Beagle_BitStr_CrossoverOnePointOp_hpp
