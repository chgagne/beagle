/*
 *  DefineDAGSServer.h
 *  Copyright (C) 2003-2004 by Marc Dubreuil
 *  Copyright (C) 2001-2002 by Marc Dubreuil, Jacques Labrie, Helene Torresan and Frederic Jean
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
 *  Universite Laval, Quebec, Canada,G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file DefineDAGSServer.h DefineDAGSServer.h "DefineDAGSServer.h"
 *  \brief Define of the dags-server
 *  This file contains default values of some variables that need to be defined.
 *  It also contains the default name of parameter in the configuration file and
 *  all the name of the tables and fields in the database.
 *
 *  \warning <li>Do not change this defines unless you know what you are doing!!!</li>
 *  \warning <li>There is no validity check for table and field name, refer to sql doc
 *  for invalid name.</li>
 *  \warning <li>Do not use sql key words (like "group") and/or the same
 *  name for 2 tables or the same name for 2 fields in a table.  This may result
 *  in a crash of the server.</li>
 */


#ifndef __DEFINE_DAGS_SERVER_H
#define __DEFINE_DAGS_SERVER_H

/*! String that will be displayed on screen when the server is launched */
#define DAGS_SERVER_NAME_VERSION_STRING  "DAGS server version 0.9.2"

/*!
 * \defgroup ParametersName Parameter's name in the configuration file
 *
 * @{
 */

/** Parameter's name of the dags-server's port that it will listen on. */
#define DAGS_PORT                      "Port"
/** Parameter's name of the number of max connections allowed by the server. */
#define DAGS_NB_MAX_CONNECTION         "MaxConnections"
/** Parameter's name of the number of max threads that will be used by the server. */
#define DAGS_NB_MAX_THREADS            "MaxThreads"
/** Parameter's name of the degree of verbosity the server will have. */
#define DAGS_VERBOSE                   "Verbose"
/** Parameter's name of the database's filename. */
#define DAGS_DB_NAME                   "DBFileName"
/** Parameter's name of the database's port number (disabled). */
#define DAGS_DB_PORT                   "DBPort"
/** Parameter's name of the database's user name (disabled). */
#define DAGS_DB_USER                   "DBUser"
/** Parameter's name of the database's password (disabled). */
#define DAGS_DB_PASSWORD               "DBPassword"
/** Parameter's name of the database's IP address (disabled). */
#define DAGS_DB_ADDRESS                "DBAddress"
/** Parameter's name of the number of jobs in a subgroup initially (sent to each worker). */
#define DAGS_NUMBER_JOBS_TO_CRUNCHER   "JobsToCruncherPerCycle"
/** Parameter's name if client statistic is enabled. */
#define DAGS_ENABLE_CLIENT_STAT        "EnableLoadBalancing"
/** True's name. */
#define DAGS_TRUE                      "true"
/** False's name. */
#define DAGS_FALSE                     "false"
/** Yes's name. */
#define DAGS_YES                       "yes"
/** No's name. */
#define DAGS_NO                        "no"
/** Parameter's name of the file that logs error messages. */
#define DAGS_LOG_ERROR_FILE            "ErrorsLogFileName"
/** Parameter's name of the file that logs messages (other than error) messages. */
#define DAGS_LOG_MESSAGE_FILE          "MessagesLogFileName"
/** Parameter's name of the expected time between communications */
#define DAGS_EXPECTED_TIME_BETWEEN_COMMS "ExpectedTimeBetweenComms"
/** Parameter's name of the percent needed to wait before sending to another client. */
#define DAGS_CLIENT_TIMEOUT            "Timeout"
/** Parameter's name of the weight of the first stat. */
#define DAGS_CLIENT_STAT_WEIGHT        "HistoryWeight"
/** Parameter's name of the number of stats used. */
#define DAGS_CLIENT_NUMBER_USED_STAT   "HistorySize"
/** Parameter's name if the server is in memory short mode. */
#define DAGS_MEMORY_SHORT              "MemoryShortMode"
/** Parameter's name if the database sync-ing will be done at another value than 100% */
#define DAGS_DB_SYNC                   "DBSyncPercent"
/** Parameter's name if the group will be sync-ing with the database by a group request. */
#define DAGS_GROUP_DB_SYNC             "DBSyncGroup"
/** Parameter's name of the compression level for group communications. */
#define DAGS_GROUP_COMPRESSION         "CompressGroupComms"
/** Parameter's name of the compression level for subgroup communications. */
#define DAGS_SUBGROUP_COMPRESSION      "CompressSubgroupComms"

/*!
 * @}
 */

/*!
 * \defgroup ParametersValue Parameter's value in the configuration file
 *
 * @{
 */

/** dags-server's port that it will listen on. */
#define DAGS_DEFAULT_PORT                       9123
/** Number of max connections allowed by the server. */
#define DAGS_DEFAULT_NB_MAX_CONNECTION          25
/** Number of max threads that will be used by the server. */
#define DAGS_DEFAULT_NB_MAX_THREADS             6
/** Database's port number (disabled). */
#define DAGS_DEFAULT_DB_PORT                    3306
/** Database's IP address (disabled). */
#define DAGS_DEFAULT_DB_ADDRESS                "127.0.0.1"
/** Database's filename. */
#define DAGS_DEFAULT_DB_NAME                   "universe.db"
/** Database's user name (disabled). */
#define DAGS_DEFAULT_DB_USER                   "universe"
/** Database's password (disabled). */
#define DAGS_DEFAULT_DB_PASSWORD               ""
/** Filename that logs error messages. */
#define DAGS_DEFAULT_LOG_ERROR_FILE            "DAGSserver.err"
/** Filename that logs messages (other than errors). */
#define DAGS_DEFAULT_LOG_MESSAGE_FILE          "DAGSserver.log"
/** Number of jobs in a subgroup initially (sent to each worker). */
#define DAGS_DEFAULT_NUMBER_JOBS_TO_CRUNCHER    100
/** If client statistic is enabled. */
#define DAGS_DEFAULT_ENABLE_CLIENT_STAT        "false"
/** Degree of verbosity the server will have. */
#define DAGS_DEFAULT_VERBOSE                    2
/** expected times between communications (in seconds). */
#define DAGS_DEFAULT_EXPECTED_TIME_BETWEEN_COMMS 60
/** Percent needed to wait before sending to another client. */
#define DAGS_DEFAULT_CLIENT_TIMEOUT            200
/** Number of stats used. The sum must be equal to 1 for weight.*/
#define DAGS_DEFAULT_CLIENT_NUMBER_USED_STAT    5
/** If the server is in memory short mode. */
#define DAGS_DEFAULT_MEMORY_SHORT              "false"
/** Database sync-ing will be done at another value than 100% */
#define DAGS_DEFAULT_DB_SYNC                    100
/** If groups will be sync-ing with the database by a group request. */
#define DAGS_DEFAULT_GROUP_DB_SYNC             "true"
/** Compression level for group communications (0-9). -1 means compress if client compress.*/
#define DAGS_DEFAULT_GROUP_COMPRESSION          -1
/** Compression level for subgroup communications (0-9). -1 means compress if client compress.*/
#define DAGS_DEFAULT_SUBGROUP_COMPRESSION       -1

/*!
 * @}
 */

/*!
 * \defgroup GroupsDatabase Name of the GROUPS fields in the database.
 *
 * @{
 */

/** Group's table name. */
#define DAGS_GROUP_TABLE_NAME        "groups"
/** Environment of the group field name. */
#define DAGS_GROUP_FIELD_ENVIRONMENT "environment"
/** Generation of the group field name. */
#define DAGS_GROUP_FIELD_GENERATION  "generation"
/** Group's id of the group field name. */
#define DAGS_GROUP_FIELD_DB_GROUP_ID "groupid"
/** Number of jobs of the group field name. */
#define DAGS_GROUP_FIELD_NB_JOBS   "nbjobs"
/** If the environment must be distributed to subgroup request of the group field name. */
#define DAGS_GROUP_FIELD_DISTRIBUTE  "distributeEnv"
/** Application's name of the group field name. */
#define DAGS_GROUP_FIELD_APPNAME     "appname"
/** Application's version of the group field name. */
#define DAGS_GROUP_FIELD_VERSION     "version"
/** Index of the group field name. */
#define DAGS_GROUP_INDEX             "indexgroup"

/*!
 * @}
 */

/*!
 * \defgroup JobsDatabase Name of the JOBS fields in the database.
 *
 * @{
 */

/** Job's table name. */
#define DAGS_JOBS_TABLE_NAME          "jobs"
/** Group's id of the jobs field name. */
#define DAGS_JOBS_FIELD_DB_GROUP_ID   "groupid"
/** Job's Data id of the jobs field name. */
#define DAGS_JOBS_FIELD_DATA          "jobdata"
/** Job's Score of the jobs field name. */
#define DAGS_JOBS_FIELD_SCORE         "jobscore"
/** Job's id of the jobs field name. */
#define DAGS_JOBS_FIELD_DB_JOB_ID   "jobid"
/** Job's id that was given by the user of the jobs field name. */
#define DAGS_JOBS_FIELD_USER_JOB_ID "userjobid"
/** If the job has an invalid score of the jobs field name. */
#define DAGS_JOBS_FIELD_INVALID_SCORE "invalidscore"
/** Index of the jobs field name. */
#define DAGS_JOBS_INDEX               "indexjob"

/*!
 * @}
 */

#endif //__DEFINE_DAGS_SERVER_H

