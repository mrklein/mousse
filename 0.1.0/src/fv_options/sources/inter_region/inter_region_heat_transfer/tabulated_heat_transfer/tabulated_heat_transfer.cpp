// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tabulated_heat_transfer.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fv
{
  DEFINE_TYPE_NAME_AND_DEBUG(tabulatedHeatTransfer, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    option,
    tabulatedHeatTransfer,
    dictionary
  );
}
}
// Private Member Functions 
const mousse::interpolation2DTable<mousse::scalar>&
mousse::fv::tabulatedHeatTransfer::hTable()
{
  if (!hTable_.valid())
  {
    hTable_.reset(new interpolation2DTable<scalar>(coeffs_));
  }
  return hTable_();
}
const mousse::volScalarField& mousse::fv::tabulatedHeatTransfer::AoV()
{
  if (!AoV_.valid())
  {
    AoV_.reset
    (
      new volScalarField
      (
        IOobject
        (
          "AoV",
          startTimeName_,
          mesh_,
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE
        ),
        mesh_
      )
    );
  }
  return AoV_();
}
// Constructors 
mousse::fv::tabulatedHeatTransfer::tabulatedHeatTransfer
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  interRegionHeatTransferModel(name, modelType, dict, mesh),
  UName_(coeffs_.lookupOrDefault<word>("UName", "U")),
  UNbrName_(coeffs_.lookupOrDefault<word>("UNbrName", "U")),
  hTable_(),
  AoV_(),
  startTimeName_(mesh.time().timeName())
{}
// Destructor 
mousse::fv::tabulatedHeatTransfer::~tabulatedHeatTransfer()
{}
// Member Functions 
void mousse::fv::tabulatedHeatTransfer::calculateHtc()
{
  const fvMesh& nbrMesh = mesh_.time().lookupObject<fvMesh>(nbrRegionName());
  const volVectorField& UNbr =
    nbrMesh.lookupObject<volVectorField>(UNbrName_);
  const scalarField UMagNbr(mag(UNbr));
  const scalarField UMagNbrMapped(interpolate(UMagNbr));
  const volVectorField& U = mesh_.lookupObject<volVectorField>(UName_);
  scalarField& htcc = htc_.internalField();
  FOR_ALL(htcc, i)
  {
    htcc[i] = hTable()(mag(U[i]), UMagNbrMapped[i]);
  }
  htcc = htcc*AoV();
}
bool mousse::fv::tabulatedHeatTransfer::read(const dictionary& dict)
{
  if (interRegionHeatTransferModel::read(dict))
  {
    return true;
  }
  else
  {
    return false;
  }
}
