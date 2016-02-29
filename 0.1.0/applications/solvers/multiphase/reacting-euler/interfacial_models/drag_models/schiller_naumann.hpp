#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_DRAG_MODELS_SCHILLER_NAUMANN_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_DRAG_MODELS_SCHILLER_NAUMANN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::SchillerNaumann
// Description
//   Schiller and Naumann drag model for dispersed bubbly flows.
// SourceFiles
//   schiller_naumann.cpp
#include "drag_model.hpp"
namespace mousse
{
class phasePair;
namespace dragModels
{
class SchillerNaumann
:
  public dragModel
{
  // Private data
    //- Residual Reynolds Number
    const dimensionedScalar residualRe_;
public:
  //- Runtime type information
  TYPE_NAME("SchillerNaumann");
  // Constructors
    //- Construct from a dictionary and a phase pair
    SchillerNaumann
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~SchillerNaumann();
  // Member Functions
    //- Drag coefficient
    virtual tmp<volScalarField> CdRe() const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
