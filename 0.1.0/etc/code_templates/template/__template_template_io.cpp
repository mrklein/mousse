// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "classname.hpp"
#include "io_streams.hpp"
// Constructors 
template<TemplateClassArgument>
mousse::CLASSNAME<TemplateArgument>::CLASSNAME(Istream& is)
:
  base1(is),
  base2(is),
  member1(is),
  member2(is)
{
  // Check state of Istream
  is.check("mousse::CLASSNAME<TemplateArgument>::CLASSNAME(mousse::Istream&)");
}
// IOstream Operators 
template<TemplateClassArgument>
mousse::Istream& mousse::operator>>
(
  Istream& is,
  CLASSNAME<TemplateArgument>&
)
{
  // Check state of Istream
  is.check
  (
    "mousse::Istream& mousse::operator>>"
    "(mousse::Istream&, mousse::CLASSNAME<TemplateArgument>&)"
  );
  return is;
}
template<TemplateClassArgument>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const CLASSNAME<TemplateArgument>&
)
{
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<"
    "(Ostream&, const CLASSNAME<TemplateArgument>&)"
  );
  return os;
}
