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
 *  \file   beagle/GP/Individual.hpp
 *  \brief  Definition of the class GP::Individual.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_GP_Individual_hpp
#define Beagle_GP_Individual_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Individual.hpp"
#include "beagle/GP/Tree.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class Individual beagle/GP/Individual.hpp "beagle/GP/Individual.hpp"
 *  \brief An individual inherits from a container of genotypes and has a fitness member.
 *  \ingroup GPF
 *  \ingroup GPPop
 */
class Individual : public Beagle::Individual
{

public:

	//! GP::Individual allocator type.
	typedef AllocatorT<GP::Individual,Beagle::Individual::Alloc> Alloc;
	//! GP::Individual handle type.
	typedef PointerT<GP::Individual,Beagle::Individual::Handle> Handle;
	//! GP::Individual bag type.
	typedef ContainerT<GP::Individual,Beagle::Individual::Bag> Bag;

	explicit Individual(unsigned int inN=0, GP::Tree::Handle inModel=NULL);

	unsigned int                chooseRandomTree(GP::Context& ioContext) const;
	unsigned int                chooseRandomNode(unsigned int inTree, GP::Context& ioContext) const;
	unsigned int                chooseRandomNodeWithArgs(unsigned int inTree, GP::Context& ioContext) const;
	unsigned int                chooseRandomNodeWithoutArgs(unsigned int inTree, GP::Context& ioContext) const;
	std::string                 deparse(GP::Context& ioContext, const std::string& inFunctionNamePrefix="individual") const;
	virtual const std::string&  getType() const;
	void                        run(GP::Datum& outResult, GP::Context& ioContext);
	bool                        validate(GP::Context& ioContext);

	/*!
	 *  \brief Acces to a constant handle the nth tree of the GP::Individual.
	 *  \param inN Indice of the GP::Tree to get
	 *  \return Constant pointer (GP::Tree::Handle) at the indice N.
	 */
	inline const GP::Tree::Handle& operator[](unsigned int inN) const
	{
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,size()-1);
		return castHandleT<GP::Tree>(std::vector<Pointer>::operator[](inN));
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Acces to an handle the nth tree of the GP::Individual.
	 *  \param inN Indice of the GP::Tree to get.
	 *  \return Pointer (GP::Tree::Handle) at the indice N.
	 */
	inline GP::Tree::Handle& operator[](unsigned int inN)
	{
		Beagle_StackTraceBeginM();
		Beagle_UpperBoundCheckAssertM(inN,size()-1);
		return castHandleT<GP::Tree>(std::vector<Pointer>::operator[](inN));
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Maximum tree depth of the individual.
	 */
	inline unsigned int getMaxTreeDepth() const
	{
		Beagle_StackTraceBeginM();
		unsigned int lDepth = 0;
		for(unsigned int i=0; i<size(); ++i) lDepth = maxOf(lDepth, (*this)[i]->getTreeDepth());
		return lDepth;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \return Total number of nodes in the trees.
	 */
	inline unsigned int getTotalNodes() const
	{
		Beagle_StackTraceBeginM();
		unsigned int lNumberNodes = 0;
		for(unsigned int i=0; i<size(); ++i) lNumberNodes += (*this)[i]->size();
		return lNumberNodes;
		Beagle_StackTraceEndM();
	}

};

}
}

#endif // Beagle_GP_Individual_hpp
