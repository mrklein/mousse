// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tabulated_acceleration_source.hpp"
#include "fv_mesh.hpp"
#include "fv_matrices.hpp"
#include "geometric_one_field.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fv
{
  defineTypeNameAndDebug(tabulatedAccelerationSource, 0);
  addToRunTimeSelectionTable
  (
    option,
    tabulatedAccelerationSource,
    dictionary
  );
}
}
// Constructors 
mousse::fv::tabulatedAccelerationSource::tabulatedAccelerationSource
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  option(name, modelType, dict, mesh),
  motion_(coeffs_, mesh.time()),
  UName_(coeffs_.lookupOrDefault<word>("UName", "U")),
  g0_("g0", dimAcceleration, vector::zero)
{
  fieldNames_.setSize(1, UName_);
  applied_.setSize(1, false);
  if (mesh.foundObject<uniformDimensionedVectorField>("g"))
  {
    g0_ = mesh.lookupObject<uniformDimensionedVectorField>("g");
  }
}
// Member Functions 
void mousse::fv::tabulatedAccelerationSource::addSup
(
  fvMatrix<vector>& eqn,
  const label fieldi
)
{
  addSup<geometricOneField>(geometricOneField(), eqn, fieldi);
}
void mousse::fv::tabulatedAccelerationSource::addSup
(
  const volScalarField& rho,
  fvMatrix<vector>& eqn,
  const label fieldi
)
{
  addSup<volScalarField>(rho, eqn, fieldi);
}
bool mousse::fv::tabulatedAccelerationSource::read(const dictionary& dict)
{
  if (option::read(dict))
  {
    return motion_.read(coeffs_);
  }
  else
  {
    return false;
  }
}
