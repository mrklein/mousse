// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_surface_features.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(searchableSurfaceFeatures, 0);
defineRunTimeSelectionTable(searchableSurfaceFeatures, dict);
}
mousse::autoPtr<mousse::searchableSurfaceFeatures>
mousse::searchableSurfaceFeatures::New
(
  const searchableSurface& surface,
  const dictionary& dict
)
{
  word searchableSurfaceFeaturesType = surface.type() + "Features";
  dictConstructorTable::iterator cstrIter =
    dictConstructorTablePtr_->find(searchableSurfaceFeaturesType);
  if (cstrIter == dictConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "searchableSurfaceFeatures::New(const word&,"
      " const searchableSurface&, const dictionary&)"
    )   << "Unknown searchableSurfaceFeatures type "
      << searchableSurfaceFeaturesType << endl << endl
      << "Valid searchableSurfaceFeatures types : " << endl
      << dictConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<searchableSurfaceFeatures>(cstrIter()(surface, dict));
}
// Constructors 
mousse::searchableSurfaceFeatures::searchableSurfaceFeatures
(
  const searchableSurface& surface,
  const dictionary& dict
)
:
  surface_(surface),
  dict_(dict)
{}
// Destructor 
mousse::searchableSurfaceFeatures::~searchableSurfaceFeatures()
{}
// Member Functions
