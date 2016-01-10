// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sha1.hpp"
#include <cstring>
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
