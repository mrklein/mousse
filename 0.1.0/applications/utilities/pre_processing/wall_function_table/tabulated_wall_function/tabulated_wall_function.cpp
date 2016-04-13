// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tabulated_wall_function.hpp"
#include "time.hpp"
// Static Data Members
namespace mousse {
namespace tabulatedWallFunctions {
DEFINE_TYPE_NAME_AND_DEBUG(tabulatedWallFunction, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(tabulatedWallFunction, dictionary);
}
}
// Constructors 
mousse::tabulatedWallFunctions::tabulatedWallFunction::tabulatedWallFunction
(
  const dictionary& dict,
  const polyMesh& mesh,
  const word& name
)
:
  dict_{dict},
  mesh_{mesh},
  coeffDict_{dict.subDict(name + "Coeffs")},
  invertedTableName_{dict.lookup("invertedTableName")},
  invertedTable_{invertedTableName_, mesh_, dict, true}
{}
// Destructor 
mousse::tabulatedWallFunctions::tabulatedWallFunction::~tabulatedWallFunction()
{}
// Member Functions 
void mousse::tabulatedWallFunctions::tabulatedWallFunction::write()
{
  if (invertedTable_.log10())
  {
    invertedTable_.note() =
      "U+ as a function of log10(Re) computed using " + type();
  }
  else
  {
    invertedTable_.note() =
      "U+ as a function of Re computed using " + type();
  }
  Info << "Writing inverted table to\n    " << invertedTable_.objectPath()
    << endl;
  invertedTable_.write();
}
