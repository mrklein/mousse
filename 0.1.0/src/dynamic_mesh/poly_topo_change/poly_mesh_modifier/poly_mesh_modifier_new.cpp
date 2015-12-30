// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh_modifier.hpp"
#include "dictionary.hpp"
// Member Functions 
mousse::autoPtr<mousse::polyMeshModifier> mousse::polyMeshModifier::New
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyTopoChanger& mme
)
{
  if (debug)
  {
    Info<< "polyMeshModifier::New(const word&, const dictionary&, "
      << "const label, const polyTopoChanger&) : "
      << "constructing polyMeshModifier"
      << endl;
  }
  const word modifierType(dict.lookup("type"));
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modifierType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalIOErrorIn
    (
      "polyMeshModifier::New(const word&, const dictionary&, "
      "const label, const polyMesh&)",
      dict
    )   << "Unknown polyMeshModifier type "
      << modifierType << nl << nl
      << "Valid polyMeshModifier types are :" << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalIOError);
  }
  return autoPtr<polyMeshModifier>(cstrIter()(name, dict, index, mme));
}
