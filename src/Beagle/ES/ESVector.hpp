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
 *  \file   beagle/GA/ESVector.hpp
 *  \brief  Definition of the type GA::ESVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 18:09:10 $
 */

/*!
 *  \defgroup GAES Evolution Strategy
 *  \ingroup GAF
 *  \brief Evolution strategy (ES) representation, operators and evolvers, part of the GA framework.
 *
 *  Actual evolution strategy support in the GA framework includes a representation made of vectors of
 *  (value,strategy) pairs, where the value is parameter value and strategy is the adaptive mutation
 *  parameter strategy, a simple mutation operator for the vector of pairs representation and a
 *  ES evolver using by default a (mu,lambda) replacement strategy. Different GA crossover are also
 *  supported although they are not used in the default ES evolver.
 */

#ifndef Beagle_GA_ESVector_hpp
#define Beagle_GA_ESVector_hpp

#include <vector>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Genotype.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \struct ESPair beagle/GA/ESVector.hpp "beagle/GA/ESVector.hpp"
 *  \brief Evolution strategy (value, strategy parameter) pair composing ES genotypes.
 *  \ingroup GAF
 *  \ingroup GAES
 */
struct ESPair
{
	double mValue;      //!< Value of the element of the ES vector.
	double mStrategy;   //!< Strategy parameter to use for ES mutation of the value.

	ESPair(double inValue=0.0, double inStrategy=1.0);
	bool operator==(const ESPair& inRightPair) const;
};


/*!
 *  \class ESVector beagle/GA/ESVector.hpp "beagle/GA/ESVector.hpp"
 *  \brief Evolution strategy genotype class.
 *  \ingroup GAF
 *  \ingroup GAES
 */
class ESVector : public Genotype, public std::vector<ESPair>
{

public:

	//! ESVector allocator type.
	typedef AllocatorT<ESVector,Genotype::Alloc> Alloc;
	//! ESVector handle type.
	typedef PointerT<ESVector,Genotype::Handle> Handle;
	//! ESVector bag type.
	typedef ContainerT<ESVector,Genotype::Bag> Bag;

	explicit ESVector(unsigned int inSize=0, ESPair inModel=ESPair(0.f,1.f));

	virtual void                copy(const Member& inOriginal, System& ioSystem);
	virtual unsigned int        getSize() const;
	virtual const std::string&  getType() const;
	virtual void				swap(unsigned int inI, unsigned inJ);
	virtual bool                isEqual(const Object& inRightObj) const;
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}
}

#endif // Beagle_GA_ESVector_hpp

