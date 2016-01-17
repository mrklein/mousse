// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_to_face.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(patchToFace, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, patchToFace, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, patchToFace, istream);
}

mousse::topoSetSource::addToUsageTable mousse::patchToFace::usage_
(
  patchToFace::typeName,
  "\n    Usage: patchToFace patch\n\n"
  "    Select all faces in the patch. Note:accepts wildcards for patch.\n\n"
);

// Private Member Functions 
void mousse::patchToFace::combine(topoSet& set, const bool add) const
{
  labelHashSet patchIDs = mesh_.boundaryMesh().patchSet
  (
    List<wordRe>(1, patchName_),
    true,           // warn if not found
    true            // use patch groups if available
  );
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs, iter)
  {
    label patchI = iter.key();
    const polyPatch& pp = mesh_.boundaryMesh()[patchI];
    Info<< "    Found matching patch " << pp.name()
      << " with " << pp.size() << " faces." << endl;
    for
    (
      label faceI = pp.start();
      faceI < pp.start() + pp.size();
      faceI++
    )
    {
      addOrDelete(set, faceI, add);
    }
  }
  if (patchIDs.empty())
  {
    WARNING_IN("patchToFace::combine(topoSet&, const bool)")
      << "Cannot find any patch named " << patchName_ << endl
      << "Valid names are " << mesh_.boundaryMesh().names() << endl;
  }
}
// Constructors 
// Construct from components
mousse::patchToFace::patchToFace
(
  const polyMesh& mesh,
  const word& patchName
)
:
  topoSetSource(mesh),
  patchName_(patchName)
{}
// Construct from dictionary
mousse::patchToFace::patchToFace
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  patchName_(dict.lookup("name"))
{}
// Construct from Istream
mousse::patchToFace::patchToFace
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  patchName_(checkIs(is))
{}
// Destructor 
mousse::patchToFace::~patchToFace()
{}
// Member Functions 
void mousse::patchToFace::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding all faces of patch " << patchName_ << " ..." << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing all faces of patch " << patchName_ << " ..."
      << endl;
    combine(set, false);
  }
}
