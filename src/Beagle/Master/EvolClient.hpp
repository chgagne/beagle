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
 *  \file   beagle/Distrib/Master/EvolClient.hpp
 *  \brief  Definition of class EvolClient.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Marc Dubreuil <mdubreuil@gmail.com>
 *  $Revision: 1.4 $
 *  $Date: 2007/08/08 19:26:33 $
 */

#ifndef Beagle_Distrib_Master_EvolClient_hpp
#define Beagle_Distrib_Master_EvolClient_hpp

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "PACC/Util/SignalHandler.hpp"
#include "dags/Client.hpp"
#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Deme.hpp"
#include "beagle/Distrib/Master/EnvironmentManager.hpp"


namespace Beagle
{
namespace Distrib
{
namespace Master
{

/*!
 *  \class EvolClient beagle/Distrib/EvolClient.hpp "beagle/Distrib/EvolClient.hpp"
 *  \brief Communication's layer of a cruncher client.
 *  \ingroup DBMS
 *
 *  This class interface with the DAGS library.  It translate jobs into
 *  individuals that Beagle can understand. It sends individuals to be
 *  evaluated to the DAGS server.
 */
class EvolClient : public DAGS::Client, public SignalHandler
{

public:

	static EvolClient* getSingleton();
	static void        logError(std::string inMessage);
	static void        setSingleton(EvolClient* inSingleton);
	virtual void       main(PACC::SignalHandler::SignalType inSignal);

	explicit EvolClient(std::string inServerIP="127.0.0.1",
	                    int inServerPort=9123,
	                    unsigned int inCompressLevel=0,
	                    std::string inVersion = "");
	virtual ~EvolClient()
	{ }

	unsigned int getNumberOfDemes();
	void         consultDeme(unsigned int inDemeID);
	void         receiveDeme();
	void         sendAndReceiveDeme();
	void         sendDeme(DAGS::Client::eGroupType inDemeType);
	void         terminate();
	void         updateEnv();

	/*!
	 *  \brief Get the context of the evolution.
	 *  \return Handle of the evolution's context.
	 */
	inline Beagle::Context::Handle getContext() const
	{
		return mContext;
	}

	/*!
	 *  \brief Get the current deme.
	 *  \return Handle of the current deme.
	 */
	inline Beagle::Deme::Handle getDeme() const
	{
		return mDeme;
	}

	/*!
	 *  \brief Get the received deme's id.
	 *  \return deme's id.
	 */
	inline unsigned int getDemeID() const
	{
		return mDemeID;
	}

	/*!
	 *  \brief Get the deme's id of the evolver.
	 *  \return deme's id.
	 */
	inline unsigned int getDemeIDEvol() const
	{
		return mDemeIDEvol;
	}

	/*!
	 *  \brief Get the environment manager.
	 *  \return Handle of the environment manager.
	 */
	inline EnvironmentManager::Handle getEnvManager() const
	{
		return mEnvManager;
	}

	/*!
	 *  \brief Get the received deme's generation number.
	 *  \return deme's generation.
	 */
	inline unsigned int getGeneration() const
	{
		return mGeneration;
	}

	/*!
	 *  \brief Get if the deme's is valid/stored.
	 *  \return true if it is stored, false otherwise.
	 */
	inline bool isDemeStored() const
	{
		return mDemeStored;
	}

	/*!
	 *  \brief Set the context of the evolution.
	 *  \param inContext Handle of the context that will be set.
	 */
	inline void setContext(Beagle::Context::Handle inContext)
	{
		mContext = inContext;
	}

	/*!
	 *  \brief Set the current deme.
	 *  \param inDeme Handle of deme that will be set.
	 */
	inline void setDeme(Beagle::Deme::Handle inDeme)
	{
		mDeme = inDeme;
	}

	/*!
	 *  \brief Set the deme's id for DAGS server
	 *  \param inDemeID deme's id
	 */
	inline void setDemeID(unsigned int inDemeID)
	{
		mDemeID = inDemeID;
	}

	/*!
	 *  \brief Set the deme's id for the evolver
	 *  \param inDemeIDEvol deme's id
	 */
	inline void setDemeIDEvol(unsigned int inDemeIDEvol)
	{
		mDemeIDEvol = inDemeIDEvol;
	}

	/*!
	 *  \brief Set the Environment manager.
	 *  \param inEnvManager Handle of the environment manager.
	 */
	inline void setEnvManager(EnvironmentManager::Handle inEnvManager)
	{
		mEnvManager = inEnvManager;
	}

	/*!
	 *  \brief Set the deme generation for the DAGS server
	 *  \param inGeneration deme's generation.
	 */
	inline void setGeneration(unsigned int inGeneration)
	{
		mGeneration = inGeneration;
	}

protected:

	static EvolClient*         smSingleton;     //!< static instance of the EvolClient

	EnvironmentManager::Handle mEnvManager; //!< Handle to the Environment manager (read/write)
	Beagle::Deme::Handle       mDeme;       //!< Handle to the current deme
	Beagle::Context::Handle    mContext;    //!< Handle to the context of the evolution
	unsigned int               mDemeID;     //!< Deme's id
	unsigned int               mDemeIDEvol; //!< Deme's id of evolver
	unsigned int               mGeneration; //!< Deme's generation
	bool                       mDemeStored; //!< If the deme is valid
};

}
}
}


#endif // Beagle_Distrib_Master_EvolClient_hpp


