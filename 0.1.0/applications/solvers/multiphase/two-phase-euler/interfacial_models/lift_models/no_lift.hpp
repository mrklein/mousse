// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::liftModels::noLift
// Description
// SourceFiles
//   no_lift.cpp
#ifndef NO_LIFT_HPP_
#define NO_LIFT_HPP_
#include "lift_model.hpp"
namespace mousse
{
class phasePair;
namespace liftModels
{
class noLift
:
  public liftModel
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from a dictionary and a phase pair
    noLift
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~noLift();
  // Member Functions
    //- Lift coefficient
    virtual tmp<volScalarField> Cl() const;
    //- Lift force
    virtual tmp<volVectorField> F() const;
    //- Lift force on faces
    virtual tmp<surfaceScalarField> Ff() const;
};
}  // namespace liftModels
}  // namespace mousse
#endif
