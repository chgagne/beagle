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
 *  \file   beagle/Core.hpp
 *  \brief  File to include to import core Beagle classes.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.53 $
 *  $Date: 2007/08/29 17:38:31 $
 */

/*!
 *  \namespace Beagle
 *  \brief     Namespace of classes and global functions of the Open BEAGLE framework.
 *  \ingroup OOF
 */

/*!
 *  \mainpage Open BEAGLE Reference Manual
 *
 *  You can use the actual manual in different ways. If you are looking for documentation to get
 *  information on specific element of the Open BEAGLE framework, you can check in the
 *  <A HREF="annotated.html">compound list</A> for a specific class, or the
 *  <A HREF="functions.html">compound member index</A> for a specific method. If you intent to use the
 *  manual in order to get familiar with the framework, start by browsing the
 *  <A HREF="modules.html">modules</A>, especially the examples modules. The different components are
 *  there logically regrouped to facilitate assimilation of the internal framework organization.
 */

/*!
 *  \defgroup OOF Object Oriented Foundations
 *  \brief Classes and functions related to the Object Oriented (OO) foundations, the basis of
 *    the Open BEAGLE architecture.
 */

#ifndef Beagle_Core_hpp
#define Beagle_Core_hpp

// PACC classes
#include "PACC/Math.hpp"
#include "PACC/Util.hpp"
#include "PACC/XML.hpp"

// Configuration and macros
#include "beagle/config.hpp"
#include "beagle/macros.hpp"

// Open MP helper functions
#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
#include "beagle/Core/OpenMP.hpp"
#endif

// Headers of Open BEAGLE core classes and functions
#include "beagle/Core/Object.hpp"
#include "beagle/Core/Pointer.hpp"
#include "beagle/Core/PointerT.hpp"
#include "beagle/Core/castObjectT.hpp"
#include "beagle/Core/Allocator.hpp"
#include "beagle/Core/AbstractAllocT.hpp"
#include "beagle/Core/AllocatorT.hpp"
#include "beagle/Core/NamedObject.hpp"
#include "beagle/Core/Container.hpp"
#include "beagle/Core/ContainerT.hpp"
#include "beagle/Core/HashString.hpp"
#include "beagle/Core/Map.hpp"
#include "beagle/Core/Matrix.hpp"
#include "beagle/Core/Vector.hpp"
#include "beagle/Core/IsEqualMapPairPredicate.hpp"
#include "beagle/Core/IsEqualPointerPredicate.hpp"
#include "beagle/Core/IsLessMapPairPredicate.hpp"
#include "beagle/Core/IsLessPointerPredicate.hpp"
#include "beagle/Core/IsMorePointerPredicate.hpp"
#include "beagle/Core/Exception.hpp"
#include "beagle/Core/AssertException.hpp"
#include "beagle/Core/BadCastException.hpp"
#include "beagle/Core/InternalException.hpp"
#include "beagle/Core/IOException.hpp"
#include "beagle/Core/ObjectException.hpp"
#include "beagle/Core/RunTimeException.hpp"
#include "beagle/Core/TargetedException.hpp"
#include "beagle/Core/ValidationException.hpp"
#include "beagle/Core/WrapperT.hpp"
#include "beagle/Core/ArrayT.hpp"
#include "beagle/Core/Bool.hpp"
#include "beagle/Core/Char.hpp"
#include "beagle/Core/Double.hpp"
#include "beagle/Core/Float.hpp"
#include "beagle/Core/Int.hpp"
#include "beagle/Core/Long.hpp"
#include "beagle/Core/Short.hpp"
#include "beagle/Core/String.hpp"
#include "beagle/Core/UChar.hpp"
#include "beagle/Core/UInt.hpp"
#include "beagle/Core/ULong.hpp"
#include "beagle/Core/UShort.hpp"

#endif //Beagle_Core_hpp

