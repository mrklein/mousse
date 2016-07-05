// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::SchillerNaumann
// Description
//   Schiller and Naumann drag model for dispersed bubbly flows.
// SourceFiles
//   schiller_naumann.cpp
#ifndef SCHILLER_NAUMANN_HPP_
#define SCHILLER_NAUMANN_HPP_
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
