// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "explicit_porosity_source.hpp"
#include "fv_mesh.hpp"
#include "fv_matrices.hpp"
#include "porosity_model.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fv
{
  defineTypeNameAndDebug(explicitPorositySource, 0);
  addToRunTimeSelectionTable
  (
    option,
    explicitPorositySource,
    dictionary
  );
}
}
// Constructors 
mousse::fv::explicitPorositySource::explicitPorositySource
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  cellSetOption(name, modelType, dict, mesh),
  porosityPtr_(NULL)
{
  read(dict);
  if (selectionMode_ != smCellZone)
  {
    FatalErrorIn("void mousse::fv::explicitPorositySource::initialise()")
      << "The porosity region must be specified as a cellZone.  Current "
      << "selection mode is " << selectionModeTypeNames_[selectionMode_]
      << exit(FatalError);
  }
  porosityPtr_.reset
  (
    porosityModel::New
    (
      name_,
      mesh_,
      coeffs_,
      cellSetName_
    ).ptr()
  );
}
// Member Functions 
void mousse::fv::explicitPorositySource::addSup
(
  fvMatrix<vector>& eqn,
  const label fieldI
)
{
  fvMatrix<vector> porosityEqn(eqn.psi(), eqn.dimensions());
  porosityPtr_->addResistance(porosityEqn);
  eqn -= porosityEqn;
}
void mousse::fv::explicitPorositySource::addSup
(
  const volScalarField& rho,
  fvMatrix<vector>& eqn,
  const label fieldI
)
{
  fvMatrix<vector> porosityEqn(eqn.psi(), eqn.dimensions());
  porosityPtr_->addResistance(porosityEqn);
  eqn -= porosityEqn;
}
void mousse::fv::explicitPorositySource::addSup
(
  const volScalarField& alpha,
  const volScalarField& rho,
  fvMatrix<vector>& eqn,
  const label fieldI
)
{
  fvMatrix<vector> porosityEqn(eqn.psi(), eqn.dimensions());
  porosityPtr_->addResistance(porosityEqn);
  eqn -= alpha*porosityEqn;
}
bool mousse::fv::explicitPorositySource::read(const dictionary& dict)
{
  if (cellSetOption::read(dict))
  {
    if (coeffs_.found("UNames"))
    {
      coeffs_.lookup("UNames") >> fieldNames_;
    }
    else if (coeffs_.found("UName"))
    {
      word UName(coeffs_.lookup("UName"));
      fieldNames_ = wordList(1, UName);
    }
    else
    {
      fieldNames_ = wordList(1, "U");
    }
    applied_.setSize(fieldNames_.size(), false);
    return true;
  }
  else
  {
    return false;
  }
}
