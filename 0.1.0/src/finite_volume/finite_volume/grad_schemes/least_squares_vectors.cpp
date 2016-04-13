// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "least_squares_vectors.hpp"
#include "vol_fields.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(leastSquaresVectors, 0);

}


// Constructors
mousse::leastSquaresVectors::leastSquaresVectors(const fvMesh& mesh)
:
  MeshObject<fvMesh, mousse::MoveableMeshObject, leastSquaresVectors>{mesh},
  pVectors_
  {
    {
      "LeastSquaresP",
      mesh_.pointsInstance(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    mesh_,
    {"zero", dimless/dimLength, vector::zero}
  },
  nVectors_
  {
    {
      "LeastSquaresN",
      mesh_.pointsInstance(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    mesh_,
    {"zero", dimless/dimLength, vector::zero}
  }
{
  calcLeastSquaresVectors();
}


// Destructor
mousse::leastSquaresVectors::~leastSquaresVectors()
{}


// Member Functions 
void mousse::leastSquaresVectors::calcLeastSquaresVectors()
{
  if (debug) {
    Info << "leastSquaresVectors::calcLeastSquaresVectors() :"
      << "Calculating least square gradient vectors"
      << endl;
  }
  const fvMesh& mesh = mesh_;
  // Set local references to mesh data
  const labelUList& owner = mesh_.owner();
  const labelUList& neighbour = mesh_.neighbour();
  const volVectorField& C = mesh.C();
  const surfaceScalarField& w = mesh.weights();
  const surfaceScalarField& magSf = mesh.magSf();
  // Set up temporary storage for the dd tensor (before inversion)
  symmTensorField dd{mesh_.nCells(), symmTensor::zero};
  FOR_ALL(owner, facei) {
    label own = owner[facei];
    label nei = neighbour[facei];
    vector d = C[nei] - C[own];
    symmTensor wdd = (magSf[facei]/magSqr(d))*sqr(d);
    dd[own] += (1 - w[facei])*wdd;
    dd[nei] += w[facei]*wdd;
  }
  surfaceVectorField::GeometricBoundaryField& blsP =
    pVectors_.boundaryField();
  FOR_ALL(blsP, patchi) {
    const fvsPatchScalarField& pw = w.boundaryField()[patchi];
    const fvsPatchScalarField& pMagSf = magSf.boundaryField()[patchi];
    const fvPatch& p = pw.patch();
    const labelUList& faceCells = p.patch().faceCells();
    // Build the d-vectors
    vectorField pd{p.delta()};
    if (pw.coupled()) {
      FOR_ALL(pd, patchFacei) {
        const vector& d = pd[patchFacei];
        dd[faceCells[patchFacei]] +=
          ((1 - pw[patchFacei])*pMagSf[patchFacei]/magSqr(d))*sqr(d);
      }
    } else {
      FOR_ALL(pd, patchFacei) {
        const vector& d = pd[patchFacei];
        dd[faceCells[patchFacei]] +=
          (pMagSf[patchFacei]/magSqr(d))*sqr(d);
      }
    }
  }
  // Invert the dd tensor
  const symmTensorField invDd{inv(dd)};
  // Revisit all faces and calculate the pVectors_ and nVectors_ vectors
  FOR_ALL(owner, facei) {
    label own = owner[facei];
    label nei = neighbour[facei];
    vector d = C[nei] - C[own];
    scalar magSfByMagSqrd = magSf[facei]/magSqr(d);
    pVectors_[facei] = (1 - w[facei])*magSfByMagSqrd*(invDd[own] & d);
    nVectors_[facei] = -w[facei]*magSfByMagSqrd*(invDd[nei] & d);
  }
  FOR_ALL(blsP, patchi) {
    fvsPatchVectorField& patchLsP = blsP[patchi];
    const fvsPatchScalarField& pw = w.boundaryField()[patchi];
    const fvsPatchScalarField& pMagSf = magSf.boundaryField()[patchi];
    const fvPatch& p = pw.patch();
    const labelUList& faceCells = p.faceCells();
    // Build the d-vectors
    vectorField pd{p.delta()};
    if (pw.coupled()) {
      FOR_ALL(pd, patchFacei) {
        const vector& d = pd[patchFacei];
        patchLsP[patchFacei] =
          ((1 - pw[patchFacei])*pMagSf[patchFacei]/magSqr(d))
          *(invDd[faceCells[patchFacei]] & d);
      }
    } else {
      FOR_ALL(pd, patchFacei) {
        const vector& d = pd[patchFacei];
        patchLsP[patchFacei] =
          pMagSf[patchFacei]*(1.0/magSqr(d))*(invDd[faceCells[patchFacei]] & d);
      }
    }
  }
  if (debug) {
    Info << "leastSquaresVectors::calcLeastSquaresVectors() :"
      << "Finished calculating least square gradient vectors"
      << endl;
  }
}


bool mousse::leastSquaresVectors::movePoints()
{
  calcLeastSquaresVectors();
  return true;
}

