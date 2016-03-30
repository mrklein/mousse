// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "switch.hpp"
#include "iostreams.hpp"


// Constructors 
mousse::Switch::Switch(Istream& is)
{
  is >> *this;
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, Switch& s)
{
  token t{is};
  if (!t.good()) {
    is.setBad();
    return is;
  }
  if (t.isLabel()) {
    s = bool(t.labelToken());
  }
  else if (t.isWord()) {
    // allow invalid values, but catch after for correct error message
    Switch sw{t.wordToken(), true};
    if (sw.valid()) {
      s = sw;
    } else {
      is.setBad();
      FATAL_IO_ERROR_IN("operator>>(Istream&, bool/Switch&)", is)
        << "expected 'true/false', 'on/off' ... found " << t.wordToken()
        << exit(FatalIOError);
      return is;
    }
  } else {
    is.setBad();
    FATAL_IO_ERROR_IN("operator>>(Istream&, bool/Switch&)", is)
      << "wrong token type - expected bool, found " << t
      << exit(FatalIOError);
    return is;
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, Switch&)");
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const Switch& s)
{
  os << Switch::names[s.switch_];
  os.check("Ostream& operator<<(Ostream&, const Switch&)");
  return os;
}
