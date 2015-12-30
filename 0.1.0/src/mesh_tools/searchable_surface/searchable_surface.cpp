// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_surface.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(searchableSurface, 0);
  defineRunTimeSelectionTable(searchableSurface, dict);
}
mousse::autoPtr<mousse::searchableSurface> mousse::searchableSurface::New
(
  const word& searchableSurfaceType,
  const IOobject& io,
  const dictionary& dict
)
{
  dictConstructorTable::iterator cstrIter =
    dictConstructorTablePtr_->find(searchableSurfaceType);
  if (cstrIter == dictConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "searchableSurface::New(const word&,"
      " const IOobject&, const dictionary&)"
    )   << "Unknown searchableSurface type " << searchableSurfaceType
      << endl << endl
      << "Valid searchableSurface types : " << endl
      << dictConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<searchableSurface>(cstrIter()(io, dict));
}
// Constructors 
mousse::searchableSurface::searchableSurface(const IOobject& io)
:
  regIOobject(io)
{}
// Destructor 
mousse::searchableSurface::~searchableSurface()
{}
// Member Functions 
void mousse::searchableSurface::findNearest
(
  const pointField& sample,
  const scalarField& nearestDistSqr,
  List<pointIndexHit>& info,
  vectorField& normal,
  labelList& region
) const
{
  findNearest(sample, nearestDistSqr, info);
  getNormal(info, normal);
  getRegion(info, region);
}
