/*
 *  SQLiteQuery.hpp
 *  Copyright (C) 2003-2004 by Marc Dubreuil
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
#ifndef __SQLQUERY_HPP__
#define __SQLQUERY_HPP__

#include <string>
#include <vector>
#include <map>
#include "sqlite.h"


/*!
*  \class SQLQuery SQLiteQuery.hpp "SQLiteQuery.hpp"
*  \brief SQLQuery implementation of sqlite
*
*  This class facilitate communications wiht the database sqlite. This classes should be protected by mutex
*  since 2 or more threads can modify the returned value of the database.
*/

class SQLQuery
{
public:

	/*! \brief Constructor with arguments:
	 *
	 *  \param inDBName: name of the database file
	 */
	SQLQuery(std::string inDBName);

	/*! \brief Destructor
	 */
	~SQLQuery();

	/*! \brief Open the connection with the sqlite server
	 *  \throw runtime_error No name specified for the database
	 */
	void openConnection(void);

	/*! \brief Close the connection with the server
	 */
	void closeConnection(void);

	/*! \brief Execute the SQL query
	 *  \param inQuery: SQL query.  Can be inside transactions.
	 *  \throw runtime_error Problem while querying the database
	 */
	void executeQuery(const std::string& inQuery);

	/*! \brief Return the primary key of the last insert row id
	 */
	int getLastInsertID();

	/*! \brief Return the number of lines in the result vector returned by SQLite.
	 */
	unsigned int getNumberLines()
	{
		return mResult.size();
	}

	/*! \brief Return the number of database rows that were changed with the last call to execute
	 */
	unsigned int getNumberRows();

	/*! \brief After a query, return the row name of a line number and column number.
	 *
	 *  \throw error if inLineNumber or inColumnNumber are invalid.
	 */
	std::string getRowName(const unsigned int inLineNumber, const unsigned int inColumnNumber);

	/*! \brief After a query, return the value of a given line number and column name.
	 *
	 *  \throw error if inLineNumber or inColumnName are invalid.
	 */
	std::string* getValue(const unsigned int inLineNumber, const std::string& inColumnName);

	/*! \brief Set permanently the number of pages SQlite will hold in memory.
	 *  Each page is 1.5 kB.  Default value of SQlite is 2000 pages.
	 */
	void setCacheSize(int inPages = 2000);

private:

	/*! \brief A callback function used by SQLite.
	*
	*  \param inNArg: number of arguments returned by SQLite.
	*  \param inAzArg: value returned by SQLite for a given column name.
	*  \param inAzCol: column name returned by SQLite.
	*/
	static int callback(void *inPtrArg, int inNArg, char **inAzArg, char **inAzCol);

	/*! \brief Static container of returned values of the query.  The key's map are the name of the column.
	*
	*  To get result: *(lDatabaseObject.getValue(Index,ColumnName))
	*/
	static std::vector<std::map<std::string, std::string> > mResult;

	/*! \brief Sqlite database returned when connecting to a database.
	*/
	sqlite* mDB;

	/*! \brief Database's name.
	*/
	std::string mDBName;

	/*! if the connection to the DB is active
	*/
	bool mIsActive;

	// deactivation of the default constructor
	SQLQuery();
};

#endif
