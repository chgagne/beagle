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
 *  \file   beagle/GP/Tree.hpp
 *  \brief  Definition of the type GP::Tree.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.22 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_GP_Tree_hpp
#define Beagle_GP_Tree_hpp

#include <vector>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/Container.hpp"
#include "beagle/Genotype.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveSet.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


namespace Beagle
{
namespace GP
{

class Context;  // Forward declaration.

/*!
 *  \struct Node beagle/GP/Tree.hpp "beagle/GP/Tree.hpp"
 *  \brief GP tree node structure.
 *  \ingroup GPF
 *  \ingroup GPPop
 */
struct Node
{
	Primitive::Handle mPrimitive;    //!< Smart pointer to the associated primitive.
	unsigned int      mSubTreeSize;  //!< Sub-tree size, including actual node.

	explicit Node(Primitive::Handle inPrimitive=NULL, unsigned int inSubTreeSize=0);
	bool operator==(const Node& inRightNode) const;
};


/*!
 *  \class Tree beagle/GP/Tree.hpp "beagle/GP/Tree.hpp"
 *  \brief The GP tree class, the genotype used in GP.
 *  \ingroup GPF
 *  \ingroup GPPop
 */
class Tree : public Genotype, public std::vector<Node>
{

public:

	//! GP::Tree allocator type.
	typedef AllocatorT<Tree,Genotype::Alloc> Alloc;
	//! GP::Tree handle type.
	typedef PointerT<Tree,Genotype::Handle> Handle;
	//! GP::Tree bag type.
	typedef ContainerT<Tree,Genotype::Bag> Bag;

	explicit Tree(unsigned int inSize=0,
	              unsigned int inPrimitiveSetIndex=UINT_MAX,
	              unsigned int inNumberArguments=0);

	virtual void         copy(const Member& inOriginal, System& ioSystem);
	virtual unsigned int fixSubTreeSize(unsigned int inNodeIndex=0);
	GP::PrimitiveSet&    getPrimitiveSet(GP::Context& ioContext) const;

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getRootType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual unsigned int        getSize() const;
	virtual const std::string&  getType() const;
	virtual void				swap(unsigned int inI, unsigned int inJ);
	unsigned int                getTreeDepth(unsigned int inNodeIndex=0) const;
	void                        interpret(GP::Datum& outResult, GP::Context& ioContext);
	virtual bool                isEqual(const Object& inRightObj) const;
	virtual std::string         deparse() const;
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Beagle::Context& ioContext);
	void                        setContextToNode(unsigned int inNodeIndex, GP::Context& ioContext) const;
	bool                        validateSubTree(unsigned int inNodeIndex, GP::Context& ioContext) const;
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Get the number of arguments for this particular tree
	 *  \return Number of arguments
	 */
	inline unsigned int getNumberArguments() const
	{
		Beagle_StackTraceBeginM();
		return mNumberArguments;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get the primitive set index for this particular tree
	 *  \return Primitive set index
	 */
	inline unsigned int getPrimitiveSetIndex() const
	{
		Beagle_StackTraceBeginM();
		return mPrimitiveSetIndex;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the number of arguments for this particular tree
	 *  \param inNumberArguments The new number of arguments
	 */
	inline void setNumberArguments(unsigned int inNumberArguments)
	{
		Beagle_StackTraceBeginM();
		mNumberArguments = inNumberArguments;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Set the primitive set index for this particular tree
	 *  \param inIndex The new primitive set index
	 */
	inline void setPrimitiveSetIndex(unsigned int inIndex)
	{
		Beagle_StackTraceBeginM();
		mPrimitiveSetIndex = inIndex;
		Beagle_StackTraceEndM();
	}

#ifdef BEAGLE_HAVE_RTTI
	/*!
	 *  \brief Set desired type of the tree root.
	 *  \param inRootType Desired tree root type.
	 */
	inline void setRootType(const std::type_info* inRootType)
	{
		Beagle_StackTraceBeginM();
		mRootType = inRootType;
		Beagle_StackTraceEndM();
	}
#endif // BEAGLE_HAVE_RTTI

protected:

	unsigned int deparseSubTree(std::string& outResult,
	                            unsigned int inN) const;
	unsigned int readSubTree(PACC::XML::ConstIterator inIter, GP::Context& ioContext);
	unsigned int writeSubTree(PACC::XML::Streamer& ioStreamer,
	                          unsigned int inN,
	                          bool inIndent=true) const;

	unsigned int mPrimitiveSetIndex;     //!< Primitive set index associated to current GP tree.
	unsigned int mNumberArguments;       //!< Number of ADF arguments to the GP tree.

#ifdef BEAGLE_HAVE_RTTI
	const std::type_info* mRootType;     //!< Desired type as the tree's root.
#endif // BEAGLE_HAVE_RTTI

};

}
}

#endif // Beagle_GP_Tree_hpp
