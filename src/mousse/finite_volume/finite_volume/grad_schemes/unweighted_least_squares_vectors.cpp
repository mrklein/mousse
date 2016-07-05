// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
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
  // Set up temporary storage for the dd tensor (before inversion)
  symmTensorField dd{mesh_.nCells(), symmTensor::zero};
  FOR_ALL(owner, facei) {
    label own = owner[facei];
    label nei = neighbour[facei];
    symmTensor wdd = sqr(C[nei] - C[own]);
    dd[own] += wdd;
    dd[nei] += wdd;
  }
  surfaceVectorField::GeometricBoundaryField& blsP =
    pVectors_.boundaryField();
  FOR_ALL(blsP, patchi) {
    const fvsPatchVectorField& patchLsP = blsP[patchi];
    const fvPatch& p = patchLsP.patch();
    const labelUList& faceCells = p.patch().faceCells();
    // Build the d-vectors
    vectorField pd{p.delta()};
    FOR_ALL(pd, patchFacei) {
      dd[faceCells[patchFacei]] += sqr(pd[patchFacei]);
    }
  }
  // Invert the dd tensor
  const symmTensorField invDd{inv(dd)};
  // Revisit all faces and calculate the pVectors_ and nVectors_ vectors
  FOR_ALL(owner, facei) {
    label own = owner[facei];
    label nei = neighbour[facei];
    vector d = C[nei] - C[own];
    pVectors_[facei] = (invDd[own] & d);
    nVectors_[facei] = -(invDd[nei] & d);
  }
  FOR_ALL(blsP, patchi) {
    fvsPatchVectorField& patchLsP = blsP[patchi];
    const fvPatch& p = patchLsP.patch();
    const labelUList& faceCells = p.faceCells();
    // Build the d-vectors
    vectorField pd{p.delta()};
    FOR_ALL(pd, patchFacei) {
      patchLsP[patchFacei] =
        (invDd[faceCells[patchFacei]] & pd[patchFacei]);
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

