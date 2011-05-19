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
 *  \file   beagle/HPC/EvaluationOp.hpp
 *  \brief  Definition of the class EvaluationOp.
 *  \author Felix-Antoine Fortin
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_HPC_EvaluationOpBreederOp_hpp
#define Beagle_HPC_EvaluationOp_hpp

#include "beagle/HPC.hpp"
namespace Beagle
{
namespace HPC
{
/*!
 *  \class EvaluationOp beagle/HPC/EvaluationOp.hpp "beagle/HPC/EvaluationOp.hpp"
 *  \brief Abstract evaluation operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *  \ingroup FitStats
 */
class EvaluationOp : public Beagle::EvaluationOp
{

public:
	//! EvaluationOp allocator type.
	typedef AbstractAllocT<HPC::EvaluationOp,Beagle::EvaluationOp::Alloc> Alloc;
	//! EvaluationOp handle type.
	typedef PointerT<HPC::EvaluationOp,Beagle::EvaluationOp::Handle> Handle;
	//! EvaluationOp bag type.
	typedef ContainerT<HPC::EvaluationOp,Beagle::EvaluationOp::Bag> Bag;

	explicit EvaluationOp(std::string inName="HPC-EvaluationOp");
	virtual ~EvaluationOp()
	{ }
	virtual void operate(Deme& ioDeme, Context& ioContext);
};
}
}

#endif // Beagle_HPC_EvaluationOp_hpp
