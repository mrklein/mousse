// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_temperature_constraint.hpp"
#include "fv_mesh.hpp"
#include "fv_matrices.hpp"
#include "basic_thermo.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace fv
  {
    DEFINE_TYPE_NAME_AND_DEBUG(fixedTemperatureConstraint, 0);
    ADD_TO_RUN_TIME_SELECTION_TABLE
    (
      option,
      fixedTemperatureConstraint,
      dictionary
    );
  }
  template<>
  const char* NamedEnum<fv::fixedTemperatureConstraint::temperatureMode, 2>::
  names[] =
  {
    "uniform",
    "lookup"
  };
}
const mousse::NamedEnum<mousse::fv::fixedTemperatureConstraint::temperatureMode, 2>
  mousse::fv::fixedTemperatureConstraint::temperatureModeNames_;
// Constructors 
mousse::fv::fixedTemperatureConstraint::fixedTemperatureConstraint
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  cellSetOption(name, modelType, dict, mesh),
  mode_(temperatureModeNames_.read(coeffs_.lookup("mode"))),
  Tuniform_(NULL),
  TName_("T")
{
  switch (mode_)
  {
    case tmUniform:
    {
      Tuniform_.reset
      (
        DataEntry<scalar>::New("temperature", coeffs_).ptr()
      );
      break;
    }
    case tmLookup:
    {
      TName_ = coeffs_.lookupOrDefault<word>("TName", "T");
      break;
    }
    default:
    {
      // error handling done by NamedEnum
    }
  }
  // Set the field name to that of the energy field from which the temperature
  // is obtained
  const basicThermo& thermo =
    mesh_.lookupObject<basicThermo>(basicThermo::dictName);
  fieldNames_.setSize(1, thermo.he().name());
  applied_.setSize(1, false);
}
// Member Functions 
void mousse::fv::fixedTemperatureConstraint::constrain
(
  fvMatrix<scalar>& eqn,
  const label
)
{
  const basicThermo& thermo =
    mesh_.lookupObject<basicThermo>(basicThermo::dictName);
  switch (mode_)
  {
    case tmUniform:
    {
      const scalar t = mesh_.time().value();
      scalarField Tuni(cells_.size(), Tuniform_->value(t));
      eqn.setValues(cells_, thermo.he(thermo.p(), Tuni, cells_));
      break;
    }
    case tmLookup:
    {
      const volScalarField& T =
        mesh().lookupObject<volScalarField>(TName_);
      scalarField Tlkp(T, cells_);
      eqn.setValues(cells_, thermo.he(thermo.p(), Tlkp, cells_));
      break;
    }
    default:
    {
      // error handling done by NamedEnum
    }
  }
}
bool mousse::fv::fixedTemperatureConstraint::read(const dictionary& dict)
{
  if (cellSetOption::read(dict))
  {
    if (coeffs_.found(Tuniform_->name()))
    {
      Tuniform_.reset
      (
        DataEntry<scalar>::New(Tuniform_->name(), dict).ptr()
      );
    }
    coeffs_.readIfPresent("TName", TName_);
    return true;
  }
  else
  {
    return false;
  }
}
