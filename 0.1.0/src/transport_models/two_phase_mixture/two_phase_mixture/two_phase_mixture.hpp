#ifndef TRANSPORT_MODELS_TWO_PHASE_MIXTURE_TWO_PHASE_MIXTURE_TWO_PHASE_MIXTURE_HPP_
#define TRANSPORT_MODELS_TWO_PHASE_MIXTURE_TWO_PHASE_MIXTURE_TWO_PHASE_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::twoPhaseMixture
// Description
//   A two-phase mixture model
// SourceFiles
//   two_phase_mixture.cpp
#include "vol_fields.hpp"
namespace mousse
{
class twoPhaseMixture
{
protected:
  // Protected data
    word phase1Name_;
    word phase2Name_;
    volScalarField alpha1_;
    volScalarField alpha2_;
public:
  // Constructors
    //- Construct from components
    twoPhaseMixture
    (
      const fvMesh& mesh,
      const dictionary& dict
    );
  //- Destructor
  ~twoPhaseMixture()
  {}
  // Member Functions
    const word& phase1Name() const
    {
      return phase1Name_;
    }
    const word& phase2Name() const
    {
      return phase2Name_;
    }
    //- Return the phase-fraction of phase 1
    const volScalarField& alpha1() const
    {
      return alpha1_;
    }
    //- Return the phase-fraction of phase 1
    volScalarField& alpha1()
    {
      return alpha1_;
    }
    //- Return the phase-fraction of phase 2
    const volScalarField& alpha2() const
    {
      return alpha2_;
    }
    //- Return the phase-fraction of phase 2
    volScalarField& alpha2()
    {
      return alpha2_;
    }
};
}  // namespace mousse
#endif
