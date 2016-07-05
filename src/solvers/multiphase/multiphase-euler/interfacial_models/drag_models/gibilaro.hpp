// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::Gibilaro
// Description
//   H, Enwald, E. Peirano, A-E Almstedt
//   'Eulerian Two-Phase Flow Theory Applied to Fluidization'
//   Int. J. Multiphase Flow, Vol. 22, Suppl, pp. 21-66 (1996)
//   Eq. 106, p. 43
// SourceFiles
//   gibilaro.cpp
#ifndef GIBILARO_HPP_
#define GIBILARO_HPP_
#include "drag_model.hpp"
namespace mousse
{
namespace dragModels
{
class Gibilaro
:
  public dragModel
{
public:
  //- Runtime type information
  TYPE_NAME("Gibilaro");
  // Constructors
    //- Construct from components
    Gibilaro
    (
      const dictionary& interfaceDict,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~Gibilaro();
  // Member Functions
    tmp<volScalarField> K(const volScalarField& Ur) const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
