// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inter_region_explicit_porosity_source.hpp"
#include "fv_mesh.hpp"
#include "fv_matrices.hpp"
#include "porosity_model.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace fv {

DEFINE_TYPE_NAME_AND_DEBUG(interRegionExplicitPorositySource, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  option,
  interRegionExplicitPorositySource,
  dictionary
);

}
}


// Protected Member Functions 
void mousse::fv::interRegionExplicitPorositySource::initialise()
{
  if (!firstIter_) {
    return;
  }
  const word zoneName(name_ + ":porous");
  const fvMesh& nbrMesh = mesh_.time().lookupObject<fvMesh>(nbrRegionName_);
  const cellZoneMesh& cellZones = nbrMesh.cellZones();
  label zoneID = cellZones.findZoneID(zoneName);
  if (zoneID == -1) {
    cellZoneMesh& cz = const_cast<cellZoneMesh&>(cellZones);
    zoneID = cz.size();
    cz.setSize(zoneID + 1);
    cz.set
    (
      zoneID,
      new cellZone
      {
        zoneName,
        nbrMesh.faceNeighbour(), // neighbour internal cells
        zoneID,
        cellZones
      }
    );
    cz.clearAddressing();
  } else {
    FATAL_ERROR_IN
    (
      "void mousse::fv::interRegionExplicitPorositySource::initialise()"
    )
    << "Unable to create porous cellZone " << zoneName
    << ": zone already exists"
    << abort(FatalError);
  }
  porosityPtr_.reset
  (
    porosityModel::New
    (
      name_,
      nbrMesh,
      coeffs_,
      zoneName
    ).ptr()
  ),
  firstIter_ = false;
}


// Constructors 
mousse::fv::interRegionExplicitPorositySource::interRegionExplicitPorositySource
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  interRegionOption{name, modelType, dict, mesh},
  porosityPtr_{nullptr},
  firstIter_{false},
  UName_{coeffs_.lookupOrDefault<word>("UName", "U")},
  muName_{coeffs_.lookupOrDefault<word>("muName", "thermo:mu")}
{
  if (active_) {
    fieldNames_.setSize(1, UName_);
    applied_.setSize(1, false);
  }
}


// Member Functions 
void mousse::fv::interRegionExplicitPorositySource::addSup
(
  fvMatrix<vector>& eqn,
  const label /*fieldI*/
)
{
  initialise();
  const fvMesh& nbrMesh = mesh_.time().lookupObject<fvMesh>(nbrRegionName_);
  const volVectorField& U = eqn.psi();
  volVectorField UNbr
  {
    // IOobject
    {
      name_ + ":UNbr",
      nbrMesh.time().timeName(),
      nbrMesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    nbrMesh,
    // dimensionedVector("zero", U.dimensions(), vector::zero)
    {"zero", U.dimensions(), vector::zero}
  };
  // map local velocity onto neighbour region
  meshInterp().mapSrcToTgt
    (
      U.internalField(),
      plusEqOp<vector>(),
      UNbr.internalField()
    );
  fvMatrix<vector> nbrEqn{UNbr, eqn.dimensions()};
  porosityPtr_->addResistance(nbrEqn);
  // convert source from neighbour to local region
  fvMatrix<vector> porosityEqn{U, eqn.dimensions()};
  scalarField& Udiag = porosityEqn.diag();
  vectorField& Usource = porosityEqn.source();
  Udiag.setSize(eqn.diag().size(), 0.0);
  Usource.setSize(eqn.source().size(), vector::zero);
  meshInterp().mapTgtToSrc(nbrEqn.diag(), plusEqOp<scalar>(), Udiag);
  meshInterp().mapTgtToSrc(nbrEqn.source(), plusEqOp<vector>(), Usource);
  eqn -= porosityEqn;
}


void mousse::fv::interRegionExplicitPorositySource::addSup
(
  const volScalarField& rho,
  fvMatrix<vector>& eqn,
  const label /*fieldI*/
)
{
  initialise();
  const fvMesh& nbrMesh = mesh_.time().lookupObject<fvMesh>(nbrRegionName_);
  const volVectorField& U = eqn.psi();
  volVectorField UNbr
  {
    // IOobject
    {
      name_ + ":UNbr",
      nbrMesh.time().timeName(),
      nbrMesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    nbrMesh,
    // dimensionedVector("zero", U.dimensions(), vector::zero)
    {"zero", U.dimensions(), vector::zero}
  };
  // map local velocity onto neighbour region
  meshInterp().mapSrcToTgt
    (
      U.internalField(),
      plusEqOp<vector>(),
      UNbr.internalField()
    );
  fvMatrix<vector> nbrEqn{UNbr, eqn.dimensions()};
  volScalarField rhoNbr
  {
    // IOobject
    {
      "rho:UNbr",
      nbrMesh.time().timeName(),
      nbrMesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    nbrMesh,
    // dimensionedScalar("zero", dimDensity, 0.0)
    {"zero", dimDensity, 0.0}
  };
  volScalarField muNbr
  {
    // IOobject
    {
      "mu:UNbr",
      nbrMesh.time().timeName(),
      nbrMesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    nbrMesh,
    // dimensionedScalar("zero", dimViscosity, 0.0)
    {"zero", dimViscosity, 0.0}
  };
  const volScalarField& mu = mesh_.lookupObject<volScalarField>(muName_);
  // map local rho onto neighbour region
  meshInterp().mapSrcToTgt
    (
      rho.internalField(),
      plusEqOp<scalar>(),
      rhoNbr.internalField()
    );
  // map local mu onto neighbour region
  meshInterp().mapSrcToTgt
    (
      mu.internalField(),
      plusEqOp<scalar>(),
      muNbr.internalField()
    );
  porosityPtr_->addResistance(nbrEqn, rhoNbr, muNbr);
  // convert source from neighbour to local region
  fvMatrix<vector> porosityEqn{U, eqn.dimensions()};
  scalarField& Udiag = porosityEqn.diag();
  vectorField& Usource = porosityEqn.source();
  Udiag.setSize(eqn.diag().size(), 0.0);
  Usource.setSize(eqn.source().size(), vector::zero);
  meshInterp().mapTgtToSrc(nbrEqn.diag(), plusEqOp<scalar>(), Udiag);
  meshInterp().mapTgtToSrc(nbrEqn.source(), plusEqOp<vector>(), Usource);
  eqn -= porosityEqn;
}


bool mousse::fv::interRegionExplicitPorositySource::read(const dictionary& dict)
{
  if (interRegionOption::read(dict)) {
    coeffs_.readIfPresent("UName", UName_);
    coeffs_.readIfPresent("muName", muName_);
    // reset the porosity model?
    return true;
  }
  return false;
}

