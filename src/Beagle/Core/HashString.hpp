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
 *  \file   Beagle/Core/HashString.hpp
 *  \brief  Definition of HashString Map.
 *  \author Christian Gagne
 *  $Revision: 1.3 $
 *  $Date: 2007/10/09 17:45:54 $
 */

#ifndef Beagle_Core_HashString_hpp
#define Beagle_Core_HashString_hpp

#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"

#ifdef BEAGLE_HAVE_STDINT_H
#include "stdint.h"
#else // BEAGLE_HAVE_STDINT_H
#define uint8_t  unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#endif // BEAGLE_HAVE_STDINT_H

#ifdef BEAGLE_WORDS_BIGENDIAN
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#else // BEAGLE_WORDS_BIGENDIAN
#define get16bits(d) (*((const uint16_t *) (d)))
#endif // BEAGLE_WORDS_BIGENDIAN

/*
#ifdef BEAGLE_HAVE_STDTR1_UNORDEREDMAP
  #include <tr1/unordered_map>
#else
  #ifdef BEAGLE_HAVE_STD_UNORDEREDMAP
    #include <unordered_map>
  #endif // BEAGLE_HAVE_STD_UNORDEREDMAP
#endif //BEAGLE_HAVE_STDTR1_UNORDEREDMAP
*/
namespace Beagle
{

/*!
 *  \class HashString Beagle/Core/HashString.hpp "Beagle/Core/HashString.hpp"
 *  \brief Functor used to generate hash value from std::string.
 *  \ingroup Core
 *  \ingroup DataStruct
 */

class HashString
{

public:

	HashString()
	{ }

	/*!
	 *  \brief Compute the hash number of a STL string.
	 *  \param inString String to hash
	 *  \return Hash number.
	 *
	 *  This hash function is a light adaptation of the SuperFastHash
	 *  function by Paul Hsieh, available online at
	 *  http://www.azillionmonkeys.com/qed/hash.html.
	 *  Complete copyright of that function is retained by the author.
	 */
	inline unsigned int operator()(const std::string& inString) const {
		const char* data = inString.data();
		int len = (int)inString.size();

		uint32_t hash = len, tmp;
		int rem;

		if (len <= 0 || data == NULL) return 0;

		rem = len & 3;
		len >>= 2;

		/* Main loop */
		for (; len > 0; len--) {
			hash  += get16bits (data);
			tmp    = (get16bits (data+2) << 11) ^ hash;
			hash   = (hash << 16) ^ tmp;
			data  += 2*sizeof (uint16_t);
			hash  += hash >> 11;
		}

		/* Handle end cases */
		switch (rem) {
		case 3:
			hash += get16bits (data);
			hash ^= hash << 16;
			hash ^= data[sizeof (uint16_t)] << 18;
			hash += hash >> 11;
			break;
		case 2:
			hash += get16bits (data);
			hash ^= hash << 11;
			hash += hash >> 17;
			break;
		case 1:
			hash += *data;
			hash ^= hash << 10;
			hash += hash >> 1;
		}

		/* Force "avalanching" of final 127 bits */
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;

		return hash;
	}

};

}

#endif // Beagle_Core_HashString_hpp
