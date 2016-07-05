// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "porosity_model.hpp"
#include "vol_fields.hpp"
#include "time.hpp"
#include "pstream_reduce_ops.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(porosityModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(porosityModel, mesh);

}


// Protected Member Functions 
void mousse::porosityModel::adjustNegativeResistance(dimensionedVector& resist)
{
  scalar maxCmpt = max(0, cmptMax(resist.value()));
  if (maxCmpt < 0) {
    FATAL_ERROR_IN_FUNCTION
      << "Negative resistances are invalid, resistance = " << resist
      << exit(FatalError);
  } else {
    vector& val = resist.value();
    for (label cmpt = 0; cmpt < vector::nComponents; cmpt++) {
      if (val[cmpt] < 0) {
        val[cmpt] *= -maxCmpt;
      }
    }
  }
}


mousse::label mousse::porosityModel::fieldIndex(const label i) const
{
  label index = 0;
  if (!coordSys_.R().uniform()) {
    index = i;
  }
  return index;
}


// Constructors 
mousse::porosityModel::porosityModel
(
  const word& name,
  const word& modelType,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& cellZoneName
)
:
  regIOobject
  {
    {
      name,
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    }
  },
  name_{name},
  mesh_{mesh},
  dict_{dict},
  coeffs_{dict.subDict(modelType + "Coeffs")},
  active_{true},
  zoneName_{cellZoneName},
  cellZoneIDs_{},
  coordSys_{coordinateSystem::New(mesh, coeffs_)}
{
  if (zoneName_ == word::null) {
    dict.lookup("active") >> active_;
    dict_.lookup("cellZone") >> zoneName_;
  }
  cellZoneIDs_ = mesh_.cellZones().findIndices(zoneName_);
  Info<< "    creating porous zone: " << zoneName_ << endl;
  bool foundZone = !cellZoneIDs_.empty();
  reduce(foundZone, orOp<bool>());
  if (!foundZone && Pstream::master()) {
    FATAL_ERROR_IN_FUNCTION
      << "cannot find porous cellZone " << zoneName_
      << exit(FatalError);
  }
}


// Destructor 
mousse::porosityModel::~porosityModel()
{}


// Member Functions 
void mousse::porosityModel::transformModelData()
{
  if (!mesh_.upToDatePoints(*this)) {
    calcTranformModelData();
    // set model up-to-date wrt points
    mesh_.setUpToDatePoints(*this);
  }
}


mousse::tmp<mousse::vectorField> mousse::porosityModel::porosityModel::force
(
  const volVectorField& U,
  const volScalarField& rho,
  const volScalarField& mu
)
{
  transformModelData();
  tmp<vectorField> tforce{new vectorField{U.size(), vector::zero}};
  if (!cellZoneIDs_.empty()) {
    this->calcForce(U, rho, mu, tforce());
  }
  return tforce;
}


void mousse::porosityModel::addResistance(fvVectorMatrix& UEqn)
{
  if (cellZoneIDs_.empty()) {
    return;
  }
  transformModelData();
  this->correct(UEqn);
}


void mousse::porosityModel::addResistance
(
  fvVectorMatrix& UEqn,
  const volScalarField& rho,
  const volScalarField& mu
)
{
  if (cellZoneIDs_.empty()) {
    return;
  }
  transformModelData();
  this->correct(UEqn, rho, mu);
}


void mousse::porosityModel::addResistance
(
  const fvVectorMatrix& UEqn,
  volTensorField& AU,
  bool correctAUprocBC
)
{
  if (cellZoneIDs_.empty()) {
    return;
  }
  transformModelData();
  this->correct(UEqn, AU);
  if (correctAUprocBC) {
    // Correct the boundary conditions of the tensorial diagonal to ensure
    // processor boundaries are correctly handled when AU^-1 is interpolated
    // for the pressure equation.
    AU.correctBoundaryConditions();
  }
}


bool mousse::porosityModel::writeData(Ostream&) const
{
  return true;
}


bool mousse::porosityModel::read(const dictionary& dict)
{
  active_ = readBool(dict.lookup("active"));
  coeffs_ = dict.subDict(type() + "Coeffs");
  dict.lookup("cellZone") >> zoneName_;
  cellZoneIDs_ = mesh_.cellZones().findIndices(zoneName_);
  return true;
}

