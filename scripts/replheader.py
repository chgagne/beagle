#!/usr/bin/python

import os, sys, re

usage = "usage: %s [infile [outfile]]" % os.path.basename(sys.argv[0])

matchtext="""\/\*.*Open BEAGLE.*Copyright.*Laboratoire de Vision et Systemes Numeriques.*http:\/\/vision\.gel\.ulaval\.ca"""

newheader="""/*
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
 *  christian.gagne@gel.ulaval.ca"""

input = sys.stdin
if len(sys.argv) > 1:
       input = open(sys.argv[1])


reg = re.compile(matchtext, re.S)
inputstr = ""
for s in input:
       inputstr += s
if len(sys.argv) > 1:
       input.close()

outputstr = reg.sub(newheader, inputstr)

output = sys.stdout
if len(sys.argv) > 2:
       output = open(sys.argv[2], 'w')
output.write(outputstr)
if len(sys.argv) > 2:
       output.close()

