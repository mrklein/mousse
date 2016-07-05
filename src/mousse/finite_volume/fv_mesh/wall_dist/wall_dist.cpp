// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_dist.hpp"
#include "wall_poly_patch.hpp"
#include "pstream.hpp"
#include "time.hpp"

// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(wallDist, 0);
}
// Private Member Functions
void mousse::wallDist::constructn() const
{
  n_ = tmp<volVectorField>
  (
    new volVectorField
    (
      IOobject
      (
        "n" & patchTypeName_,
        mesh().time().timeName(),
        mesh()
      ),
      mesh(),
      dimensionedVector("n" & patchTypeName_, dimless, vector::zero),
      patchDistMethod::patchTypes<vector>(mesh(), patchIDs_)
    )
  );
  const fvPatchList& patches = mesh().boundary();
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs_, iter)
  {
    label patchi = iter.key();
    n_().boundaryField()[patchi] == patches[patchi].nf();
  }
}
// Constructors
mousse::wallDist::wallDist(const fvMesh& mesh, const word& patchTypeName)
:
  MeshObject<fvMesh, mousse::UpdateableMeshObject, wallDist>(mesh),
  patchIDs_(mesh.boundaryMesh().findPatchIDs<wallPolyPatch>()),
  patchTypeName_(patchTypeName),
  pdm_
  (
    patchDistMethod::New
    (
      static_cast<const fvSchemes&>(mesh)
     .subDict(patchTypeName_ & "Dist"),
      mesh,
      patchIDs_
    )
  ),
  y_
  (
    IOobject
    (
      "y" & patchTypeName_,
      mesh.time().timeName(),
      mesh
    ),
    mesh,
    dimensionedScalar("y" & patchTypeName_, dimLength, SMALL),
    patchDistMethod::patchTypes<scalar>(mesh, patchIDs_)
  ),
  nRequired_
  (
    static_cast<const fvSchemes&>(mesh).subDict(patchTypeName_ & "Dist")
   .lookupOrDefault<Switch>("nRequired", false)
  ),
  n_(volVectorField::null())
{
  if (nRequired_)
  {
    constructn();
  }
  movePoints();
}
mousse::wallDist::wallDist
(
  const fvMesh& mesh,
  const labelHashSet& patchIDs,
  const word& patchTypeName
)
:
  MeshObject<fvMesh, mousse::UpdateableMeshObject, wallDist>(mesh),
  patchIDs_(patchIDs),
  patchTypeName_(patchTypeName),
  pdm_
  (
    patchDistMethod::New
    (
      static_cast<const fvSchemes&>(mesh)
     .subDict(patchTypeName_ & "Dist"),
      mesh,
      patchIDs_
    )
  ),
  y_
  (
    IOobject
    (
      "y" & patchTypeName_,
      mesh.time().timeName(),
      mesh
    ),
    mesh,
    dimensionedScalar("y" & patchTypeName_, dimLength, SMALL),
    patchDistMethod::patchTypes<scalar>(mesh, patchIDs_)
  ),
  nRequired_
  (
    static_cast<const fvSchemes&>(mesh).subDict(patchTypeName_ & "Dist")
   .lookupOrDefault<Switch>("nRequired", false)
  ),
  n_(volVectorField::null())
{
  if (nRequired_)
  {
    constructn();
  }
  movePoints();
}

// Destructor
mousse::wallDist::~wallDist()
{}

// Member Functions
const mousse::volVectorField& mousse::wallDist::n() const
{
  if (isNull(n_()))
  {
    WARNING_IN("mousse::wallDist::n()")
      << "n requested but 'nRequired' not specified in the "
      << (patchTypeName_ & "Dist") << " dictionary" << nl
      << "    Recalculating y and n fields." << endl;
    nRequired_ = true;
    constructn();
    pdm_->correct(y_, n_());
  }
  return n_();
}
bool mousse::wallDist::movePoints()
{
  if (pdm_->movePoints())
  {
    if (nRequired_)
    {
      return pdm_->correct(y_, n_());
    }
    else
    {
      return pdm_->correct(y_);
    }
  }
  else
  {
    return false;
  }
}
void mousse::wallDist::updateMesh(const mapPolyMesh& mpm)
{
  pdm_->updateMesh(mpm);
  movePoints();
}
