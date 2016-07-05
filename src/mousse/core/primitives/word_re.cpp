// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "word_re.hpp"
#include "iostreams.hpp"
#include "info_proxy.hpp"


// Static Data Members
const mousse::wordRe mousse::wordRe::null;


// Constructors 
mousse::wordRe::wordRe(Istream& is)
:
  word{},
  re_{nullptr}
{
  is >> *this;
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, wordRe& w)
{
  token t{is};
  if (!t.good()) {
    is.setBad();
    return is;
  }
  if (t.isWord()) {
    w = t.wordToken();
  } else if (t.isString()) {
    // Auto-tests for regular expression
    w = t.stringToken();
    // flag empty strings as an error
    if (w.empty()) {
      is.setBad();
      FATAL_IO_ERROR_IN("operator>>(Istream&, wordRe&)", is)
        << "empty word/expression "
        << exit(FatalIOError);
      return is;
    }
  } else {
    is.setBad();
    FATAL_IO_ERROR_IN("operator>>(Istream&, wordRe&)", is)
      << "wrong token type - expected word or string, found "
      << t.info()
      << exit(FatalIOError);
    return is;
  }
  // Check state of IOstream
  is.check("Istream& operator>>(Istream&, wordRe&)");
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const wordRe& w)
{
  os.writeQuoted(w, w.isPattern());
  os.check("Ostream& operator<<(Ostream&, const wordRe&)");
  return os;
}


mousse::Ostream& mousse::wordRe::info(Ostream& os) const
{
  if (isPattern()) {
    os  << "wordRe(regex) " << *this;
  } else {
    os  << "wordRe(plain) \"" << *this << '"';
  }
  os.flush();
  return os;
}
