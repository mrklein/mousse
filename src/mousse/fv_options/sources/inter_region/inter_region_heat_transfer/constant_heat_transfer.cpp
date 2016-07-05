// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_heat_transfer.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fv {

DEFINE_TYPE_NAME_AND_DEBUG(constantHeatTransfer, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  option,
  constantHeatTransfer,
  dictionary
);

}
}


// Constructors 
mousse::fv::constantHeatTransfer::constantHeatTransfer
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  interRegionHeatTransferModel{name, modelType, dict, mesh},
  htcConst_{},
  AoV_{}
{
  if (active() && master_) {
    htcConst_.reset
    (
      new volScalarField
      {
        IOobject
        {
          "htcConst",
          mesh_.time().timeName(),
          mesh_,
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE
        },
        mesh_
      }
    );
    AoV_.reset
    (
      new volScalarField
      {
        IOobject
        {
          "AoV",
          mesh_.time().timeName(),
          mesh_,
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE
        },
        mesh_
      }
    );
    htc_ = htcConst_()*AoV_();
  }
}


// Destructor 
mousse::fv::constantHeatTransfer::~constantHeatTransfer()
{}


// Member Functions 
void mousse::fv::constantHeatTransfer::calculateHtc()
{}


bool mousse::fv::constantHeatTransfer::read(const dictionary& dict)
{
  if (interRegionHeatTransferModel::read(dict)) {
    return true;
  }
  return false;
}

