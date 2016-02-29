#ifndef SOLVERS_MULTIPHASE_MULTIPHASE_EULER_INTERFACIAL_MODELS_DRAG_MODELS_ERGUN_HPP_
#define SOLVERS_MULTIPHASE_MULTIPHASE_EULER_INTERFACIAL_MODELS_DRAG_MODELS_ERGUN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::Ergun
// Description
//   H, Enwald, E. Peirano, A-E Almstedt
//   'Eulerian Two-Phase Flow Theory Applied to Fluidization'
//   Int. J. Multiphase Flow, Vol. 22, Suppl, pp. 21-66 (1996)
//   Eq. 104, p. 42
// SourceFiles
//   ergun.cpp
#include "drag_model.hpp"
namespace mousse
{
namespace dragModels
{
class Ergun
:
  public dragModel
{
public:
  //- Runtime type information
  TYPE_NAME("Ergun");
  // Constructors
    //- Construct from components
    Ergun
    (
      const dictionary& interfaceDict,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~Ergun();
  // Member Functions
    tmp<volScalarField> K(const volScalarField& Ur) const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
