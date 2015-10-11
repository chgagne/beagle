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
 *  \file   beagle/GP/Primitive.hpp
 *  \brief  Definition of the type GP::Primitive.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.22 $
 *  $Date: 2008/05/18 19:16:49 $
 */

/*!
 *  \defgroup Primit GP Primitives
 *  \ingroup GPF
 *  \brief Genetic programming primitives mechanism and library, part of the GP framework.
 */

#ifndef Beagle_GP_Primitive_hpp
#define Beagle_GP_Primitive_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/NamedObject.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/Container.hpp"
#include "beagle/System.hpp"
#include "beagle/GP/Datum.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

namespace Beagle
{
namespace GP
{

// Forward declaration.
class Context;

/*!
 *  \class Primitive beagle/GP/Primitive.hpp "beagle/GP/Primitive.hpp"
 *  \brief The GP primitive class.
 *  \ingroup GPF
 *  \ingroup Primit
 */
class Primitive : public NamedObject
{

public:

	//! Specify the constants for specific kind of primitive arguments.
	enum ArgumentKind {eTerminal=0, eBranch=(UINT_MAX-1), eAny=UINT_MAX};

	//! GP::Primitive allocator type.
	typedef AbstractAllocT<Primitive,Object::Alloc>
	Alloc;
	//! GP::Primitive handle type.
	typedef PointerT<Primitive,Object::Handle>
	Handle;
	//! GP::Primitive bag type.
	typedef ContainerT<Primitive,Object::Bag>
	Bag;

	explicit Primitive(unsigned int inNumberArguments=GP::Primitive::eAny,
	                   std::string inName="UnnamedPrimitive");
	virtual ~Primitive()
	{ }

	/*!
	 *  \brief Execute the caracteristic primitive operation.
	 *  \param outDatum Result of the execution.
	 *  \param ioContext Evolutionary context.
	 */
	virtual void execute(GP::Datum& outDatum, GP::Context& ioContext) =0;

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual std::string  deparse(std::vector<std::string>& lSubTrees) const;
	unsigned int         getChildrenNodeIndex(unsigned int inN, GP::Context& ioContext) const;
	inline  unsigned int getNumberArguments() const;
	virtual double       getSelectionWeight(unsigned int inNumberArguments,
	                                        GP::Context& ioContext) const;
	virtual void         getValue(Object& outValue);
	virtual bool         haveValue() const;
	virtual Handle       giveReference(unsigned int inNumberArguments, GP::Context& ioContext);
	virtual bool         isEqual(const Object& inRightObj) const;
	virtual bool         isSelectionWeightStable(unsigned int inNumberArguments) const;
	virtual void         registerParams(System& ioSystem);
	virtual void         readWithContext(PACC::XML::ConstIterator inIter, GP::Context& ioContext);
	inline  void         setNumberArguments(unsigned int inNumberArguments);
	virtual void         setValue(const Object& inValue);
	virtual bool         validate(GP::Context& ioContext) const;
	virtual void         write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void         writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	inline unsigned int getArgumentIndex(unsigned int inN,
	                                     Beagle::GP::Context& ioContext) const;
	inline void getArgument(unsigned int inN,
	                        Beagle::GP::Datum& outResult,
	                        Beagle::GP::Context& ioContext);
	inline void getArguments(Beagle::GP::Datum outResults[],
	                         size_t inSizeTDatum,
	                         Beagle::GP::Context& ioContext);
	inline void get1stArgument(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext);
	inline void get2ndArgument(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext);
	inline void get3rdArgument(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext);

private:

	unsigned int mNumberArguments;  //!< Number of arguments of the primitive.

};

}
}

#endif // Beagle_GP_Primitive_hpp


