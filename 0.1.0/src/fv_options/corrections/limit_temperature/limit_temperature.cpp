// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limit_temperature.hpp"
#include "fv_mesh.hpp"
#include "basic_thermo.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fv
{
  DEFINE_TYPE_NAME_AND_DEBUG(limitTemperature, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    option,
    limitTemperature,
    dictionary
  );
}
}
// Constructors 
mousse::fv::limitTemperature::limitTemperature
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  cellSetOption(name, modelType, dict, mesh),
  Tmin_(readScalar(coeffs_.lookup("Tmin"))),
  Tmax_(readScalar(coeffs_.lookup("Tmax")))
{
  // Set the field name to that of the energy field from which the temperature
  // is obtained
  const basicThermo& thermo =
    mesh_.lookupObject<basicThermo>(basicThermo::dictName);
  fieldNames_.setSize(1, thermo.he().name());
  applied_.setSize(1, false);
}
// Member Functions 
void mousse::fv::limitTemperature::correct(volScalarField& he)
{
  const basicThermo& thermo =
    mesh_.lookupObject<basicThermo>(basicThermo::dictName);
  scalarField Tmin(cells_.size(), Tmin_);
  scalarField Tmax(cells_.size(), Tmax_);
  scalarField heMin(thermo.he(thermo.p(), Tmin, cells_));
  scalarField heMax(thermo.he(thermo.p(), Tmax, cells_));
  scalarField& hec = he.internalField();
  FOR_ALL(cells_, i)
  {
    label cellI = cells_[i];
    hec[cellI]= max(min(hec[cellI], heMax[i]), heMin[i]);
  }
  // handle boundaries in the case of 'all'
  if (selectionMode_ == smAll)
  {
    volScalarField::GeometricBoundaryField& bf = he.boundaryField();
    FOR_ALL(bf, patchi)
    {
      fvPatchScalarField& hep = bf[patchi];
      if (!hep.fixesValue())
      {
        const scalarField& pp = thermo.p().boundaryField()[patchi];
        scalarField Tminp(pp.size(), Tmin_);
        scalarField Tmaxp(pp.size(), Tmax_);
        scalarField heMinp(thermo.he(pp, Tminp, patchi));
        scalarField heMaxp(thermo.he(pp, Tmaxp, patchi));
        FOR_ALL(hep, facei)
        {
          hep[facei] =
            max(min(hep[facei], heMaxp[facei]), heMinp[facei]);
        }
      }
    }
  }
}
bool mousse::fv::limitTemperature::read(const dictionary& dict)
{
  if (cellSetOption::read(dict))
  {
    coeffs_.readIfPresent("Tmin", Tmin_);
    coeffs_.readIfPresent("Tmax", Tmax_);
    return true;
  }
  else
  {
    return false;
  }
}
