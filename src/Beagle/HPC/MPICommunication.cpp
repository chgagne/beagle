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
 *  \file   beagle/HPC/MPICommunication.cpp
 *  \brief  Source code of the class HPC::MPICommunication.
 *  \author Felix-Antoine Fortin
 *  $Revision: $
 *  $Date: $
 */

#include "beagle/HPC.hpp"
#include <csignal>


using namespace Beagle;

HPC::MPICommunication::Handle HPC::MPICommunication::mInstance = 0;
PACC::Threading::Mutex HPC::MPICommunication::mInstMutex;
bool HPC::MPICommunication::mAborted = false;
/*!
 * \brief Build this MPICommunication component.
 * Abort all MPI communications. This function is associated with
 * the signal SIGABRT during the construction of this component.
 */
void HPC::MPICommunication::abortMPI(int inSigNum)
{
	Beagle_StackTraceBeginM();
	mAborted = true;
	MPI::COMM_WORLD.Abort(inSigNum);
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::abortMPI(int)");
}
/*!
 * \brief Return a unique instance of the class MPICommunication.
 * The MPICommunication object is a singleton. This function uses
 * a lock around the instance initialisation so only one instance
 * can be created. If the handle is null, it creates the first and
 * only instance of the class, and then it returns an handle to
 * the instance. The lock permits the class to be used in a multi-
 * thread environment.
 */

HPC::MPICommunication::Handle HPC::MPICommunication::getInstance()
{
	Beagle_StackTraceBeginM();
	mInstMutex.lock();
	if(mInstance == 0){
		mInstance = new HPC::MPICommunication;
	}
	mInstMutex.unlock();
	return mInstance;
	Beagle_HPC_StackTraceEndM("HPC::MPICommunication::Handle HPC::MPICommunication::getInstance()");
}

/*!
 * \brief Build this MPICommunication component.
 * The constructor initialise MPI and request a multiple threads MPI
 * environment. MPI::Init_thread return MPI_THREAD_MULTIPLE if the
 * multiple threads environment is available on the system, otherwise
 * it returns MPI_THREAD_SINGLE. If the multiple threads environment
 * isn't provided, only the main thread can make MPI calls.
 */
HPC::MPICommunication::MPICommunication(void) : Component("MPICommunication"), mMultipleThreads(false)
{
	if(MPI::Init_thread(MPI_THREAD_MULTIPLE) == MPI_THREAD_MULTIPLE)
		mMultipleThreads = true;
	MPI::COMM_WORLD.Set_errhandler(MPI::ERRORS_THROW_EXCEPTIONS);
	signal(SIGABRT, MPICommunication::abortMPI);
}

/*!
 * \brief Destroy the MPI Communication component.
 * For now, we have a problem with this function. Essentially, MPI::Finalize()
 * should be call here. But, when the program is destroy because of an exception
 * that occurs in the current process, MPI::COMM_WORLD.Abort() should be called
 * before. If MPI::Finalize() is called when an exceptions occurs, there is an
 * high probability of deadlocking. Since Finalize() is waiting for all processes
 * to call Finalize, if one process is waiting for a reception from the process
 * that just called Finalize, a deadlock occurs, and we never know what happened.
 * So we need a mechanism to determine if an exception occured.
 *
 * The mechanism we found for now is to check if an abortion signal has been
 * launched. Since, we associated the function abortMPI to the SIGABRT we are aware
 * of the abortion by the variable mAborted. If an MPI process quits without calling
 * MPI::Finalize(), every other process quit immediatly.
 *
 */
HPC::MPICommunication::~MPICommunication()
{
	if(!mAborted)
		MPI::Finalize();
}

/*!
 * \brief Initialize this MPICommunication component.
 * \param ioSystem System reference.
 */
void HPC::MPICommunication::init(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	void* lValue;
	if(!MPI::COMM_WORLD.Get_attr( MPI::TAG_UB, &lValue ))
		throw RunTimeException("HPC::MPICommunication::init(Beagle::System&) the MPI::TAG_UB couldn't be retrieve.",__FILE__,__LINE__);
	mTagUpperBound = *(int*)lValue;

	unsigned int lSize = MPI::COMM_WORLD.Get_size();
	mNodeRank = new UInt(MPI::COMM_WORLD.Get_rank());
	int lRank = (*mNodeRank);
	UIntArray::Handle lPopulationSize = castHandleT<UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));

	unsigned int lNbEvolvers = (*mNbEvolvers);
	int lNbEvaluators = lSize - lNbEvolvers - 1;

	Beagle_ValidateParameterM( lNbEvolvers <= lPopulationSize->size() && lNbEvolvers > 0,
					"hpc.evolver.nb",
					"The number of evolver must between 1 and the number of deme");

	Beagle_ValidateParameterM( lNbEvolvers <= lNbEvaluators  &&  lNbEvaluators > 0,
					"Number of nodes.",
					"There is not enough node for each evolver node to have at least one evaluator node each.");

	mTopology["Supervisor"] = new IntArray;
	mTopology["Any"] = new IntArray;
	mTopology["Parent"] = new IntArray;
	mTopology["Sibling"] = new IntArray;
	mTopology["Child"] = new IntArray;

	mTopology["Supervisor"]->push_back(0);
	mTopology["Any"]->push_back(MPI::ANY_SOURCE);

	mTypesMap["Supervisor"] = 1;
	mTypesMap["Evolver"] = lNbEvolvers;
	mTypesMap["Evaluator"] = lSize - lNbEvolvers - 1;

	if( lRank == 0 ){
		mNodeType = "Supervisor";
		for(int i = 0; i < lPopulationSize->size(); ++i){
			mTopology["Child"]->push_back((i % lNbEvolvers) + 1);
		}
	}
	else if( lRank <= lNbEvolvers){
		mNodeType = "Evolver";
		mTopology["Parent"]->push_back(0);
		int i = lRank + 1;
		for(;  i <= lNbEvolvers; ++i)
			mTopology["Sibling"]->push_back(i);
		for(i = 1; i < lRank; ++i)
			mTopology["Sibling"]->push_back(i);

		for(int j = lRank + lNbEvolvers; j < lSize; j += lNbEvolvers){
			mTopology["Child"]->push_back(j);
		}

		UIntArray::Handle lNewPopulationSize = new UIntArray;
		for(unsigned int k =  lRank - 1; k < lPopulationSize->size(); k += lNbEvolvers){
			lNewPopulationSize->push_back( (*lPopulationSize)[k] );
		}
		(*lPopulationSize) = (*lNewPopulationSize);
	}
	else{
		mNodeType = "Evaluator";
		UIntArray::Handle lNewPopulationSize = new UIntArray;

		if(lRank % lNbEvolvers != 0 ){
			mTopology["Parent"]->push_back(lRank % lNbEvolvers);
		} else {
			mTopology["Parent"]->push_back(lNbEvolvers);
		}
		for(unsigned int i = (*mTopology["Parent"])[0] - 1; i < lPopulationSize->size(); i+= lNbEvolvers){
			lNewPopulationSize->push_back( (*lPopulationSize)[i] );
		}
		(*lPopulationSize) = (*lNewPopulationSize);
	}

	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::init(System& ioSystem)");
}

/*!
 * \brief Hash a string tag to return a unique integer tag.
 * \param inTag Tag string to convert in integer.
 * The integer tag is computed with Beagle::HashString functor.
 * However, MPI Tags are integer, and Beagle::HashString functor
 * produces unsigned integer. Also, the MPI Tags have limited bits.
 * So the function compute a logical AND between the unsigned int tag
 * and the MPI tag upper bound.
 */
int HPC::MPICommunication::hashTag(const std::string& inTag) const
{
	Beagle_StackTraceBeginM();
	return Beagle::HashString()(inTag) & mTagUpperBound;
	Beagle_HPC_StackTraceEndM("int HPC::MPICommunication::hashTag(const std::string&) const");
}

/*!
 * \brief Cancel a non-blocking communication request.
 * \param inRequest The communication receipt of the communication to cancel.
 */
void HPC::MPICommunication::cancel(Request::Handle inRequest) const
{
	Beagle_StackTraceBeginM();
	inRequest->mSizeRequest.Cancel();
	inRequest->mMsgRequest.Cancel();
	inRequest->mCancel = true;
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::cancel(Request& inReceipt) const");
}

/*!
 * \brief Send message to a specific node rank via MPI.
 * \param inMessage Message to send.
 * \param inTag Tag associated to the message.
 * \param inRank Node rank to send message.
 * The function compress the message if the user asked to. It then
 * sends the message size with a MPI blocking Send. Once the size
 * is sent, it sends the message string with a MPI blocking send.
 */
void HPC::MPICommunication::send(const std::string& inMessage, const std::string& inTag, int inRank) const
{
	Beagle_StackTraceBeginM();
#ifdef BEAGLE_HAVE_LIBZ
	if(mCompressionLevel->getWrappedValue() > 0){
		std::string lCompressMsg;
		compressString(inMessage, lCompressMsg, mCompressionLevel->getWrappedValue());
		const int& lSize = lCompressMsg.size();
		MPI::COMM_WORLD.Send(&lSize, 1, MPI::INT, inRank, hashTag(inTag+"_size"));
		MPI::COMM_WORLD.Send(&lCompressMsg[0], lSize, MPI::CHAR, inRank, hashTag(inTag+"_str"));
	} else{
		const int& lSize = inMessage.size();
		MPI::COMM_WORLD.Send(&lSize, 1, MPI::INT, inRank, hashTag(inTag+"_size"));
		MPI::COMM_WORLD.Send(&inMessage[0], lSize, MPI::CHAR, inRank, hashTag(inTag+"_str"));
	}
#else
	const int& lSize = inMessage.size();
	MPI::COMM_WORLD.Send(&lSize, 1, MPI::INT, inRank, hashTag(inTag+"_size"));
	MPI::COMM_WORLD.Send(&inMessage[0], lSize, MPI::CHAR, inRank, hashTag(inTag+"_str"));
#endif // BEAGLE_HAVE_LIBZ
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::send(const std::string&, int, int) const");
}

/*!
 * \brief Receive message from a specific node rank via MPI
 * \param outMessage Message receive.
 * \param inTag Tag associated to the message to be received.
 * \param inRank Node rank of the sending node.
 */
void HPC::MPICommunication::receive(std::string& outMessage, const std::string& inTag, int inRank) const
{
	Beagle_StackTraceBeginM();
	MPI::Status lStatus;

	int lSize = 0;
	MPI::COMM_WORLD.Recv(&lSize, 1, MPI::INT, inRank, hashTag(inTag+"_size"));
	MPI::COMM_WORLD.Probe(inRank,hashTag(inTag+"_str"),lStatus);
	Beagle_AssertM(lStatus.Get_count(MPI::CHAR) == lSize);
	outMessage.resize(lSize);
	MPI::COMM_WORLD.Recv(&outMessage[0], lSize, MPI::CHAR, lStatus.Get_source(), hashTag(inTag+"_str"));

#ifdef BEAGLE_HAVE_LIBZ
	if(mCompressionLevel->getWrappedValue() > 0){
		std::string lString;
		decompressString(outMessage, lString);
		outMessage = lString;
	}
#endif
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::receive(std::string&, const std::string&, int) const");
}

/*!
 * \brief Receive message non blocking from a specific node rank via MPI
 * \param outReq Request corresponding to the non-blocking reception.
 * \param inTag Tag associated to the message.
 * \param inRank Node rank from which message will be received.
 */
void HPC::MPICommunication::receiveNonBlocking(Request::Handle outReq, const std::string& inTag, int inRank) const
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(outReq);
	outReq->mSize = 0;
	outReq->mTag = inTag;
	outReq->mType = Request::RECEPTION;
	std::string lSizeTag = inTag + "_size";
	outReq->mSizeRequest =  MPI::COMM_WORLD.Irecv(&(outReq->mSize), 1, MPI::INT, inRank, hashTag(lSizeTag));
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::receiveNonBlocking(Request::Handle, const std::string&, int) const");
}

/*!
 * \brief Send non blocking message to a specific node rank via MPI
 * \param inMessage Message to send.
 * \param outReq Request corresponding to the non-blocking sending.
 * \param inTag Tag associated to the message.
 * \param inRank Node rank to which message will be sent.
 * The function compress the message \c inMessage if the user asked for compression.
 * It then sends the message size and the message using MPI non blocking send
 * function. The communication Request \c outReq contains both MPI::Request associated
 * to each of the non-blocking send. It also contains the tag, the message and the rank.
 * The request \c outReq can then be used with a wait function to wait until the
 * communication is completed.
 */
void HPC::MPICommunication::sendNonBlocking(const std::string& inMessage, Request::Handle outReq, const std::string& inTag, int inRank) const
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(outReq);
#ifdef BEAGLE_HAVE_LIBZ
	if(mCompressionLevel->getWrappedValue() > 0){
		outReq->mMessage = new Beagle::String(inMessage);
		compressString(inMessage, outReq->mMessage->getWrappedValue(), mCompressionLevel->getWrappedValue());
		outReq->mSize = (int)outReq->mMessage->getWrappedValue().size();
	} else {
		outReq->mSize = (int)inMessage.size();
		outReq->mMessage =  new Beagle::String(inMessage);
	}
#else
	outReq->mSize = (int)inMessage.size();
	outReq->mMessage =  new Beagle::String(inMessage);
#endif
	outReq->mTag = inTag;
	outReq->mType = Request::SENDING;
	std::string lSizeTag = inTag + "_size";
	std::string lStringTag = inTag + "_str";
	outReq->mSizeRequest = MPI::COMM_WORLD.Isend(&(outReq->mSize), 1, MPI::INT, inRank, hashTag(lSizeTag));
	const std::string& lMsgRef = outReq->mMessage->getWrappedValue();
	outReq->mMsgRequest = MPI::COMM_WORLD.Isend(&lMsgRef[0], outReq->mSize, MPI::CHAR, inRank, hashTag(lStringTag));
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::sendNonBlocking(std::string&, Request::Handle, const std::string&, int) const");
}

void HPC::MPICommunication::wait(Request::Handle ioReq) const
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(ioReq);
	switch(ioReq->mType){
		case Request::RECEPTION:
			waitReception(ioReq);
			break;
		case Request::SENDING:
			waitSending(ioReq);
			break;
		default:
			throw RunTimeException("MPI::Communication::wait(Request::Handle) the request is invalid",__FILE__,__LINE__);
			break;
	}
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::wait(Request&) const");
}

void HPC::MPICommunication::waitAll(Request::Bag& ioRequests) const
{
	Beagle_StackTraceBeginM();
	for(unsigned int i = 0; i < ioRequests.size(); ++i){
		Beagle_NonNullPointerAssertM(ioRequests[i]);
		switch(ioRequests[i]->mType){
			case Request::RECEPTION:
				waitReception(ioRequests[i]);
				break;
			case Request::SENDING:
				waitSending(ioRequests[i]);
				break;
			default:
				throw RunTimeException("MPI::Communication::waitAll(Request::Bag&) the request"+uint2str(i)+
						       " is invalid",__FILE__,__LINE__);
				break;
		}
	}
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::waitAll(Request::Bag&) const");
}

int HPC::MPICommunication::waitAny(Request::Bag& ioRequests) const
{
	Beagle_StackTraceBeginM();
	unsigned int lSize = ioRequests.size();
	std::vector<MPI::Request> lRequests;
	lRequests.reserve(lSize);
	for(unsigned int i = 0; i < lSize; ++i){
		lRequests.push_back(ioRequests[i]->mSizeRequest);
	}
	MPI::Status lStatus;
	int lIndex = MPI::Request::Waitany(lSize, &lRequests[0], lStatus);
	ioRequests[lIndex]->mSizeRequest = lRequests[lIndex];
	if(lStatus.Is_cancelled()) return -1;

	if(ioRequests[lIndex]->mType == Request::RECEPTION){
		int lRank = lStatus.Get_source();
		std::string lStringTag = ioRequests[lIndex]->mTag + "_str";
		int lMsgSize = ioRequests[lIndex]->mSize;
		MPI::COMM_WORLD.Probe(lRank,hashTag(lStringTag),lStatus);
		Beagle_AssertM(lStatus.Get_count(MPI::CHAR) == lMsgSize);

		//constructing a string of the right size.
		std::string lMsg(lMsgSize,' ');
		MPI::COMM_WORLD.Recv(&lMsg[0], lMsgSize, MPI::CHAR, lRank, hashTag(lStringTag));
#ifdef BEAGLE_HAVE_LIBZ
		if(mCompressionLevel->getWrappedValue() > 0){
			ioRequests[lIndex]->mMessage = new Beagle::String;
			decompressString(lMsg, ioRequests[lIndex]->mMessage->getWrappedValue());
		} else {
			ioRequests[lIndex]->mMessage = new Beagle::String(lMsg);
		}
#else
		ioRequests[lIndex]->mMessage = new Beagle::String(lMsg);
#endif
	} else if(ioRequests[lIndex]->mType == Request::SENDING){
		ioRequests[lIndex]->mMsgRequest.Wait();
	} else {
		throw RunTimeException("MPI::Communication::waitAll(Request&) the request"+uint2str(lIndex)+
				       " is invalid",__FILE__,__LINE__);
	}
	return lIndex;
	Beagle_HPC_StackTraceEndM("int HPC::MPICommunication::waitAny(Request::Bag&) const");
}

void HPC::MPICommunication::waitReception(Request::Handle ioRequest) const
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(ioRequest);
	MPI::Status lStatus;
	ioRequest->mSizeRequest.Wait(lStatus);
	if(lStatus.Is_cancelled()) return;
	int lRank = lStatus.Get_source();
	int lMsgSize = ioRequest->mSize;
	std::string lStringTag = ioRequest->mTag + "_str";
	MPI::COMM_WORLD.Probe(lRank,hashTag(lStringTag),lStatus);
	Beagle_AssertM(lStatus.Get_count(MPI::CHAR) == lMsgSize);
	//constructing a string of the right size.
	std::string lMsg(lMsgSize, ' ');
	MPI::COMM_WORLD.Recv(&lMsg[0], lMsgSize, MPI::CHAR, lRank, hashTag(lStringTag));
#ifdef BEAGLE_HAVE_LIBZ
	if(mCompressionLevel->getWrappedValue() > 0){
		ioRequest->mMessage = new Beagle::String;
		decompressString(lMsg, ioRequest->mMessage->getWrappedValue());
	} else {
		ioRequest->mMessage = new Beagle::String(lMsg);
	}
#else
	ioRequest->mMessage = new Beagle::String(lMsg);
#endif
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::waitReception(Request::Handle) const");
}

void HPC::MPICommunication::waitSending(Request::Handle ioRequest) const
{
	Beagle_StackTraceBeginM();
	Beagle_NonNullPointerAssertM(ioRequest);
	MPI::Status lStatus;
	ioRequest->mSizeRequest.Wait(lStatus);
	if(lStatus.Is_cancelled()) return;
	ioRequest->mMsgRequest.Wait();
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::waitReception(Request::Handle) const");
}

/*!
 *  \brief register the parameters of this logger.
 *  \param ioSystem Reference to the system to use for initialization.
 *
 *  This method registers the following parameter:
 *   - hpc.node.nbevolver
 */
void HPC::MPICommunication::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
		"Number of Evolver nodes.",
		"UInt",
		"1",
		"Determine the number of evolver nodes that will be used. This number must be between 1 and the number of demes."
		);
		mNbEvolvers = castHandleT<UInt>(
		ioSystem.getRegister().insertEntry("hpc.evolver.nb", new UInt(1), lDescription));
	}
#ifdef BEAGLE_HAVE_LIBZ
	{
		std::ostringstream lOSS;
		lOSS << "Determine the zlib compression level of every messages. 0 = no compression, 9 = max. compression.";
		Register::Description lDescription(
		  "Compression level of messages.",
		  "UInt",
		  "0",
		  lOSS.str()
		);
		mCompressionLevel = castHandleT<UInt>(
		ioSystem.getRegister().insertEntry("hpc.compression.level", new UInt(0), lDescription));
	}
#endif //BEAGLE_HAVE_LIBZ
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::registerParams(System& ioSystem)");
}

#ifdef BEAGLE_HAVE_LIBZ
/*!
 * \brief Compress a STL string using zlib with given compression level and return the binary data.
 * \param inString The string to compress.
 * \param outCompress The string containing the compress data.
 * \param inLevel The compression level.
 * \author Timo Bingmann
 * \ref http://idlebox.net/blogtags/compress
 */
void HPC::MPICommunication::compressString(const std::string& inString, std::string& outCompress, int inLevel) const
{
	Beagle_StackTraceBeginM();
	outCompress.clear();
	z_stream zs; // z_stream is zlib's control structure
	memset(&zs, 0, sizeof(zs));

	if (deflateInit(&zs, inLevel) != Z_OK){
		throw RunTimeException("HPC::MPICommunication::compressString() - deflateInit failed while compressing."
				       ,__FILE__,__LINE__);
	}

	zs.next_in = (Bytef*)inString.data();
	zs.avail_in = inString.size(); // set the z_stream's input

	int ret;
	char outbuffer[32768];

    	// retrieve the compressed bytes blockwise
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);

		ret = deflate(&zs, Z_FINISH);

		if (outCompress.size() < zs.total_out) {
            		// append the block to the output string
			outCompress.append(outbuffer, zs.total_out - outCompress.size());
		}
	} while (ret == Z_OK);

	deflateEnd(&zs);

	if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
		std::ostringstream oss;
		oss << "HPC::MPICommunication::compressString : Exception during zlib compression: (" << ret << ") " << zs.msg;
		throw(std::runtime_error(oss.str()));
	}
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::compressString(const std::string&, std::string&, int) const");
}

/*!
 * \brief Decompress an STL string using zlib and return the original data.
 * \param inCompress The compressed string.
 * \param outString The string containing the uncompress data.
 * \author Timo Bingmann
 * \ref http://idlebox.net/blogtags/compress
 */
void HPC::MPICommunication::decompressString(const std::string& inCompress, std::string& outString) const
{
	Beagle_StackTraceBeginM();
	outString.clear();
	z_stream zs;                        // z_stream is zlib's control structure
	memset(&zs, 0, sizeof(zs));

	if (inflateInit(&zs) != Z_OK){
		throw RunTimeException("HPC::MPICommunication::decompressString : inflateInit failed while decompressing.",__FILE__,__LINE__);
	}

	zs.next_in = (Bytef*)inCompress.data();
	zs.avail_in = inCompress.size();

	int ret;
	char outbuffer[32768];

	// get the decompressed bytes blockwise using repeated calls to inflate
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);

		ret = inflate(&zs, 0);

		if (outString.size() < zs.total_out) {
			outString.append(outbuffer, zs.total_out - outString.size());
		}
	} while (ret == Z_OK);

	inflateEnd(&zs);

	if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
		std::ostringstream oss;
		oss << "HPC::MPICommunication::decompressString : Exception during zlib decompression: ("
		    << ret << ") " << zs.msg;
		throw RunTimeException(oss.str(),__FILE__,__LINE__);
	}
	Beagle_HPC_StackTraceEndM("void HPC::MPICommunication::decompressString(const std::string&, std::string&) const");
}
#endif
