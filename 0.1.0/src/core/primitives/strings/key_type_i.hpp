// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::keyType::keyType()
:
  word(),
  isPattern_(false)
{}
inline mousse::keyType::keyType(const keyType& s)
:
  word(s, false),
  isPattern_(s.isPattern())
{}
inline mousse::keyType::keyType(const word& s)
:
  word(s, false),
  isPattern_(false)
{}
inline mousse::keyType::keyType(const string& s)
:
  word(s, false),
  isPattern_(true)
{}
inline mousse::keyType::keyType(const char* s)
:
  word(s, false),
  isPattern_(false)
{}
inline mousse::keyType::keyType
(
  const std::string& s,
  const bool isPattern
)
:
  word(s, false),
  isPattern_(isPattern)
{}
// Member Functions 
inline bool mousse::keyType::isPattern() const
{
  return isPattern_;
}
// Member Operators 
inline const mousse::keyType& mousse::keyType::operator=(const keyType& s)
{
  // Bypass checking
  string::operator=(s);
  isPattern_ = s.isPattern_;
  return *this;
}
inline const mousse::keyType& mousse::keyType::operator=(const word& s)
{
  word::operator=(s);
  isPattern_ = false;
  return *this;
}
inline const mousse::keyType& mousse::keyType::operator=(const string& s)
{
  // Bypass checking
  string::operator=(s);
  isPattern_ = true;
  return *this;
}
inline const mousse::keyType& mousse::keyType::operator=(const char* s)
{
  // Bypass checking
  string::operator=(s);
  isPattern_ = false;
  return *this;
}
