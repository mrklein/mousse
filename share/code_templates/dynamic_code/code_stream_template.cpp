// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dictionary.hpp"
#include "ostream.hpp"
#include "pstream.hpp"
#include "unit_conversion.hpp"
//{{{ begin codeInclude
${codeInclude}
//}}} end codeInclude
namespace mousse
{
// Local Functions
//{{{ begin localCode
${localCode}
//}}} end localCode
// Global Functions 
extern "C"
{
  void ${typeName}
  (
    Ostream& os,
    const dictionary& dict
  )
  {
//{{{ begin code
    ${code}
//}}} end code
  }
}
}  // namespace mousse
