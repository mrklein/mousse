#ifndef CORE_PRIMITIVES_CHARS_CHAR_HPP_
#define CORE_PRIMITIVES_CHARS_CHAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


namespace mousse {

class Istream;
class Ostream;

char readChar(Istream&);

Istream& operator>>(Istream&, char&);
Ostream& operator<<(Ostream&, const char);
Ostream& operator<<(Ostream&, const char*);

inline bool isspace(char c)
{
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

}  // namespace mousse

#endif
