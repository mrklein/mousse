#ifndef CORE_PRIMITIVES_CHARS_WCHAR_HPP_
#define CORE_PRIMITIVES_CHARS_WCHAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   wchar_io.cpp
//   see_also
//   http://en.wikipedia.org/wiki/utf-8
//   http://en.wikibooks.org/wiki/_unicode/_character_reference
#include <cwchar>
#include <string>
namespace mousse
{
class Istream;
class Ostream;
// IOstream Operators 
//- Output wide character (Unicode) as UTF-8
Ostream& operator<<(Ostream&, const wchar_t);
//- Output wide character (Unicode) string as UTF-8
Ostream& operator<<(Ostream&, const wchar_t*);
//- Output wide character (Unicode) string as UTF-8
Ostream& operator<<(Ostream&, const std::wstring&);
}  // namespace mousse
#endif
