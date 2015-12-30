// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "centred_fit_data.hpp"
#include "surface_fields.hpp"
#include "vol_fields.hpp"
#include "svd.hpp"
#include "sync_tools.hpp"
#include "extended_centred_cell_to_face_stencil.hpp"
// Constructors
template<class Polynomial>
mousse::CentredFitData<Polynomial>::CentredFitData
(
  const fvMesh& mesh,
  const extendedCentredCellToFaceStencil& stencil,
  const scalar linearLimitFactor,
  const scalar centralWeight
)
:
  FitData
  <
    CentredFitData<Polynomial>,
    extendedCentredCellToFaceStencil,
    Polynomial
  >
  (
    mesh, stencil, true, linearLimitFactor, centralWeight
  ),
  coeffs_(mesh.nFaces())
{
  if (debug)
  {
    Info<< "Contructing CentredFitData<Polynomial>" << endl;
  }
  calcFit();
  if (debug)
  {
    Info<< "CentredFitData<Polynomial>::CentredFitData() :"
      << "Finished constructing polynomialFit data"
      << endl;
  }
}
// Member Functions 
template<class Polynomial>
void mousse::CentredFitData<Polynomial>::calcFit()
{
  const fvMesh& mesh = this->mesh();
  // Get the cell/face centres in stencil order.
  // Centred face stencils no good for triangles or tets.
  // Need bigger stencils
  List<List<point> > stencilPoints(mesh.nFaces());
  this->stencil().collectData(mesh.C(), stencilPoints);
  // find the fit coefficients for every face in the mesh
  const surfaceScalarField& w = mesh.surfaceInterpolation::weights();
  for (label facei = 0; facei < mesh.nInternalFaces(); facei++)
  {
    FitData
    <
      CentredFitData<Polynomial>,
      extendedCentredCellToFaceStencil,
      Polynomial
    >::calcFit(coeffs_[facei], stencilPoints[facei], w[facei], facei);
  }
  const surfaceScalarField::GeometricBoundaryField& bw = w.boundaryField();
  forAll(bw, patchi)
  {
    const fvsPatchScalarField& pw = bw[patchi];
    if (pw.coupled())
    {
      label facei = pw.patch().start();
      forAll(pw, i)
      {
        FitData
        <
          CentredFitData<Polynomial>,
          extendedCentredCellToFaceStencil,
          Polynomial
        >::calcFit(coeffs_[facei], stencilPoints[facei], pw[i], facei);
        facei++;
      }
    }
  }
}
