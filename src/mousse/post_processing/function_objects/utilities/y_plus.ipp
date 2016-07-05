// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "y_plus.hpp"
#include "nut_wall_function_fv_patch_scalar_field.hpp"
#include "near_wall_dist.hpp"
#include "wall_fv_patch.hpp"


// Private Member Functions 
template<class TurbulenceModel>
void mousse::yPlus::calcYPlus
(
  const TurbulenceModel& turbulenceModel,
  const fvMesh& mesh,
  volScalarField& yPlus
)
{
  volScalarField::GeometricBoundaryField d = nearWallDist(mesh).y();
  const volScalarField::GeometricBoundaryField nutBf =
    turbulenceModel.nut()().boundaryField();
  const volScalarField::GeometricBoundaryField nuEffBf =
    turbulenceModel.nuEff()().boundaryField();
  const volScalarField::GeometricBoundaryField nuBf =
    turbulenceModel.nu()().boundaryField();
  const fvPatchList& patches = mesh.boundary();
  FOR_ALL(patches, patchi) {
    const fvPatch& patch = patches[patchi];
    if (isA<nutWallFunctionFvPatchScalarField>(nutBf[patchi])) {
      const nutWallFunctionFvPatchScalarField& nutPf =
        dynamic_cast<const nutWallFunctionFvPatchScalarField&>
        (
          nutBf[patchi]
        );
      yPlus.boundaryField()[patchi] = nutPf.yPlus();
      const scalarField& yPlusp = yPlus.boundaryField()[patchi];
      const scalar minYplus = gMin(yPlusp);
      const scalar maxYplus = gMax(yPlusp);
      const scalar avgYplus = gAverage(yPlusp);
      // TODO: move outside if/else
      if (Pstream::master()) {
        if (log_) {
          Info
            << "    patch " << patch.name()
            << " y+ : min = " << minYplus << ", max = " << maxYplus
            << ", average = " << avgYplus << nl;
        }
        file() << obr_.time().value()
          << token::TAB << patch.name()
          << token::TAB << minYplus
          << token::TAB << maxYplus
          << token::TAB << avgYplus
          << endl;
      }
    } else if (isA<wallFvPatch>(patch)) {
      yPlus.boundaryField()[patchi] =
        d[patchi]
        *sqrt
        (
          nuEffBf[patchi]
          *mag(turbulenceModel.U().boundaryField()[patchi].snGrad())
        )/nuBf[patchi];
      const scalarField& yPlusp = yPlus.boundaryField()[patchi];
      const scalar minYplus = gMin(yPlusp);
      const scalar maxYplus = gMax(yPlusp);
      const scalar avgYplus = gAverage(yPlusp);
      if (Pstream::master()) {
        if (log_) {
          Info
            << "    patch " << patch.name()
            << " y+ : min = " << minYplus << ", max = " << maxYplus
            << ", average = " << avgYplus << nl;
        }
        file() << obr_.time().value()
          << token::TAB << patch.name()
          << token::TAB << minYplus
          << token::TAB << maxYplus
          << token::TAB << avgYplus
          << endl;
      }
    }
  }
}

