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
 *  \file   Beagle/Core/Algorithm.hpp
 *  \brief  Definition of the class Algorithm.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_Core_Algorithm_hpp
#define Beagle_Core_Algorithm_hpp

#include <map>
#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/System.hpp"


namespace Beagle
{


/*!
 *  \class Algorithm Beagle/Core/Algorithm.hpp "Beagle/Core/Algorithm.hpp"
 *  \brief Abstract algorithm class, used to setup operators in an evolver.
 *  \ingroup Core
 */
class Algorithm : public NamedObject
{

public:

	//! Algorithm allocator type.
	typedef AbstractAllocT< Algorithm, Object::Alloc > Alloc;
	//! Algorithm handle type.
	typedef PointerT< Algorithm, Object::Handle > Handle;
	//! Algorithm bag type.
	typedef ContainerT< Algorithm, Object::Bag > Bag;

	explicit Algorithm(std::string inName="UnnamedAlgorithm");
	virtual ~Algorithm()
	{ }

	/*!
	 *  \brief Configure evolver to use with that algorithm.
	 *  \param ioEvolver Evolver modified by setting the algorithm.
	 *  \param ioSystem Evolutionary system.
	 */
	virtual void configure(Evolver& ioEvolver, System& ioSystem) =0;

};

}

#endif // Beagle_Core_Algorithm_hpp
