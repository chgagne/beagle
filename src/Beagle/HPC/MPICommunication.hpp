/*
 * Open BEAGLE
 * Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Contact:
 * Laboratoire de Vision et Systemes Numeriques
 * Departement de genie electrique et de genie informatique
 * Universite Laval, Quebec, Canada, G1K 7P4
 * http://vision.gel.ulaval.ca
 *
 */

/*!
 * \file beagle/HPC/MPICommunication.hpp
 * \brief Definition of the class MPICommunication.
 * \author Felix-Antoine Fortin
 *  $Revision: $
 *  $Date: $
 */

#ifndef Beagle_HPC_MPICommunication_hpp
#define Beagle_HPC_MPICommunication_hpp

#include <mpi.h>

#ifdef BEAGLE_HAVE_STDTR1_UNORDEREDMAP
#include <tr1/unordered_map>
#elif BEAGLE_HAVE_STD_UNORDEREDMAP
#include <unordered_map>
#endif

#ifdef BEAGLE_HAVE_LIBZ
#include <zlib.h>
#endif //BEAGLE_HAVE_LIBZ

using namespace Beagle;

namespace Beagle {

namespace HPC {

/*!
*  \class MPICommunication beagle/HPC/MPICommunication.hpp "beagle/HPC/MPICommunication.hpp"
*  \brief Component to manage MPI communication.
*  \ingroup HPC
*/
class MPICommunication : public Component {
public:

	class Request : public Beagle::Object{
		public:
			typedef AllocatorT< Request, Object::Alloc > Alloc;
			typedef PointerT< Request, Object::Handle > Handle;
			typedef ContainerT< Request, Object::Bag > Bag;
			Request(void) : mSize(0),mCancel(false) {}
			~Request(void) {}
			inline bool isCancelled(){ return mCancel; }
			inline const Beagle::String::Handle getMessage() const { return mMessage; }
		protected:
			MPI::Request mSizeRequest;
			MPI::Request mMsgRequest;
			std::string mTag;
			int mSize;
			bool mCancel;
			Beagle::String::Handle mMessage;
			enum eCommType {RECEPTION, SENDING};
			eCommType mType;

			friend class MPICommunication;
	};

	//! MPICommunication allocator type.
	typedef AllocatorT<MPICommunication,Component::Alloc> Alloc;
	//! MPICommunication handle type.
	typedef PointerT<MPICommunication,Component::Handle> Handle;
	//! MPICommunication bag type.
	typedef ContainerT<MPICommunication,Component::Bag> Bag;

	//! Singleton accessor
	static MPICommunication::Handle getInstance();

	virtual void init(Beagle::System& ioSystem);
	virtual void registerParams(Beagle::System& ioSystem);

	void wait(Request::Handle) const;
	void waitAll(Request::Bag&) const;
	int waitAny(Request::Bag&) const;

	//! Cancel non-blocking transaction request.
	void cancel(Request::Handle inRequest) const;

	/*! \brief Wait for all task to reach the same point.
	 * When a task encounters a Barrier call, it waits for all the other tasks to reach the
	 * same point, before continuing its execution.
	 */
	inline void barrier() const{
		MPI::COMM_WORLD.Barrier();
	}

	//! Return the number nodes with the relation type \c inRelationType.
	inline unsigned int getNbrOfRelation(const std::string& inRelationType) const {
		Beagle_StackTraceBeginM();
		TopologyMap::const_iterator lIterTopoMap = mTopology.find(inRelationType);
		if(lIterTopoMap == mTopology.end()) return 0;
		else return lIterTopoMap->second->size();
		Beagle_StackTraceEndM();
	}

	inline unsigned int getNbrOfType(const std::string& inNodeType) const {
		Beagle_StackTraceBeginM();
		TypeSizeMap::const_iterator lIter = mTypesMap.find(inNodeType);
		if(lIter == mTypesMap.end()) return 0;
		else return lIter->second;
		Beagle_StackTraceEndM();
	}

	inline void send(const std::string& inMessage,
			 const std::string& inTag,
			 const std::string& inTypeName,
			 unsigned int inIndex=0) const
	{
		Beagle_StackTraceBeginM();
		TopologyMap::const_iterator lIterTopoMap = mTopology.find(inTypeName);
		Beagle_AssertM(lIterTopoMap != mTopology.end());
		Beagle_BoundCheckAssertM(inIndex, 0, lIterTopoMap->second->size()-1);
		send(inMessage,inTag, (*lIterTopoMap->second)[inIndex]);
		Beagle_StackTraceEndM();
	}

	inline void sendNonBlocking(const std::string& inMessage,
				    Request::Handle outRequest,
				    const std::string& inTag,
				    const std::string& inTypeName,
				    unsigned int inIndex=0) const
	{
		Beagle_StackTraceBeginM();
		TopologyMap::const_iterator lIterTopoMap = mTopology.find(inTypeName);
		Beagle_AssertM(lIterTopoMap != mTopology.end());
		Beagle_BoundCheckAssertM(inIndex, 0, lIterTopoMap->second->size()-1);
		sendNonBlocking(inMessage, outRequest, inTag, (*lIterTopoMap->second)[inIndex]);
		Beagle_StackTraceEndM();
	}

	inline void receive(std::string& outMessage,
		            const std::string& inTag,
		            const std::string& inTypeName,
	                    unsigned int inIndex=0) const
	{
		Beagle_StackTraceBeginM();
		TopologyMap::const_iterator lIterTopoMap = mTopology.find(inTypeName);
		Beagle_AssertM(lIterTopoMap != mTopology.end());
		Beagle_BoundCheckAssertM(inIndex, 0, lIterTopoMap->second->size()-1);
		receive(outMessage, inTag, (*lIterTopoMap->second)[inIndex]);
		Beagle_StackTraceEndM();
	}

	inline void receiveNonBlocking(Request::Handle outRequest,
				       const std::string& inTag,
				       const std::string& inTypeName,
				       unsigned int inIndex=0) const
	{
		Beagle_StackTraceBeginM();
		TopologyMap::const_iterator lIterTopoMap = mTopology.find(inTypeName);
		Beagle_AssertM(lIterTopoMap != mTopology.end());
		Beagle_BoundCheckAssertM(inIndex, 0, lIterTopoMap->second->size()-1);
		receiveNonBlocking(outRequest, inTag, (*lIterTopoMap->second)[inIndex]);
		Beagle_StackTraceEndM();
	}

	/*!
	* \brief Return true if the multiple threads environment is provided
	*/
	inline bool isMultipleThreadsProvided() const
	{
		Beagle_StackTraceBeginM();
		return mMultipleThreads;
		Beagle_StackTraceEndM();
	}

	/*!
	*  \brief Get the current node type.
	*  \return "Supervisor"
	*  \return "Evolver"
	*  \return "Evaluator"
	*/
	inline std::string getNodeType() const
	{
		Beagle_StackTraceBeginM();
		return mNodeType;
		Beagle_StackTraceEndM();
	}

	/*!
	 *  \brief Get the current MPI node rank.
	 *  \return MPI node rank.
	 */
	inline const UInt::Handle getNodeRank() const
	{
		Beagle_StackTraceBeginM();
		return mNodeRank;
		Beagle_StackTraceEndM();
	}

protected:
	//! Construct default MPICommunication.
	explicit MPICommunication(void);
	//! Destructor of the component
	virtual ~MPICommunication();

	static void abortMPI(int inSigNum);

	void send(const std::string& inMessage, const std::string& inTag, int inRank) const;
	void receive(std::string& outMessage, const std::string& inTag, int inRank) const;

	void receiveNonBlocking(Request::Handle, const std::string& inTag, int inRank) const;
	void sendNonBlocking(const std::string& inMessage, Request::Handle, const std::string& inTag, int inRank) const;

	void waitReception(Request::Handle) const;
	void waitSending(Request::Handle) const;

	//! Produce an integer tag from an string tag.
	int hashTag(const std::string& inTag) const;

protected:
#if defined(BEAGLE_HAVE_STDTR1_UNORDEREDMAP)
	typedef std::tr1::unordered_map<std::string,IntArray::Handle,HashString> TopologyMap;
	typedef std::tr1::unordered_map<std::string,unsigned int,HashString> TypeSizeMap;
#elif defined(BEAGLE_HAVE_STD_UNORDEREDMAP)
	typedef std::unordered_map<std::string,IntArray::Handle,HashString> TopologyMap;
	typedef std::unordered_map<std::string,unsigned int,HashString> TypeSizeMap;
#else
	typedef std::map<std::string,IntArray::Handle, std::less<std::string> > TopologyMap;
	typedef std::map<std::string,unsigned int, std::less<std::string> > TypeSizeMap;
#endif

	TopologyMap mTopology;
	TypeSizeMap mTypesMap;

	std::string mNodeType;
	UInt::Handle mNodeRank;
	UInt::Handle mNbEvolvers;

	int mTagUpperBound;
	bool mMultipleThreads;

#ifdef BEAGLE_HAVE_LIBZ
	void compressString(const std::string& inString, std::string& outCompress, int inLevel = Z_BEST_COMPRESSION) const;
	void decompressString(const std::string& inCompress, std::string& outString) const;
	UInt::Handle mCompressionLevel;
#endif //BEAGLE_HAVE_LIBZ

private:
	static PACC::Threading::Mutex mInstMutex;
	static bool mAborted;
	static MPICommunication::Handle mInstance;

};
}
}
#endif // Beagle_HPC_MPICommunication
