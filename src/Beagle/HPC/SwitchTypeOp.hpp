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
 *  \file   beagle/HPC/SwitchTypeOp.hpp
 *  \brief  Definition of the class HPC::SwitchTypeOp.
 *  \author Felix-Antoine Fortin
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_HPC_SwitchTypeOp_hpp
#define Beagle_HPC_SwitchTypeOp_hpp

#include "beagle/HPC.hpp"

#ifdef BEAGLE_HAVE_STDTR1_UNORDEREDMAP
#include <tr1/unordered_map>
#elif BEAGLE_HAVE_STD_UNORDEREDMAP
#include <unordered_map>
#endif

using namespace Beagle;

namespace Beagle {

namespace HPC {

/*!
 *  \class SwitchTypeOp beagle/HPC/SwitchTypeOp.hpp "beagle/HPC/SwitchTypeOp.hpp"
 *  \brief HPC Switch type operator class.
 *  This operator is used to execute different operator on different node type.
 *  The operators are inserted in their corresponding type set. The association
 *  node type - operator set is made with an hash map. The operator provides a
 *  function to insert operators, and a function to retrieve the operator set of
 *  a given type.
 */
class SwitchTypeOp : public Operator {

public:
	//! SwitchTypeOp allocator type
	typedef AllocatorT<SwitchTypeOp,Operator::Alloc> Alloc;
	//! SwitchTypeOp handle type.
	typedef PointerT<SwitchTypeOp,Operator::Handle> Handle;
	//! SwitchTypeOp bag type.
	typedef ContainerT<SwitchTypeOp,Operator::Bag> Bag;

	explicit SwitchTypeOp(std::string inName="SwitchTypeOp");
	virtual ~SwitchTypeOp() { }
	virtual void init(System& ioSystem);

	virtual void operate(Deme& ioDeme, Context& ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void registerParams(System& ioSystem);

	/*!
	*  \brief Return a constant reference to the corresponding inType operator set.
	*  \param inType Type of the node associated to the operator set that will be retrieve.
	*  \return corresponding inType operator set.
	*  If the node type isn't present in the map, the process is asserted.
	*/
	inline const Operator::Bag& getSet(const std::string& inType) const
	{
		Beagle_StackTraceBeginM();
		OperatorSetMap::const_iterator lIter = mOperatorSetMap.find(inType);
		Beagle_AssertM(lIter != mOperatorSetMap.end());
		return lIter->second;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Insert an operator in the corresponding inType set of the switch case operator.
	 *  \param inType Node type that will execute the operator \c inOperator.
	 *  \param inOperator Operator to insert.
	 *  If the node type isn't already in the map, the function adds a new entry in the map
	 *  with a empty operator set associated to it.
	 */
	void insertOp(const std::string& inType, Operator::Handle inOperator)
	{
		Beagle_StackTraceBeginM();
		OperatorSetMap::iterator lIter = mOperatorSetMap.find(inType);
		if(lIter == mOperatorSetMap.end()){
			mOperatorSetMap.insert(make_pair(inType,Operator::Bag()));
			lIter = mOperatorSetMap.find(inType);
		}
		lIter->second.push_back(inOperator);
		Beagle_StackTraceEndM();
	}

protected:
#if defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP)
	typedef std::tr1::unordered_map<std::string,Operator::Bag,HashString> OperatorSetMap;
#elif defined(BEAGLE_HAVE_STD_UNORDEREDMAP)
	typedef std::unordered_map<std::string,Operator::Bag,HashString> OperatorSetMap;
#else
	typedef std::map< std::string,Operator::Bag,std::less<std::string> > OperatorSetMap;
#endif

	OperatorSetMap mOperatorSetMap;
};
}
}

#endif // Beagle_HPC_SwitchTypeOp_hpp
