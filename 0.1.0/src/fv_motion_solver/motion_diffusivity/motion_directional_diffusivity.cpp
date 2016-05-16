// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "motion_directional_diffusivity.hpp"
#include "surface_interpolate.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "velocity_motion_solver.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(motionDirectionalDiffusivity, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  motionDiffusivity,
  motionDirectionalDiffusivity,
  Istream
);

}


// Constructors 
mousse::motionDirectionalDiffusivity::motionDirectionalDiffusivity
(
  const fvMesh& mesh,
  Istream& mdData
)
:
  uniformDiffusivity{mesh, mdData},
  diffusivityVector_{mdData}
{}


// Destructor 
mousse::motionDirectionalDiffusivity::~motionDirectionalDiffusivity()
{}


// Member Functions 
void mousse::motionDirectionalDiffusivity::correct()
{
  static bool first = true;
  if (!first) {
    const volVectorField& cellMotionU =
      mesh().lookupObject<volVectorField>("cellMotionU");
    volVectorField D
    {
      IOobject
      {
        "D",
        mesh().time().timeName(),
        mesh()
      },
      diffusivityVector_.y()*vector::one
      + (diffusivityVector_.x() - diffusivityVector_.y())*cellMotionU
      /(mag(cellMotionU) + dimensionedScalar("small", dimVelocity, SMALL)),
      zeroGradientFvPatchVectorField::typeName
    };
    D.correctBoundaryConditions();
    const surfaceVectorField n{mesh().Sf()/mesh().magSf()};
    faceDiffusivity_ == (n & cmptMultiply(fvc::interpolate(D), n));
  } else {
    first = false;
    const velocityMotionSolver& mSolver =
      mesh().lookupObject<velocityMotionSolver>("dynamicMeshDict");
    const_cast<velocityMotionSolver&>(mSolver).solve();
    correct();
  }
}

