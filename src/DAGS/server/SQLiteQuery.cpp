/*
 *  SQLiteQuery.cpp
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


#include <stdexcept>
#include <sstream>
#include <vector>
#include "SQLiteQuery.hpp"
#include "PACC/Threading/Thread.hpp" //sleep function

using namespace std;

//static container to hold results
std::vector<std::map<std::string, std::string> > SQLQuery::mResult;

SQLQuery::SQLQuery(std::string inDBName)
		: mDB(NULL),
		mDBName(inDBName),
		mIsActive(false)
{}

SQLQuery::~SQLQuery()
{
	closeConnection();
}

void SQLQuery::openConnection(void)
{
	if(mDBName == "") throw std::runtime_error("SQLQuery::OpenConnection error. No name specified for the database");
	char *lErrmsg;
	mDB = sqlite_open(mDBName.c_str(), 0, &lErrmsg);
	mIsActive = true;
}

/*
** This is the callback routine that the SQLite library
** invokes for each fields of a query result.
*/
int SQLQuery::callback(void *inPtrArg, int inNArg, char **inAzArg, char **inAzCol)
{
	if(inAzArg==0)
		return 0;

	std::map<std::string,std::string> lResult;
	for(int i = 0; i < inNArg; ++i) {
		lResult[inAzCol[i]] = inAzArg[i];
	}
	mResult.push_back(lResult);
	return 0;
}

void SQLQuery::closeConnection(void)
{
	if(mIsActive == true) {
		sqlite_close(mDB);
		mIsActive = false;
	}
}

void SQLQuery::executeQuery(const std::string& inQuery)
{
	mResult.clear();

	bool lContinueTrying = true;
	int lReturnedValue = -1;
	while (lContinueTrying) {
		char* zErrMsg;
		lReturnedValue = sqlite_exec(mDB, inQuery.c_str(), callback, NULL, &zErrMsg);
		if(lReturnedValue != SQLITE_BUSY &&
		        lReturnedValue != SQLITE_OK &&
		        lReturnedValue != SQLITE_LOCKED &&
		        zErrMsg !=0) {
			std::ostringstream lOSSerror;
			lOSSerror << lReturnedValue;
			std::string lError;
			lError = "SQLQuery::executeQuery";
			lError += "\nThe error returned by sqlite is: ";
			lError += zErrMsg;
			lError += "\nError Code = " + lOSSerror.str();
			lError += "\nThe Query is: "  + inQuery;
			free(zErrMsg);
			throw std::runtime_error(lError);
		}
		switch (lReturnedValue) {
		case SQLITE_BUSY:
			PACC::Threading::Thread::sleep(0.01);
			break;
		case SQLITE_LOCKED:
			PACC::Threading::Thread::sleep(0.01);
			break;
		case SQLITE_OK:
			lContinueTrying = false; // We're done
			break;
		default:
			//error processing the query
			lContinueTrying = false;
			break;
		}
	}

	if(lReturnedValue > 0) {
		std::ostringstream lOSSerror;
		lOSSerror << lReturnedValue;
		std::string lErrorString;
		lErrorString = "SQLQuery returned an error.\n The SQLite error code is: ";
		lErrorString += lOSSerror.str();
		lErrorString += "\n";
		throw std::runtime_error(lErrorString);
	}
}

int SQLQuery::getLastInsertID()
{
	return sqlite_last_insert_rowid(mDB);
}

unsigned int SQLQuery::getNumberRows()
{
	return sqlite_changes(mDB);
}

std::string SQLQuery::getRowName(const unsigned int inLineNumber, const unsigned int inColumnNumber)
{
	if(inLineNumber > mResult.size()) {
		ostringstream lOSSError;
		lOSSError << "SQLQuery getValue() returned an error.\n" << endl
		<< "inLineNumber (" << inLineNumber << ") is greater than result obtained by SQLite ("
		<< mResult.size() << ")."
		<< "\n";
		throw std::runtime_error(lOSSError.str());
	}
	if(inColumnNumber > mResult[inLineNumber].size()) {
		ostringstream lOSSError;
		lOSSError << "SQLQuery getValue() returned an error.\n" << endl
		<< "inColumnNumber (" << inColumnNumber << ") is greater than result obtained by SQLite ("
		<< mResult[inLineNumber].size() << ")."
		<< "\n";
		throw std::runtime_error(lOSSError.str());
	}

	map<std::string, std::string>::iterator lIteratorResult = mResult[inLineNumber].begin();
	for(unsigned int i = 0; i < inColumnNumber; ++i) {
		++lIteratorResult;
	}
	return lIteratorResult->first;
}


std::string* SQLQuery::getValue(const unsigned int inLineNumber, const std::string& inColumnName)
{
	if(inLineNumber > mResult.size()) {
		ostringstream lOSSError;
		lOSSError << "SQLQuery getValue() returned an error.\n" << endl
		<< "inLineNumber (" << inLineNumber << ") is greater than result obtained by SQLite ("
		<< mResult.size() << ")."
		<< "\n";
		throw std::runtime_error(lOSSError.str());
	}

	if(mResult[inLineNumber].find(inColumnName) == mResult[inLineNumber].end()) {
		ostringstream lOSSError;
		lOSSError << "SQLQuery getValue() returned an error." << endl
		<< "inColumnName (" << inColumnName << ") could not be found in the result obtained by SQLite. "
		<< "Possible values are:";
		map<std::string, std::string>::iterator lIteratorResult = mResult[inLineNumber].begin();
		while(lIteratorResult != mResult[inLineNumber].end()) {
			lOSSError << " " << lIteratorResult->first;
			++lIteratorResult;
		}
		lOSSError << ".\n";
		throw std::runtime_error(lOSSError.str());
	}

	return &mResult[inLineNumber][inColumnName];
}

void SQLQuery::setCacheSize(int inPages)
{
	char* zErrMsg;
	std::ostringstream lOSS;
	lOSS << "PRAGMA default_cache_size=" << inPages << ";";
	sqlite_exec(mDB, lOSS.str().c_str(), callback, NULL, &zErrMsg);
}

