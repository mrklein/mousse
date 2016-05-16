// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mean_velocity_force.hpp"
#include "fv_matrices.hpp"
#include "dimensioned_field.hpp"
#include "ifstream.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Member Functions
namespace mousse {
namespace fv {

DEFINE_TYPE_NAME_AND_DEBUG(meanVelocityForce, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  option,
  meanVelocityForce,
  dictionary
);

}
}


// Private Member Functions 
void mousse::fv::meanVelocityForce::writeProps
(
  const scalar gradP
) const
{
  // Only write on output time
  if (!mesh_.time().outputTime())
    return;
  IOdictionary propsDict
  {
    IOobject
    {
      name_ + "Properties",
      mesh_.time().timeName(),
      "uniform",
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    }
  };
  propsDict.add("gradient", gradP);
  propsDict.regIOobject::write();
}


// Constructors 
mousse::fv::meanVelocityForce::meanVelocityForce
(
  const word& sourceName,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  cellSetOption{sourceName, modelType, dict, mesh},
  Ubar_{coeffs_.lookup("Ubar")},
  gradP0_{0.0},
  dGradP_{0.0},
  flowDir_{Ubar_/mag(Ubar_)},
  relaxation_{coeffs_.lookupOrDefault<scalar>("relaxation", 1.0)},
  rAPtr_{nullptr}
{
  coeffs_.lookup("fieldNames") >> fieldNames_;
  if (fieldNames_.size() != 1) {
    FATAL_ERROR_IN
    (
      "mousse::fv::meanVelocityForce::"
      "meanVelocityForce"
      "("
      "  const word&, "
      "  const word&, "
      "  const dictionary&, "
      "  const fvMesh&"
      ")"
    )
    << "Source can only be applied to a single field.  Current "
    << "settings are:" << fieldNames_ << exit(FatalError);
  }
  applied_.setSize(fieldNames_.size(), false);
  // Read the initial pressure gradient from file if it exists
  IFstream propsFile
  {
    mesh_.time().timePath()/"uniform"/(name_ + "Properties")
  };
  if (propsFile.good()) {
    Info << "    Reading pressure gradient from file" << endl;
    dictionary propsDict(dictionary::null, propsFile);
    propsDict.lookup("gradient") >> gradP0_;
  }
  Info << "    Initial pressure gradient = " << gradP0_ << nl << endl;
}


// Member Functions 
mousse::scalar mousse::fv::meanVelocityForce::magUbarAve
(
  const volVectorField& U
) const
{
  scalar magUbarAve = 0.0;
  const scalarField& cv = mesh_.V();
  FOR_ALL(cells_, i) {
    label cellI = cells_[i];
    scalar volCell = cv[cellI];
    magUbarAve += (flowDir_ & U[cellI])*volCell;
  }
  reduce(magUbarAve, sumOp<scalar>());
  magUbarAve /= V_;
  return magUbarAve;
}


void mousse::fv::meanVelocityForce::correct(volVectorField& U)
{
  const scalarField& rAU = rAPtr_().internalField();
  // Integrate flow variables over cell set
  scalar rAUave = 0.0;
  const scalarField& cv = mesh_.V();
  FOR_ALL(cells_, i) {
    label cellI = cells_[i];
    scalar volCell = cv[cellI];
    rAUave += rAU[cellI]*volCell;
  }
  // Collect across all processors
  reduce(rAUave, sumOp<scalar>());
  // Volume averages
  rAUave /= V_;
  scalar magUbarAve = this->magUbarAve(U);
  // Calculate the pressure gradient increment needed to adjust the average
  // flow-rate to the desired value
  dGradP_ = relaxation_*(mag(Ubar_) - magUbarAve)/rAUave;
  // Apply correction to velocity field
  FOR_ALL(cells_, i) {
    label cellI = cells_[i];
    U[cellI] += flowDir_*rAU[cellI]*dGradP_;
  }
  scalar gradP = gradP0_ + dGradP_;
  Info << "Pressure gradient source: uncorrected Ubar = " << magUbarAve
    << ", pressure gradient = " << gradP << endl;
  writeProps(gradP);
}


void mousse::fv::meanVelocityForce::addSup
(
  fvMatrix<vector>& eqn,
  const label fieldI
)
{
  DimensionedField<vector, volMesh> Su
  {
    IOobject
    {
      name_ + fieldNames_[fieldI] + "Sup",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh_,
    {"zero", eqn.dimensions()/dimVolume, vector::zero}
  };
  scalar gradP = gradP0_ + dGradP_;
  UIndirectList<vector>(Su, cells_) = flowDir_*gradP;
  eqn += Su;
}


void mousse::fv::meanVelocityForce::addSup
(
  const volScalarField& /*rho*/,
  fvMatrix<vector>& eqn,
  const label fieldI
)
{
  this->addSup(eqn, fieldI);
}


void mousse::fv::meanVelocityForce::constrain
(
  fvMatrix<vector>& eqn,
  const label
)
{
  if (rAPtr_.empty()) {
    rAPtr_.reset
    (
      new volScalarField
      {
        IOobject
        {
          name_ + ":rA",
          mesh_.time().timeName(),
          mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        1.0/eqn.A()
      }
    );
  } else {
    rAPtr_() = 1.0/eqn.A();
  }
  gradP0_ += dGradP_;
  dGradP_ = 0.0;
}

