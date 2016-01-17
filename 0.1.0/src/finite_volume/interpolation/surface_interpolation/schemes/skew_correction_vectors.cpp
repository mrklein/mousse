// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "skew_correction_vectors.hpp"
#include "vol_fields.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(skewCorrectionVectors, 0);
}
// Constructors
mousse::skewCorrectionVectors::skewCorrectionVectors(const fvMesh& mesh)
:
  MeshObject<fvMesh, mousse::MoveableMeshObject, skewCorrectionVectors>(mesh),
  skew_(false),
  skewCorrectionVectors_
  (
    IOobject
    (
      "skewCorrectionVectors",
      mesh_.pointsInstance(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    ),
    mesh_,
    dimless
  )
{
  calcSkewCorrectionVectors();
}
mousse::skewCorrectionVectors::~skewCorrectionVectors()
{}
void mousse::skewCorrectionVectors::calcSkewCorrectionVectors()
{
  if (debug)
  {
    Info<< "surfaceInterpolation::calcSkewCorrectionVectors() : "
      << "Calculating skew correction vectors"
      << endl;
  }
  // Set local references to mesh data
  const volVectorField& C = mesh_.C();
  const surfaceVectorField& Cf = mesh_.Cf();
  const surfaceVectorField& Sf = mesh_.Sf();
  const labelUList& owner = mesh_.owner();
  const labelUList& neighbour = mesh_.neighbour();
  FOR_ALL(owner, facei)
  {
    label own = owner[facei];
    label nei = neighbour[facei];
    vector d = C[nei] - C[own];
    vector Cpf = Cf[facei] - C[own];
    skewCorrectionVectors_[facei] =
      Cpf - ((Sf[facei] & Cpf)/(Sf[facei] & d))*d;
  }
  FOR_ALL(skewCorrectionVectors_.boundaryField(), patchI)
  {
    fvsPatchVectorField& patchSkewCorrVecs =
      skewCorrectionVectors_.boundaryField()[patchI];
    if (!patchSkewCorrVecs.coupled())
    {
      patchSkewCorrVecs = vector::zero;
    }
    else
    {
      const fvPatch& p = patchSkewCorrVecs.patch();
      const labelUList& faceCells = p.faceCells();
      const vectorField& patchFaceCentres = Cf.boundaryField()[patchI];
      const vectorField& patchSf = Sf.boundaryField()[patchI];
      const vectorField patchD(p.delta());
      FOR_ALL(p, patchFaceI)
      {
        vector Cpf =
          patchFaceCentres[patchFaceI] - C[faceCells[patchFaceI]];
        patchSkewCorrVecs[patchFaceI] =
          Cpf
         - (
            (patchSf[patchFaceI] & Cpf)/
            (patchSf[patchFaceI] & patchD[patchFaceI])
          )*patchD[patchFaceI];
      }
    }
  }
  scalar skewCoeff = 0.0;
  if (Sf.internalField().size())
  {
    skewCoeff =
      max(mag(skewCorrectionVectors_)*mesh_.deltaCoeffs()).value();
  }
  if (debug)
  {
    Info<< "surfaceInterpolation::calcSkewCorrectionVectors() : "
      << "skew coefficient = " << skewCoeff << endl;
  }
  if (skewCoeff < 1e-5)
  {
    skew_ = false;
  }
  else
  {
    skew_ = true;
  }
  if (debug)
  {
    Info<< "surfaceInterpolation::calcSkewCorrectionVectors() : "
      << "Finished constructing skew correction vectors"
      << endl;
  }
}
bool mousse::skewCorrectionVectors::movePoints()
{
  calcSkewCorrectionVectors();
  return true;
}
