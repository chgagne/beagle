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
 *  \file   Beagle/SAES/PairVector.hpp
 *  \brief  Definition of the type SAES::PairVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 18:09:10 $
 */


#ifndef Beagle_SAES_PairVector_hpp
#define Beagle_SAES_PairVector_hpp

#include <vector>

#include "Beagle/Core.hpp"
#include "Beagle/EC.hpp"


namespace Beagle
{

namespace SAES
{

/*!
 *  \struct Pair Beagle/SAES/PairVector.hpp "Beagle/SAES/PairVector.hpp"
 *  \brief Evolution strategy (value, strategy parameter) pair composing SA-ES genotypes.
 *  \ingroup SAESF
 */
struct Pair
{
	double mValue;      //!< Value of the element of the ES vector.
	double mStrategy;   //!< Strategy parameter to use for ES mutation of the value.

	Pair(double inValue=0.0, double inStrategy=1.0);
	bool operator==(const Pair& inRightPair) const;
};


/*!
 *  \class PairVector Beagle/SAES/PairVector.hpp "Beagle/SAES/PairVector.hpp"
 *  \brief Evolution strategy genotype class.
 *  \ingroup SAESF
 */
class PairVector : public Genotype, public std::vector<Pair>
{

public:

	//! PairVector allocator type.
	typedef AllocatorT<PairVector,Genotype::Alloc> Alloc;
	//! PairVector handle type.
	typedef PointerT<PairVector,Genotype::Handle> Handle;
	//! PairVector bag type.
	typedef ContainerT<PairVector,Genotype::Bag> Bag;

	explicit PairVector(unsigned int inSize=0, Pair inModel=Pair(0.f,1.f));

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

#endif // Beagle_SAES_PairVector_hpp

