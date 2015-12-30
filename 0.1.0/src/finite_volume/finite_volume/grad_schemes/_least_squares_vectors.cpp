// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_least_squares_vectors.hpp"
// Constructors
template<class Stencil>
mousse::fv::LeastSquaresVectors<Stencil>::LeastSquaresVectors
(
  const fvMesh& mesh
)
:
  MeshObject<fvMesh, mousse::MoveableMeshObject, LeastSquaresVectors>(mesh),
  vectors_(mesh.nCells())
{
  calcLeastSquaresVectors();
}
// Destructor
template<class Stencil>
mousse::fv::LeastSquaresVectors<Stencil>::~LeastSquaresVectors()
{}
// Member Functions 
template<class Stencil>
void mousse::fv::LeastSquaresVectors<Stencil>::calcLeastSquaresVectors()
{
  if (debug)
  {
    Info<< "LeastSquaresVectors::calcLeastSquaresVectors() :"
      << "Calculating least square gradient vectors"
      << endl;
  }
  const fvMesh& mesh = this->mesh_;
  const extendedCentredCellToCellStencil& stencil = this->stencil();
  stencil.collectData(mesh.C(), vectors_);
  // Create the base form of the dd-tensor
  // including components for the "empty" directions
  symmTensor dd0(sqr((Vector<label>::one - mesh.geometricD())/2));
  forAll (vectors_, i)
  {
    List<vector>& lsvi = vectors_[i];
    symmTensor dd(dd0);
    // The current cell is 0 in the stencil
    // Calculate the deltas and sum the weighted dd
    for (label j=1; j<lsvi.size(); j++)
    {
      lsvi[j] = lsvi[j] - lsvi[0];
      scalar magSqrLsvi = magSqr(lsvi[j]);
      dd += sqr(lsvi[j])/magSqrLsvi;
      lsvi[j] /= magSqrLsvi;
    }
    // Invert dd
    dd = inv(dd);
    // Remove the components corresponding to the empty directions
    dd -= dd0;
    // Finalize the gradient weighting vectors
    lsvi[0] = vector::zero;
    for (label j=1; j<lsvi.size(); j++)
    {
      lsvi[j] = dd & lsvi[j];
      lsvi[0] -= lsvi[j];
    }
  }
  if (debug)
  {
    Info<< "LeastSquaresVectors::calcLeastSquaresVectors() :"
      << "Finished calculating least square gradient vectors"
      << endl;
  }
}
template<class Stencil>
bool mousse::fv::LeastSquaresVectors<Stencil>::movePoints()
{
  calcLeastSquaresVectors();
  return true;
}
