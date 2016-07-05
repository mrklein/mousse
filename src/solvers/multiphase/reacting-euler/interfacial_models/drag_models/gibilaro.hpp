#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_DRAG_MODELS_GIBILARO_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_DRAG_MODELS_GIBILARO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
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
#include "drag_model.hpp"
namespace mousse
{
class phasePair;
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
    //- Construct from a dictionary and a phase pair
    Gibilaro
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~Gibilaro();
  // Member Functions
    //- Drag coefficient
    virtual tmp<volScalarField> CdRe() const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
