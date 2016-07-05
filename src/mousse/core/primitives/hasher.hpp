#ifndef CORE_PRIMITIVES_HASHES_HASHER_HPP_
#define CORE_PRIMITIVES_HASHES_HASHER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include <cstddef>


namespace mousse {

//- Bob Jenkins's 96-bit mixer hashing function (lookup3)
//  \param[in] data - a character stream
//  \param[in] len  - the number of bytes
//  \param[in] seed - the previous hash, or an arbitrary value
unsigned Hasher(const void* data, size_t len, unsigned seed = 0);

}  // namespace mousse

#endif
