// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Hash
// Description
//   Hash function class for primitives.  All non-primitives used to hash
//   entries on hash tables likely need a specialized version of this class.
#ifndef hash_hpp_
#define hash_hpp_
#include "label.hpp"
#include "ulabel.hpp"
#include "hasher.hpp"
#include "ptraits.hpp"
#include "file_name.hpp"
#include "word_re.hpp"
namespace mousse
{
//template<class Type> class Hash;
//template<> class Hash<label>;
template<class PrimitiveType>
class Hash
{
public:
  Hash()
  {}
  unsigned operator()(const PrimitiveType& p, unsigned seed) const
  {
    return Hasher(&p, sizeof(p), seed);
  }
  unsigned operator()(const PrimitiveType& p) const
  {
    return Hasher(&p, sizeof(p));
  }
};
//- Hash specialization for hashing labels
template<>
class Hash<mousse::label>
{
public:
  Hash()
  {}
  //- Incrementally hash a label.
  //  This will necessarily return a different value than the
  //  non-incremental version.
  unsigned operator()(const label p, unsigned seed) const
  {
    return Hasher(&p, sizeof(label), seed);
  }
  //- Return the unsigned representation of a label.
  //  This helps if people have relied on the hash value corresponding to
  //  the natural order.
  unsigned operator()(const label p) const
  {
    return p;
  }
};
//- Hash specialization for hashing strings
template<>
class Hash<mousse::string>
{
public:
  Hash()
  {}
  unsigned operator()(const string& p, unsigned seed) const
  {
    return string::hash()(p, seed);
  }
  unsigned operator()(const string& p) const
  {
    return string::hash()(p);
  }
};
//- Hash specialization for hashing words
template<>
class Hash<mousse::word>
{
public:
  Hash()
  {}
  unsigned operator()(const word& p, unsigned seed) const
  {
    return word::hash()(p, seed);
  }
  unsigned operator()(const word& p) const
  {
    return word::hash()(p);
  }
};
//- Hash specialization for hashing fileNames
template<>
class Hash<mousse::fileName>
{
public:
  Hash()
  {}
  unsigned operator()(const fileName& p, unsigned seed) const
  {
    return fileName::hash()(p, seed);
  }
  unsigned operator()(const fileName& p) const
  {
    return fileName::hash()(p);
  }
};
//- Hash specialization for hashing wordRes
template<>
class Hash<mousse::wordRe>
{
public:
  Hash()
  {}
  unsigned operator()(const wordRe& p, unsigned seed) const
  {
    return wordRe::hash()(p, seed);
  }
  unsigned operator()(const wordRe& p) const
  {
    return wordRe::hash()(p);
  }
};
//- Hash specialization for hashing keyTypes
template<>
class Hash<mousse::keyType>
{
public:
  Hash()
  {}
  unsigned operator()(const keyType& p, unsigned seed) const
  {
    return keyType::hash()(p, seed);
  }
  unsigned operator()(const keyType& p) const
  {
    return keyType::hash()(p);
  }
};
//- Hash specialization for hashing pointer addresses.
//  Treat a pointer like a long.
//  This should work for both 32-bit and 64-bit pointers.
template<>
class Hash<void*>
{
public:
  Hash()
  {}
  unsigned operator()(const void* const& p, unsigned seed) const
  {
    return Hash<long>()(long(p), seed);
  }
  unsigned operator()(const void* const& p) const
  {
    return Hash<long>()(long(p));
  }
};
}  // namespace mousse
#endif
