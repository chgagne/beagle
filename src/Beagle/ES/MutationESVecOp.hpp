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
 *  \file   beagle/GA/MutationESVecOp.hpp
 *  \brief  Definition of the class GA::MutationESVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_MutationESVecOp_hpp
#define Beagle_GA_MutationESVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class MutationESVecOp beagle/GA/MutationESVecOp.hpp "beagle/GA/MutationESVecOp.hpp"
 *  \brief Evolution strategy mutation operator class.
 *  \ingroup GAF
 *  \ingroup GAES
 *
 *  The evolution strategy simple mutation operator apply mutation on the individuals using the
 *  following equations:
 *  \f[\sigma^{\prime}_i = \sigma_i \exp^{\tau^{\prime} N(0,1) + \tau N_i(0,1)}\f]
 *  \f[x^{\prime}_i = x_i + \sigma_i N_i(0,1)\f]
 *  \f[\tau^{\prime} = 1 / \sqrt{2n}\f]
 *  \f[\tau = 1 / \sqrt{2\sqrt{n}}\f]
 *  where \f$N(0,1)\f$ is a random number generated using a Gaussian probability density function
 *  with a zero mean and a standard deviation of 1.0, \f$x^{\prime}_i\f$ is the new value of
 *  parameter i, and \f$\sigma^{\prime}_i\f$ the new mutation strategy parameter value of parameter
 *  i.
 */
class MutationESVecOp : public Beagle::MutationOp
{

public:

	//! GA::MutationESVecOp allocator type.
	typedef AllocatorT<MutationESVecOp,Beagle::MutationOp::Alloc>
	Alloc;
	//! GA::MutationESVecOp handle type.
	typedef PointerT<MutationESVecOp,Beagle::MutationOp::Handle>
	Handle;
	//! GA::MutationESVecOp bag type.
	typedef ContainerT<MutationESVecOp,Beagle::MutationOp::Bag>
	Bag;

	explicit MutationESVecOp(std::string inMutationPbName="es.mut.prob",
	                         std::string inMinStrategyName="es.mut.minstrategy",
	                         std::string inName="GA-MutationESVecOp");
	virtual ~MutationESVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	DoubleArray::Handle  mMaxValue;           //!< Maximum of ES vector values.
	DoubleArray::Handle  mMinValue;           //!< Minimum of ES vector values.
	Double::Handle       mMinStrategy;        //!< Minimun strategy parameter value.
	std::string          mMinStrategyName;    //!< Minimum strategy parameter parameter name.

};

}
}

#endif // Beagle_GA_MutationESVecOp_hpp
