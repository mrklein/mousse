// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "radial_actuation_disk_source.hpp"
#include "geometric_one_field.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fv {

DEFINE_TYPE_NAME_AND_DEBUG(radialActuationDiskSource, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  option,
  radialActuationDiskSource,
  dictionary
);

}
}


// Constructors 
mousse::fv::radialActuationDiskSource::radialActuationDiskSource
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  actuationDiskSource{name, modelType, dict, mesh},
  radialCoeffs_{coeffs_.lookup("coeffs")}
{
  Info << "    - creating radial actuation disk zone: " << name_ << endl;
}


// Member Functions 
void mousse::fv::radialActuationDiskSource::addSup
(
  fvMatrix<vector>& eqn,
  const label /*fieldI*/
)
{
  const scalarField& cellsV = mesh_.V();
  vectorField& Usource = eqn.source();
  const vectorField& U = eqn.psi();
  if (V_ > VSMALL) {
    addRadialActuationDiskAxialInertialResistance
      (
        Usource,
        cells_,
        cellsV,
        geometricOneField(),
        U
      );
  }
}


void mousse::fv::radialActuationDiskSource::addSup
(
  const volScalarField& rho,
  fvMatrix<vector>& eqn,
  const label /*fieldI*/
)
{
  const scalarField& cellsV = mesh_.V();
  vectorField& Usource = eqn.source();
  const vectorField& U = eqn.psi();
  if (V_ > VSMALL) {
    addRadialActuationDiskAxialInertialResistance
      (
        Usource,
        cells_,
        cellsV,
        rho,
        U
      );
  }
}


bool mousse::fv::radialActuationDiskSource::read(const dictionary& dict)
{
  if (actuationDiskSource::read(dict)) {
    coeffs_.lookup("coeffs") >> radialCoeffs_;
    return true;
  }
  return false;
}

