// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "word.hpp"
#include "ostream.hpp"
#include "token.hpp"
#include "key_type.hpp"
#include "iostreams.hpp"


// Member Functions 
// Decrement the indent level
void mousse::Ostream::decrIndent()
{
  if (indentLevel_ == 0) {
    cerr << "Ostream::decrIndent() : attempt to decrement 0 indent level"
      << std::endl;
  } else {
    indentLevel_--;
  }
}


// Write keyType
// write regular expression as quoted string
// write plain word as word (unquoted)
mousse::Ostream& mousse::Ostream::write(const keyType& kw)
{
  return writeQuoted(kw, kw.isPattern());
}


// Write the keyword followed by appropriate indentation
mousse::Ostream& mousse::Ostream::writeKeyword(const keyType& kw)
{
  indent();
  write(kw);
  label nSpaces = entryIndentation_ - label(kw.size());
  // pattern is surrounded by quotes
  if (kw.isPattern()) {
    nSpaces -= 2;
  }
  // could also increment by indentSize_ ...
  if (nSpaces < 1) {
    nSpaces = 1;
  }
  while (nSpaces--) {
    write(char(token::SPACE));
  }
  return *this;
}
