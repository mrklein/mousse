// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_ink_jet_fv_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "mathematical_constants.hpp"


// Static Data Members

namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(dynamicInkJetFvMesh, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dynamicFvMesh, dynamicInkJetFvMesh, IOobject);

}


// Constructors 
mousse::dynamicInkJetFvMesh::dynamicInkJetFvMesh(const IOobject& io)
:
  dynamicFvMesh{io},
  dynamicMeshCoeffs_
  {
    IOdictionary
    {
      {
        "dynamicMeshDict",
        io.time().constant(),
        *this,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      }
    }.subDict(typeName + "Coeffs")
  },
  amplitude_{readScalar(dynamicMeshCoeffs_.lookup("amplitude"))},
  frequency_{readScalar(dynamicMeshCoeffs_.lookup("frequency"))},
  refPlaneX_{readScalar(dynamicMeshCoeffs_.lookup("refPlaneX"))},
  stationaryPoints_
  {
    {
      "points",
      io.time().constant(),
      meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    }
  }
{
  Info << "Performing a dynamic mesh calculation: " << endl
    << "amplitude: " << amplitude_
    << " frequency: " << frequency_
    << " refPlaneX: " << refPlaneX_ << endl;
}


// Destructor 
mousse::dynamicInkJetFvMesh::~dynamicInkJetFvMesh()
{}


// Member Functions 
bool mousse::dynamicInkJetFvMesh::update()
{
  using constant::mathematical::twoPi;
  scalar scalingFunction = 0.5*(::cos(twoPi*frequency_*time().value()) - 1.0);
  Info << "Mesh scaling. Time = " << time().value() << " scaling: "
    << scalingFunction << endl;
  pointField newPoints = stationaryPoints_;
  scalarField& xi =
    pos(-(stationaryPoints_.component(vector::X)) - refPlaneX_)();
  newPoints.replace
  (
    vector::X,
    stationaryPoints_.component(vector::X)*
    (
      1.0 + xi*amplitude_*scalingFunction
    )
  );
  fvMesh::movePoints(newPoints);
  volVectorField& U =
    const_cast<volVectorField&>(lookupObject<volVectorField>("U"));
  U.correctBoundaryConditions();
  return true;
}

