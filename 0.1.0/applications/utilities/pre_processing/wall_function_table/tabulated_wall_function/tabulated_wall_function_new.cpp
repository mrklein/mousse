// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tabulated_wall_function.hpp"


namespace mousse {
namespace tabulatedWallFunctions {

// Constructors 
autoPtr<tabulatedWallFunction> tabulatedWallFunction::New
(
  const dictionary& dict,
  const polyMesh& mesh
)
{
  word twfTypeName = dict.lookup("tabulatedWallFunction");
  Info << "Selecting tabulatedWallFunction " << twfTypeName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(twfTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "tabulatedWallFunction::New(const dictionary&, const polyMesh&)"
    )
    << "Unknown tabulatedWallFunction type " << twfTypeName
    << nl << nl << "Valid tabulatedWallFunction types are:" << nl
    << dictionaryConstructorTablePtr_->toc()
    << exit(FatalError);
  }
  return autoPtr<tabulatedWallFunction>{cstrIter()(dict, mesh)};
}

}  // namespace tabulatedWallFunctions
}  // namespace mousse

