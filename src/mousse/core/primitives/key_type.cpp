// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "key_type.hpp"
#include "reg_exp.hpp"
#include "iostreams.hpp"


// Static Data Members
const mousse::keyType mousse::keyType::null;


// Constructors 
mousse::keyType::keyType(Istream& is)
:
  word{},
  isPattern_{false}
{
  is  >> *this;
}


// Member Functions 
bool mousse::keyType::match
(
  const std::string& str,
  bool literalMatch
) const
{
  if (literalMatch || !isPattern_) {
    // check as string
    return (str == *this);
  } else {
    // check as regex
    return regExp(*this).match(str);
  }
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, keyType& kw)
{
  token t{is};
  if (!t.good()) {
    is.setBad();
    return is;
  }
  if (t.isWord()) {
    kw = t.wordToken();
  } else if (t.isString()) {
    // Assign from string. Set as regular expression.
    kw = t.stringToken();
    kw.isPattern_ = true;
    // flag empty strings as an error
    if (kw.empty()) {
      is.setBad();
      FATAL_IO_ERROR_IN("operator>>(Istream&, keyType&)", is)
        << "empty word/expression "
        << exit(FatalIOError);
      return is;
    }
  } else {
    is.setBad();
    FATAL_IO_ERROR_IN("operator>>(Istream&, keyType&)", is)
      << "wrong token type - expected word or string, found "
      << t.info()
      << exit(FatalIOError);
    return is;
  }
  // Check state of IOstream
  is.check("Istream& operator>>(Istream&, keyType&)");
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const keyType& kw)
{
  os.write(kw);
  os.check("Ostream& operator<<(Ostream&, const keyType&)");
  return os;
}
