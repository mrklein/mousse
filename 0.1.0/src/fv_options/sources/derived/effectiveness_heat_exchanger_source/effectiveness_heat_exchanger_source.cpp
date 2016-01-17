// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "effectiveness_heat_exchanger_source.hpp"
#include "fv_mesh.hpp"
#include "fv_matrix.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "basic_thermo.hpp"
#include "coupled_poly_patch.hpp"
#include "surface_interpolate.hpp"
// Static Data Members
namespace mousse
{
namespace fv
{
  DEFINE_TYPE_NAME_AND_DEBUG(effectivenessHeatExchangerSource, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    option,
    effectivenessHeatExchangerSource,
    dictionary
  );
}
}
// Private Member Functions 
void mousse::fv::effectivenessHeatExchangerSource::initialise()
{
  const faceZone& fZone = mesh_.faceZones()[zoneID_];
  faceId_.setSize(fZone.size());
  facePatchId_.setSize(fZone.size());
  faceSign_.setSize(fZone.size());
  label count = 0;
  FOR_ALL(fZone, i)
  {
    label faceI = fZone[i];
    label faceId = -1;
    label facePatchId = -1;
    if (mesh_.isInternalFace(faceI))
    {
      faceId = faceI;
      facePatchId = -1;
    }
    else
    {
      facePatchId = mesh_.boundaryMesh().whichPatch(faceI);
      const polyPatch& pp = mesh_.boundaryMesh()[facePatchId];
      if (isA<coupledPolyPatch>(pp))
      {
        if (refCast<const coupledPolyPatch>(pp).owner())
        {
          faceId = pp.whichFace(faceI);
        }
        else
        {
          faceId = -1;
        }
      }
      else if (!isA<emptyPolyPatch>(pp))
      {
        faceId = faceI - pp.start();
      }
      else
      {
        faceId = -1;
        facePatchId = -1;
      }
    }
    if (faceId >= 0)
    {
      if (fZone.flipMap()[i])
      {
        faceSign_[count] = -1;
      }
      else
      {
        faceSign_[count] = 1;
      }
      faceId_[count] = faceId;
      facePatchId_[count] = facePatchId;
      count++;
    }
  }
  faceId_.setSize(count);
  facePatchId_.setSize(count);
  faceSign_.setSize(count);
  calculateTotalArea(faceZoneArea_);
}
void mousse::fv::effectivenessHeatExchangerSource::calculateTotalArea
(
  scalar& area
)
{
  area = 0;
  FOR_ALL(faceId_, i)
  {
    label faceI = faceId_[i];
    if (facePatchId_[i] != -1)
    {
      label patchI = facePatchId_[i];
      area += mesh_.magSf().boundaryField()[patchI][faceI];
    }
    else
    {
      area += mesh_.magSf()[faceI];
    }
  }
  reduce(area, sumOp<scalar>());
}
// Constructors 
mousse::fv::effectivenessHeatExchangerSource::effectivenessHeatExchangerSource
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  cellSetOption(name, modelType, dict, mesh),
  secondaryMassFlowRate_(readScalar(coeffs_.lookup("secondaryMassFlowRate"))),
  secondaryInletT_(readScalar(coeffs_.lookup("secondaryInletT"))),
  primaryInletT_(readScalar(coeffs_.lookup("primaryInletT"))),
  eTable_(),
  UName_(coeffs_.lookupOrDefault<word>("UName", "U")),
  TName_(coeffs_.lookupOrDefault<word>("TName", "T")),
  phiName_(coeffs_.lookupOrDefault<word>("phiName", "phi")),
  faceZoneName_(coeffs_.lookup("faceZone")),
  zoneID_(mesh_.faceZones().findZoneID(faceZoneName_)),
  faceId_(),
  facePatchId_(),
  faceSign_(),
  faceZoneArea_(0)
{
  if (zoneID_ < 0)
  {
    FATAL_ERROR_IN
    (
      "effectivenessHeatExchangerSource::effectivenessHeatExchangerSource"
      "("
        "const word&, "
        "const word&, "
        "const dictionary&, "
        "const fvMesh&"
      ")"
    )
      << type() << " " << this->name() << ": "
      << "    Unknown face zone name: " << faceZoneName_
      << ". Valid face zones are: " << mesh_.faceZones().names()
      << nl << exit(FatalError);
  }
  // Set the field name to that of the energy field from which the temperature
  // is obtained
  const basicThermo& thermo =
    mesh_.lookupObject<basicThermo>(basicThermo::dictName);
  fieldNames_.setSize(1, thermo.he().name());
  applied_.setSize(1, false);
  eTable_.reset(new interpolation2DTable<scalar>(coeffs_));
  initialise();
}
// Member Functions 
void mousse::fv::effectivenessHeatExchangerSource::addSup
(
  const volScalarField& /*rho*/,
  fvMatrix<scalar>& eqn,
  const label
)
{
  const basicThermo& thermo =
    mesh_.lookupObject<basicThermo>(basicThermo::dictName);
  const surfaceScalarField Cpf(fvc::interpolate(thermo.Cp()));
  const surfaceScalarField& phi =
    mesh_.lookupObject<surfaceScalarField>(phiName_);
  scalar totalphi = 0;
  scalar CpfMean = 0;
  FOR_ALL(faceId_, i)
  {
    label faceI = faceId_[i];
    if (facePatchId_[i] != -1)
    {
      label patchI = facePatchId_[i];
      totalphi += phi.boundaryField()[patchI][faceI]*faceSign_[i];
      CpfMean +=
        Cpf.boundaryField()[patchI][faceI]
       *mesh_.magSf().boundaryField()[patchI][faceI];
    }
    else
    {
      totalphi += phi[faceI]*faceSign_[i];
      CpfMean += Cpf[faceI]*mesh_.magSf()[faceI];
    }
  }
  reduce(CpfMean, sumOp<scalar>());
  reduce(totalphi, sumOp<scalar>());
  scalar Qt =
    eTable_()(mag(totalphi), secondaryMassFlowRate_)
   *(secondaryInletT_ - primaryInletT_)
   *(CpfMean/faceZoneArea_)*mag(totalphi);
  const volScalarField& T = mesh_.lookupObject<volScalarField>(TName_);
  const scalarField TCells(T, cells_);
  scalar Tref = 0;
  if (Qt > 0)
  {
    Tref = max(TCells);
    reduce(Tref, maxOp<scalar>());
  }
  else
  {
    Tref = min(TCells);
    reduce(Tref, minOp<scalar>());
  }
  scalarField deltaTCells(cells_.size(), 0);
  FOR_ALL(deltaTCells, i)
  {
    if (Qt > 0)
    {
      deltaTCells[i] = max(Tref - TCells[i], 0.0);
    }
    else
    {
      deltaTCells[i] = max(TCells[i] - Tref, 0.0);
    }
  }
  const volVectorField& U = mesh_.lookupObject<volVectorField>(UName_);
  const scalarField& V = mesh_.V();
  scalar sumWeight = 0;
  FOR_ALL(cells_, i)
  {
    sumWeight += V[cells_[i]]*mag(U[cells_[i]])*deltaTCells[i];
  }
  reduce(sumWeight, sumOp<scalar>());
  if (this->V() > VSMALL && mag(Qt) > VSMALL)
  {
    scalarField& heSource = eqn.source();
    FOR_ALL(cells_, i)
    {
      heSource[cells_[i]] -=
        Qt*V[cells_[i]]*mag(U[cells_[i]])*deltaTCells[i]/sumWeight;
    }
  }
  if (debug && Pstream::master())
  {
    Info<< indent << "Net mass flux [Kg/s] = " << totalphi << nl;
    Info<< indent << "Total energy exchange [W] = " << Qt << nl;
    Info<< indent << "Tref [K] = " << Tref << nl;
    Info<< indent << "Efficiency : "
      << eTable_()(mag(totalphi), secondaryMassFlowRate_) << endl;
  }
}
bool mousse::fv::effectivenessHeatExchangerSource::read(const dictionary& dict)
{
  if (cellSetOption::read(dict))
  {
    coeffs_.lookup("secondaryMassFlowRate") >> secondaryMassFlowRate_;
    coeffs_.lookup("secondaryInletT") >> secondaryInletT_;
    coeffs_.lookup("primaryInletT") >> primaryInletT_;
    return true;
  }
  else
  {
    return false;
  }
}
