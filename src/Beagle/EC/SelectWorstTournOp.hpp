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
 *  \file   Beagle/EC/SelectWorstTournOp.hpp
 *  \brief  Definition of the class SelectWorstTournOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.1 $
 *  $Date: 2007/08/29 17:38:32 $
 */

#ifndef Beagle_EC_SelectWorstTournOp_hpp
#define Beagle_EC_SelectWorstTournOp_hpp

#include <string>

#include "Beagle/Core.hpp"


namespace Beagle
{

namespace EC
{

/*!
 *  \class SelectWorstTournOp Beagle/EC/SelectWorstTournOp.hpp "Beagle/EC/SelectWorstTournOp.hpp"
 *  \brief Selection of worst individual of a tournament operator class.
 *  \ingroup EC
 *  \ingroup Op
 */
class SelectWorstTournOp : public Beagle::EC::SelectionOp
{

public:

	//! SelectWorstTournOp allocator type.
	typedef Beagle::AllocatorT<SelectWorstTournOp,Beagle::EC::SelectionOp::Alloc> Alloc;
	//! SelectWorstTournOp handle type.
	typedef Beagle::PointerT<SelectWorstTournOp,Beagle::EC::SelectionOp::Handle> Handle;
	//! SelectWorstTournOp bag type.
	typedef Beagle::ContainerT<SelectWorstTournOp,Beagle::EC::SelectionOp::Bag> Bag;

	explicit SelectWorstTournOp(std::string inReproProbaName="ec.repro.prob",
	                            std::string inName="SelectWorstTournOp");
	virtual ~SelectWorstTournOp()
	{ }

	virtual void         registerParams(Beagle::System& ioSystem);
	virtual unsigned int selectOneIndividual(Beagle::Individual::Bag& ioPool, Beagle::Context& ioContext);

protected:

	Beagle::UInt::Handle mNumberParticipants;  //!< Number of participants to each tournament.

};

}

}

#endif // Beagle_EC_SelectWorstTournOp_hpp
