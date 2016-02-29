#ifndef THERMOPHYSICAL_MODELS_LAMINAR_FLAME_SPEED_GULDERS_EGR_HPP_
#define THERMOPHYSICAL_MODELS_LAMINAR_FLAME_SPEED_GULDERS_EGR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::laminarFlameSpeedModels::GuldersEGR
// Description
//   Laminar flame speed obtained from Gulder's correlation with EGR modelling.
// SourceFiles
//   gulders_egr.cpp
#include "laminar_flame_speed.hpp"
namespace mousse
{
namespace laminarFlameSpeedModels
{
class GuldersEGR
:
  public laminarFlameSpeed
{
  // Private Data
    dictionary coeffsDict_;
    scalar W_;
    scalar eta_;
    scalar xi_;
    scalar f_;
    scalar alpha_;
    scalar beta_;
  // Private Member Functions
    inline scalar SuRef
    (
      scalar phi
    ) const;
    inline scalar Su0pTphi
    (
      scalar p,
      scalar Tu,
      scalar phi,
      scalar Yres
    ) const;
    tmp<volScalarField> Su0pTphi
    (
      const volScalarField& p,
      const volScalarField& Tu,
      scalar phi
    ) const;
    tmp<volScalarField> Su0pTphi
    (
      const volScalarField& p,
      const volScalarField& Tu,
      const volScalarField& phi,
      const volScalarField& egr
    ) const;
    //- Construct as copy (not implemented)
    GuldersEGR(const GuldersEGR&);
    void operator=(const GuldersEGR&);
public:
  //- Runtime type information
  TYPE_NAME("GuldersEGR");
  // Constructors
    //- Construct from dictionary and psiuReactionThermo
    GuldersEGR
    (
      const dictionary&,
      const psiuReactionThermo&
    );
  //- Destructor
  virtual ~GuldersEGR();
  // Member functions
    //- Return the laminar flame speed [m/s]
    tmp<volScalarField> operator()() const;
};
} // End laminarFlameSpeedModels
}  // namespace mousse
#endif
