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
 *  \file   beagle/GA/CMAHolder.hpp
 *  \brief  Definition of the class GA::CMAHolder.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CMAHolder_hpp
#define Beagle_GA_CMAHolder_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Component.hpp"


namespace Beagle
{
namespace GA
{


/*!
 *  \class CMAValues beagle/GA/CMAValues.hpp "beagle/GA/CMAValues.hpp"
 *  \brief CMA-ES matrices and vector values.
 *  \ingroup GAF
 *  \ingroup GAFV
 */
struct CMAValues
{
	Matrix mB;       //!< CMA-ES B matrix.
	Vector mD;       //!< CMA-ES D matrix.
	Vector mPC;      //!< CMA-ES P_c cumulation path.
	Vector mPS;      //!< CMA-ES P_s cumulation path.
	Vector mXmean;   //!< Mean float vector individual.
	Double mSigma;   //!< CMA-ES sigma value.

	CMAValues()
	{ }
};


/*!
 *  \class CMAHolder beagle/GA/CMAHolder.hpp "beagle/GA/CMAHolder.hpp"
 *  \brief CMA-ES matrices and vector holder.
 *  \ingroup GAF
 *  \ingroup GAFV
 *
 *  The CMAHolder system component hold the global CMA-ES matrices and vectors used by the
 *  evolutionnary optimization process.
 *
 *  Reference: Hansen, N. and Ostermeier, A. (2001). Completely Derandomized Self-Adaptation
 *  in Evolution Strategies. Evolutionary Computation, 9(2), pp. 159-195.
 *  See also: http://www.bionik.tu-berlin.de/user/niko/cmaes_inmatlab.html
 */
class CMAHolder : public Beagle::Component,
			public std::map< unsigned int,Beagle::GA::CMAValues,std::less<unsigned int> >
{

public:

	//! GA::CMAHolder allocator type.
	typedef AllocatorT<CMAHolder,Beagle::Component::Alloc>
	Alloc;
	//! GA::CMAHolder handle type.
	typedef PointerT<CMAHolder,Beagle::Component::Handle>
	Handle;
	//! GA::CMAHolder bag type.
	typedef ContainerT<CMAHolder,Beagle::Component::Bag>
	Bag;

	explicit CMAHolder();
	virtual ~CMAHolder()
	{ }

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}
}

#endif // Beagle_GA_CMAHolder_hpp
