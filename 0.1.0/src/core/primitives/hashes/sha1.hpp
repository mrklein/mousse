// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SHA1
// Description
//   Functions to compute SHA1 message digest according to the NIST
//   specification FIPS-180-1.
//   Adapted from the gnulib implementation.
// SeeAlso
//   mousse::SHA1Digest
// SourceFiles
//   sha1.cpp
#ifndef sha1_hpp_
#define sha1_hpp_
#include <string>
#include <cstddef>
#include <cstring>
#include "int.hpp"
#include "sha1_digest.hpp"
namespace mousse
{
// Forward declaration of classes
class Ostream;
// Forward declaration of friend functions and operators
class SHA1;
class SHA1Digest;
Ostream& operator<<(Ostream&, const SHA1&);
class SHA1
{
  // Private data
    //- Track if the hashsum has been finalized (added count, etc)
    bool finalized_;
    //- The hash sums
    uint32_t hashsumA_;
    uint32_t hashsumB_;
    uint32_t hashsumC_;
    uint32_t hashsumD_;
    uint32_t hashsumE_;
    //- The total number processed, saved as 64-bit
    uint32_t bufTotal_[2];
    //- The number of elements pending in the buffer
    uint32_t bufLen_;
    //- The input processing buffer
    uint32_t buffer_[32];
  // Private Member Functions
    //- Swap bytes from internal to network (big-endian) order
    static inline uint32_t swapBytes(uint32_t);
    //- Copy the 4-byte value into the memory location pointed to by *dst.
    //  If the architecture allows unaligned access this is equivalent to
    //  *(uint32_t *) cp = val
    static inline void set_uint32(unsigned char *cp, uint32_t);
    //- Process data block-wise, LEN must be a multiple of 64!
    void processBlock(const void *data, size_t len);
    //- Process for the next LEN bytes, LEN need not be a multiple of 64.
    void processBytes(const void *data, size_t len);
    //- Calculate current digest from appended data.
    void calcDigest(SHA1Digest&) const;
public:
  // Constructors
    //- Construct null
    inline SHA1();
    //- Construct null and append initial std::string
    explicit inline SHA1(const std::string&);
    //- Construct null and append initial string
    explicit inline SHA1(const char*);
  // Member Functions
    //- Reset the hashed data before appending more
    void clear();
    //- Append data for processing
    inline SHA1& append(const char* data, size_t len);
    //- Append string for processing
    inline SHA1& append(const std::string&);
    //- Append string for processing
    inline SHA1& append(const char* str);
    //- Finalized the calculations (normally not needed directly).
    //  Returns false if no bytes were passed for processing
    bool finalize();
    //- Calculate current digest from appended data.
    SHA1Digest digest() const;
  // Member Operators
    //- Equality operator, compares %digests
    inline bool operator==(const SHA1&) const;
    //- Compare %digest
    inline bool operator==(const SHA1Digest&) const;
    //- Compare %digest to (40-byte) text representation (eg, from sha1sum)
    //  An %empty string is equivalent to
    //  "0000000000000000000000000000000000000000"
    inline bool operator==(const std::string& hexdigits) const;
    //- Compare %digest to (40-byte) text representation (eg, from sha1sum)
    //  A %null or %empty string is equivalent to
    //  "0000000000000000000000000000000000000000"
    inline bool operator==(const char* hexdigits) const;
    //- Inequality operator, compares %digests
    inline bool operator!=(const SHA1&) const;
    //- Inequality operator, compare %digest
    inline bool operator!=(const SHA1Digest&) const;
    //- Inequality operator, compares %digests
    inline bool operator!=(const std::string& hexdigits) const;
    //- Inequality operator, compare %digest
    inline bool operator!=(const char* hexdigits) const;
    //- Convert to a SHA1Digest,
    //  calculate current %digest from appended data
    inline operator SHA1Digest() const;
  // Friend Operators
    //- Output the %digest
    inline friend Ostream& operator<<(Ostream&, const SHA1&);
};
}  // namespace mousse

// Constructors 
inline mousse::SHA1::SHA1()
{
  clear();
}
inline mousse::SHA1::SHA1(const std::string& str)
{
  clear();
  append(str);
}
inline mousse::SHA1::SHA1(const char* str)
{
  clear();
  append(str);
}
// Member Functions 
inline mousse::SHA1& mousse::SHA1::append(const char* data, size_t len)
{
  processBytes(data, len);
  return *this;
}
inline mousse::SHA1& mousse::SHA1::append(const std::string& str)
{
  processBytes(str.data(), str.size());
  return *this;
}
inline mousse::SHA1& mousse::SHA1::append(const char* str)
{
  if (str)
  {
    processBytes(str, strlen(str));
  }
  return *this;
}
// Member Operators 
inline bool mousse::SHA1::operator==(const SHA1& rhs) const
{
  return this->digest() == rhs.digest();
}
inline bool mousse::SHA1::operator==(const SHA1Digest& rhs) const
{
  return this->digest() == rhs;
}
inline bool mousse::SHA1::operator==(const std::string& hexdigits) const
{
  return this->digest() == hexdigits;
}
inline bool mousse::SHA1::operator==(const char* hexdigits) const
{
  return this->digest() == hexdigits;
}
inline bool mousse::SHA1::operator!=(const SHA1& rhs) const
{
  return !this->operator==(rhs);
}
inline bool mousse::SHA1::operator!=(const SHA1Digest& rhs) const
{
  return !this->operator==(rhs);
}
inline bool mousse::SHA1::operator!=(const std::string& rhs) const
{
  return !this->operator==(rhs);
}
inline bool mousse::SHA1::operator!=(const char* rhs) const
{
  return !this->operator==(rhs);
}
inline mousse::SHA1::operator mousse::SHA1Digest() const
{
  return digest();
}
// Ostream Operator 
inline mousse::Ostream& mousse::operator<<(Ostream& os, const SHA1& sha)
{
  return os  << sha.digest();
}
#endif
