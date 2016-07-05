// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::liftModels::TomiyamaLift
// Description
//   Lift model of Tomiyama et al.
//   Reference:
//   \verbatim
//     "Transverse migration of single bubbles in simple shear flows"
//     Tomiyama, A., Tamai, H., Zun, I., Hosokawa, S.,
//     Chemical Engineering Science
//     Volume 57, Issue 11, June 2002, pp. 1849-1858
//   \endverbatim
//   The coefficient for pow3(EoH) proposed by Tomiyama (2002) has been modified
//   to make the model continuous at EoH = 10.7 while maintaining the
//   lift coefficient proposed by Tomiyama (2002) when EoH >= 10.7.
// SourceFiles
//   tomiyama_lift.cpp
#ifndef TOMIYAMA_LIFT_HPP_
#define TOMIYAMA_LIFT_HPP_
#include "lift_model.hpp"
namespace mousse
{
class phasePair;
namespace liftModels
{
class TomiyamaLift
:
  public liftModel
{
public:
  //- Runtime type information
  TYPE_NAME("Tomiyama");
  // Constructors
    //- Construct from a dictionary and a phase pair
    TomiyamaLift
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~TomiyamaLift();
  // Member Functions
    //- Lift coefficient
    virtual tmp<volScalarField> Cl() const;
};
}  // namespace liftModels
}  // namespace mousse
#endif
