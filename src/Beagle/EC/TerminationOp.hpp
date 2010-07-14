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
 *  \file   Beagle/EC/TerminationOp.hpp
 *  \brief  Definition of the class TerminationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_EC_TerminationOp_hpp
#define Beagle_EC_TerminationOp_hpp

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class TerminationOp Beagle/EC/TerminationOp.hpp "Beagle/EC/TerminationOp.hpp"
 *  \brief Check for termination criterion reached operator class.
 *  \ingroup EC
 *  \ingroup Op
 *  \par Note:
 *    The operator contained an handle to an other termination operator handle.
 *    If the handle is non-null, the termination test for the other termination operator is done
 *    and the evolution is stopped if necessary.
 */
class TerminationOp : public Beagle::Operator
{

public:

	//! TerminationOp allocator type
	typedef Beagle::AbstractAllocT<TerminationOp,Beagle::Operator::Alloc> Alloc;
	//! TerminationOp handle type.
	typedef Beagle::PointerT<TerminationOp,Beagle::Operator::Handle> Handle;
	//! TerminationOp bag type.
	typedef Beagle::ContainerT<TerminationOp,Beagle::Operator::Bag> Bag;

	explicit TerminationOp(std::string inName="TerminationOp");
	virtual ~TerminationOp()
	{ }

	/*!
	 *  \brief Check if the termination criterion is reached.
	 *  \param inDeme Actual deme of the evolution.
	 *  \param ioContext Actual evolution context.
	 *  \return True if the ending criterion is reached, false if not.
	 */
	virtual bool terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext) = 0;

	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

};

}

}

#endif // Beagle_EC_TerminationOp_hpp
