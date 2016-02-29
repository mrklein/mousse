#ifndef CORE_PRIMITIVES_HASHES_HASHER_INT_HPP_
#define CORE_PRIMITIVES_HASHES_HASHER_INT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "hasher.hpp"
#include "int.hpp"
namespace mousse
{
  //- An optimized version of Hasher
  //  \param[in] data - an array of uint32_t values
  //  \param[in] length - the number of values (not bytes)
  //  \param[in] seed - the previous hash, or an arbitrary value
  unsigned HasherInt(const uint32_t* data, size_t length, unsigned seed = 0);
  //- An optimized version of Hasher, returning dual hash values
  //  \param[in] data - an array of uint32_t values
  //  \param[in] length - the number of values (not bytes)
  //  \param[in] hash1 - the previous hash, or an arbitrary value
  //    on output, the primary hash value
  //  \param[in] hash1 - the previous hash, or an arbitrary value
  //    on output, the secondary hash value
  unsigned HasherDual
  (
    const uint32_t* data,
    size_t length,
    unsigned& hash1,
    unsigned& hash2
  );
}  // namespace mousse
#endif
