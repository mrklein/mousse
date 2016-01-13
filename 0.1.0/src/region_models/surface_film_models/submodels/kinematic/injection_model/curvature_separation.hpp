// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::curvatureSeparation
// Description
//   Curvature film separation model
//   Assesses film curvature via the mesh geometry and calculates a force
//   balance of the form:
//     F_sum = F_inertial + F_body + F_surface
//   If F_sum < 0, the film separates. Similarly, if F_sum > 0 the film will
//   remain attached.
//   Based on description given by
//     Owen and D. J. Ryley. The flow of thin liquid films around corners.
//     International Journal of Multiphase Flow, 11(1):51-62, 1985.
// SourceFiles
//   curvature_separation.cpp
#ifndef curvature_separation_hpp_
#define curvature_separation_hpp_
#include "injection_model.hpp"
#include "surface_fields.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class curvatureSeparation
:
  public injectionModel
{
protected:
  // Protected data
    //- Gradient of surface normals
    volTensorField gradNHat_;
    //- Minimum gravity driven film thickness (non-dimensionalised delta/R1)
    scalar deltaByR1Min_;
    //- List of radii for patches - if patch not defined, radius
    // calculated based on mesh geometry
    List<Tuple2<label, scalar> > definedPatchRadii_;
    //- Magnitude of gravity vector
    scalar magG_;
    //- Direction of gravity vector
    vector gHat_;
  // Protected Member Functions
    //- Calculate local (inverse) radius of curvature
    tmp<volScalarField> calcInvR1(const volVectorField& U) const;
    //- Calculate the cosine of the angle between gravity vector and
    //  cell out flow direction
    tmp<scalarField> calcCosAngle(const surfaceScalarField& phi) const;
public:
  //- Runtime type information
  TYPE_NAME("curvatureSeparation");
  // Constructors
    //- Construct from surface film model
    curvatureSeparation
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    curvatureSeparation(const curvatureSeparation&) = delete;
    //- Disallow default bitwise assignment
    curvatureSeparation& operator=(const curvatureSeparation&) = delete;
  //- Destructor
  virtual ~curvatureSeparation();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct
      (
        scalarField& availableMass,
        scalarField& massToInject,
        scalarField& diameterToInject
      );
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
