// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "upwind_fit_data.hpp"
#include "surface_fields.hpp"
#include "vol_fields.hpp"
#include "svd.hpp"
#include "extended_upwind_cell_to_face_stencil.hpp"


// Constructors
template<class Polynomial>
mousse::UpwindFitData<Polynomial>::UpwindFitData
(
  const fvMesh& mesh,
  const extendedUpwindCellToFaceStencil& stencil,
  const bool linearCorrection,
  const scalar linearLimitFactor,
  const scalar centralWeight
)
:
  FitData
  <
    UpwindFitData<Polynomial>,
    extendedUpwindCellToFaceStencil,
    Polynomial
  >
  {
    mesh, stencil, linearCorrection, linearLimitFactor, centralWeight
  },
  owncoeffs_{mesh.nFaces()},
  neicoeffs_{mesh.nFaces()}
{
  if (debug) {
    Info << "Contructing UpwindFitData<Polynomial>" << endl;
  }
  calcFit();
  if (debug) {
    Info << "UpwindFitData<Polynomial>::UpwindFitData() :"
      << "Finished constructing polynomialFit data"
      << endl;
  }
}


// Member Functions 
template<class Polynomial>
void mousse::UpwindFitData<Polynomial>::calcFit()
{
  const fvMesh& mesh = this->mesh();
  const surfaceScalarField& w = mesh.surfaceInterpolation::weights();
  const surfaceScalarField::GeometricBoundaryField& bw = w.boundaryField();
  // Owner stencil weights
  // ~~~~~~~~~~~~~~~~~~~~~
  // Get the cell/face centres in stencil order.
  List<List<point>> stencilPoints{mesh.nFaces()};
  this->stencil().collectData
  (
    this->stencil().ownMap(),
    this->stencil().ownStencil(),
    mesh.C(),
    stencilPoints
  );
  // find the fit coefficients for every owner
  for (label facei = 0; facei < mesh.nInternalFaces(); facei++) {
    FitData
    <
      UpwindFitData<Polynomial>,
      extendedUpwindCellToFaceStencil,
      Polynomial
    >::calcFit(owncoeffs_[facei], stencilPoints[facei], w[facei], facei);
  }
  FOR_ALL(bw, patchi) {
    const fvsPatchScalarField& pw = bw[patchi];
    if (pw.coupled()) {
      label facei = pw.patch().start();
      FOR_ALL(pw, i) {
        FitData
        <
          UpwindFitData<Polynomial>,
          extendedUpwindCellToFaceStencil,
          Polynomial
        >::calcFit
        (
          owncoeffs_[facei], stencilPoints[facei], pw[i], facei
        );
        facei++;
      }
    }
  }
  // Neighbour stencil weights
  // ~~~~~~~~~~~~~~~~~~~~~~~~~
  // Note:reuse stencilPoints since is major storage
  this->stencil().collectData
  (
    this->stencil().neiMap(),
    this->stencil().neiStencil(),
    mesh.C(),
    stencilPoints
  );
  // find the fit coefficients for every neighbour
  for (label facei = 0; facei < mesh.nInternalFaces(); facei++) {
    FitData
    <
      UpwindFitData<Polynomial>,
      extendedUpwindCellToFaceStencil,
      Polynomial
    >::calcFit(neicoeffs_[facei], stencilPoints[facei], w[facei], facei);
  }
  FOR_ALL(bw, patchi) {
    const fvsPatchScalarField& pw = bw[patchi];
    if (pw.coupled()) {
      label facei = pw.patch().start();
      FOR_ALL(pw, i) {
        FitData
        <
          UpwindFitData<Polynomial>,
          extendedUpwindCellToFaceStencil,
          Polynomial
        >::calcFit
        (
          neicoeffs_[facei], stencilPoints[facei], pw[i], facei
        );
        facei++;
      }
    }
  }
}

