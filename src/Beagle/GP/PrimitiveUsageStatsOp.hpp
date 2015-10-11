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
 *  \file   beagle/GP/PrimitiveUsageStatsOp.hpp
 *  \brief  Definition of the class PrimitiveUsageStatsOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_PrimitiveUsageStatsOp_hpp
#define Beagle_GP_PrimitiveUsageStatsOp_hpp

#include "beagle/Operator.hpp"
#include "beagle/GP/Primitive.hpp"


namespace Beagle
{
namespace GP
{


/*!
 *  \class PrimitiveUsageStatsOp beagle/GP/PrimitiveUsageStatsOp.hpp "beagle/GP/PrimitiveUsageStatsOp.hpp"
 *  \brief The GP primitive usage statistics operator class. Report usage of primitives.
 *  \ingroup GPF
 *  \ingroup Primit
 */
class PrimitiveUsageStatsOp : public Beagle::Operator
{

public:

	//! GP::PrimitiveUsageStatsOp allocator type.
	typedef AllocatorT<PrimitiveUsageStatsOp,Beagle::Operator::Alloc>
	Alloc;
	//! GP::PrimitiveUsageStatsOp handle type.
	typedef PointerT<PrimitiveUsageStatsOp,Beagle::Operator::Handle>
	Handle;
	//! GP::PrimitiveUsageStatsOp bag type.
	typedef ContainerT<PrimitiveUsageStatsOp,Beagle::Operator::Bag>
	Bag;

	explicit PrimitiveUsageStatsOp(std::string inName="GP-PrimitiveUsageStatsOp");
	virtual  ~PrimitiveUsageStatsOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

protected:
	std::string cleanSerialized(const std::string& inString);

	Bool::Handle mDetailedPrimitives; //!< Flag for whether primitives are stored with their attributes or just their names

};

}
}

#endif // Beagle_GP_PrimitiveUsageStatsOp_hpp
