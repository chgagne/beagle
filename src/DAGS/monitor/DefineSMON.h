/*
 *  DefineSMON.h
 *  Copyright (C) 2004 by Marc Dubreuil
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

//!This file contain the defaults values.
/*!Contains the defaults value of some variables that need to be defined
  with a value.  It also contains the default name of parameter in file and
  all the name of the tables and fields in database.
 */

#ifndef __DEFINE_SMON_H
#define __DEFINE_SMON_H

#define SMON_DAGS_SERVER_NAME_VERSION_STRING "Simple monitor for DAGS server version 0.9.2"

//name of parameter in parameters file
#define SMON_APPNAME            "ApplicationName"
#define SMON_IPADDRESS          "ServerIP"
#define SMON_PORT               "ServerPort"
#define SMON_LOG_MESSAGE_FILE   "logMessagesFile"
#define SMON_MILESTONE          "milestoneFile"

//default value
#define SMON_DEFAULT_APPNAME          "dbeagle-app"
#define SMON_DEFAULT_IPADDRESS        "127.0.0.1"
#define SMON_DEFAULT_PORT              9123
#define SMON_DEFAULT_LOG_MESSAGE_FILE "DAGSMonitor.log"
#define SMON_DEFAULT_MILESTONE        "milestone.obm"


#endif //__DEFINE_SMON_H

