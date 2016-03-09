// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_selection.hpp"
#include "fv_mesh.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(faceSelection, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(faceSelection, dictionary);
}
// Constructors 
mousse::faceSelection::faceSelection
(
  const word& name,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  name_{name},
  mesh_{mesh},
  dict_{dict},
  flip_{dict.lookupOrDefault("flip", false)}
{}
// Destructor 
mousse::faceSelection::~faceSelection()
{}
// Member Functions 
mousse::autoPtr<mousse::faceSelection> mousse::faceSelection::New
(
  const word& name,
  const fvMesh& mesh,
  const dictionary& dict
)
{
  const word sampleType{dict.lookup("type")};
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(sampleType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "faceSelection::New"
      "(const word&, const fvMesh&, const dictionary&)"
    )
    << "Unknown faceSelection type "
    << sampleType << nl << nl
    << "Valid faceSelection types : " << endl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<faceSelection>(cstrIter()(name, mesh, dict));
}
void mousse::faceSelection::select
(
  const label zoneID,
  labelList& faceToZoneID,
  boolList& faceToFlip
) const
{
  if (flip_)
  {
    FOR_ALL(faceToZoneID, faceI)
    {
      if (faceToZoneID[faceI] == zoneID)
      {
        faceToFlip[faceI] = !faceToFlip[faceI];
      }
    }
  }
}
