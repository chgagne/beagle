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
 *  \file   Beagle/Core/Package.cpp
 *  \brief  Source code of class Package.
 *  \author Christian Gagne
 *  $Revision: 1.8 $
 *  $Date: 2007/09/20 14:46:33 $
 */

#include "Beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with basic Open BEAGLE objects.
 */
Package::Package(void) :
	PackageAbstract("Package")
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void Package::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::Bool", new Bool::Alloc);
	lFactory.insertAllocator("Beagle::BoolArray", new BoolArray::Alloc);
	lFactory.insertAllocator("Beagle::Char", new Char::Alloc);
	lFactory.insertAllocator("Beagle::Container", new Container::Alloc);
	lFactory.insertAllocator("Beagle::Context", new Context::Alloc);
	lFactory.insertAllocator("Beagle::DataSetClassification", new DataSetClassification::Alloc);
	lFactory.insertAllocator("Beagle::DataSetRegression", new DataSetRegression::Alloc);
	lFactory.insertAllocator("Beagle::Deme", new Deme::Alloc);
	lFactory.insertAllocator("Beagle::Double", new Double::Alloc);
	lFactory.insertAllocator("Beagle::DoubleArray", new DoubleArray::Alloc);
	lFactory.insertAllocator("Beagle::Evolver", new Evolver::Alloc);
	lFactory.insertAllocator("Beagle::Factory", new Factory::Alloc);
	lFactory.insertAllocator("Beagle::Float", new Float::Alloc);
	lFactory.insertAllocator("Beagle::FloatArray", new FloatArray::Alloc);
	lFactory.insertAllocator("Beagle::Individual", new Individual::Alloc);
	lFactory.insertAllocator("Beagle::Int", new Int::Alloc);
	lFactory.insertAllocator("Beagle::IntArray", new IntArray::Alloc);
	lFactory.insertAllocator("Beagle::LoggerXML", new LoggerXML::Alloc);
	lFactory.insertAllocator("Beagle::Long", new Long::Alloc);
	lFactory.insertAllocator("Beagle::LongArray", new LongArray::Alloc);
	lFactory.insertAllocator("Beagle::Map", new Map::Alloc);
	lFactory.insertAllocator("Beagle::Matrix", new Matrix::Alloc);
	lFactory.insertAllocator("Beagle::Package", new Package::Alloc);
	lFactory.insertAllocator("Beagle::QuasiRandom", new QuasiRandom::Alloc);
	lFactory.insertAllocator("Beagle::Randomizer", new Randomizer::Alloc);
	lFactory.insertAllocator("Beagle::Register", new Register::Alloc);
	lFactory.insertAllocator("Beagle::Short", new Short::Alloc);
	lFactory.insertAllocator("Beagle::ShortArray", new ShortArray::Alloc);
	lFactory.insertAllocator("Beagle::Stats", new Stats::Alloc);
	lFactory.insertAllocator("Beagle::String", new String::Alloc);
	lFactory.insertAllocator("Beagle::System", new System::Alloc);
	lFactory.insertAllocator("Beagle::UChar", new UChar::Alloc);
	lFactory.insertAllocator("Beagle::UInt", new UInt::Alloc);
	lFactory.insertAllocator("Beagle::UIntArray", new UIntArray::Alloc);
	lFactory.insertAllocator("Beagle::ULong", new ULong::Alloc);
	lFactory.insertAllocator("Beagle::ULongArray", new ULongArray::Alloc);
	lFactory.insertAllocator("Beagle::UShort", new UShort::Alloc);
	lFactory.insertAllocator("Beagle::UShortArray", new UShortArray::Alloc);
	lFactory.insertAllocator("Beagle::Vector", new Vector::Alloc);
	lFactory.insertAllocator("Beagle::Vivarium", new Vivarium::Alloc);

	// Set aliases
	lFactory.aliasAllocator("Beagle::Bool", "Bool");
	lFactory.aliasAllocator("Beagle::BoolArray", "BoolArray");
	lFactory.aliasAllocator("Beagle::Char", "Char");
	lFactory.aliasAllocator("Beagle::Container", "Container");
	lFactory.aliasAllocator("Beagle::Context", "Context");
	lFactory.aliasAllocator("Beagle::DataSetClassification", "DataSetClassification");
	lFactory.aliasAllocator("Beagle::DataSetRegression", "DataSetRegression");
	lFactory.aliasAllocator("Beagle::Deme", "Deme");
	lFactory.aliasAllocator("Beagle::Double", "Double");
	lFactory.aliasAllocator("Beagle::DoubleArray", "DoubleArray");
	lFactory.aliasAllocator("Beagle::Evolver", "Evolver");
	lFactory.aliasAllocator("Beagle::Factory", "Factory");
	lFactory.aliasAllocator("Beagle::Float", "Float");
	lFactory.aliasAllocator("Beagle::FloatArray", "FloatArray");
	lFactory.aliasAllocator("Beagle::HallOfFame", "HallOfFame");
	lFactory.aliasAllocator("Beagle::Individual", "Individual");
	lFactory.aliasAllocator("Beagle::Int", "Int");
	lFactory.aliasAllocator("Beagle::IntArray", "IntArray");
	lFactory.aliasAllocator("Beagle::LoggerXML", "LoggerXML");
	lFactory.aliasAllocator("Beagle::Long", "Long");
	lFactory.aliasAllocator("Beagle::LongArray", "LongArray");
	lFactory.aliasAllocator("Beagle::Map", "Map");
	lFactory.aliasAllocator("Beagle::Matrix", "Matrix");
	lFactory.aliasAllocator("Beagle::Package", "Package");
	lFactory.aliasAllocator("Beagle::QuasiRandom", "QuasiRandom");
	lFactory.aliasAllocator("Beagle::Randomizer", "Randomizer");
	lFactory.aliasAllocator("Beagle::Register", "Register");
	lFactory.aliasAllocator("Beagle::Short", "Short");
	lFactory.aliasAllocator("Beagle::ShortArray", "ShortArray");
	lFactory.aliasAllocator("Beagle::Stats", "Stats");
	lFactory.aliasAllocator("Beagle::String", "String");
	lFactory.aliasAllocator("Beagle::UChar", "UChar");
	lFactory.aliasAllocator("Beagle::UInt", "UInt");
	lFactory.aliasAllocator("Beagle::UIntArray", "UIntArray");
	lFactory.aliasAllocator("Beagle::ULong", "ULong");
	lFactory.aliasAllocator("Beagle::ULongArray", "ULongArray");
	lFactory.aliasAllocator("Beagle::UShort", "UShort");
	lFactory.aliasAllocator("Beagle::UShortArray", "UShortArray");
	lFactory.aliasAllocator("Beagle::Vector", "Vector");
	lFactory.aliasAllocator("Beagle::Vivarium", "Vivarium");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("Vivarium", "Vivarium");
	lFactory.setConcept("Deme", "Deme");
	lFactory.setConcept("Individual", "Individual");
	lFactory.setConcept("Fitness", "FitnessSimple");
	lFactory.setConcept("Stats", "Stats");
	lFactory.setConcept("HallOfFame", "HallOfFame");
	lFactory.setConcept("Context", "Context");
	lFactory.setConcept("Factory", "Factory");
	lFactory.setConcept("Logger", "LoggerXML");
	lFactory.setConcept("Randomizer", "Randomizer");
	lFactory.setConcept("Register", "Register");
	lFactory.setConcept("System", "System");

	Beagle_StackTraceEndM();
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  Package doesn't depend on any other packages, it is the basic package of all the
    others.
 */
PackageAbstract::Bag Package::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	return Package::Bag();
	Beagle_StackTraceEndM();
}
