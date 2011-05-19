/*
 *  Open BEAGLE: A Generic Evolutionary Computation Framework in C++
 *  Copyright (C) 2001-2010 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, version 3 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License and GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and GNU General Public License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *  Christian Gagne
 *  Laboratoire de vision et systemes numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec (Quebec), Canada  G1V 0A6
 *  http://vision.gel.ulaval.ca/~cgagne
 *  christian.gagne@gel.ulaval.ca
 *
 */

/*!
 *  \file   Beagle/EC/HistoryMarkOp.cpp
 *  \brief  Source code of class HistoryMarkOp.
 *  \author Alexandre Devert
 *  $Revision: 1.6 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#include "Beagle/EC.hpp"

#include <cstdio>
#include <fstream>

using namespace Beagle;
using namespace Beagle::EC;


/*!
 *  \brief Construct a history mark operator.
 */
HistoryMarkOp::HistoryMarkOp(std::string inName) :
	Operator(inName)
{ }


/*!
 *  \brief Apply the characteristic operation.
 *  \param ioDeme
 *  \param ioContext Context of the evolution.
 */
void HistoryMarkOp::operate(Deme& ioDeme, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	// Mark every individual in the deme
	mark(ioDeme, ioContext);

	Beagle_StackTraceEndM();
}


void HistoryMarkOp::mark(Individual::Bag& ioIndividuals, Context& ioContext)
{
	Beagle_StackTraceBeginM();

	// Get history component
	History::Handle lHistory = castHandleT<History>(ioContext.getSystem().haveComponent("History"));

	// Mark
	if(lHistory != NULL) {
		for (unsigned int i=0; i<ioIndividuals.size(); ++i) {
			lHistory->trace(ioContext, std::vector<HistoryID>(), ioIndividuals[i], getName(), "mark");
		}
	}

	Beagle_StackTraceEndM();
}

