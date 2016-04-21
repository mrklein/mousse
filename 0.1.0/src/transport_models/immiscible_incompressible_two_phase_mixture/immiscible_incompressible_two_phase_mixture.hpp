#ifndef TRANSPORT_MODELS_IMMISCIBLE_INCOMPRESSIBLE_TWO_PHASE_MIXTURE_IMMISCIBLE_INCOMPRESSIBLE_TWO_PHASE_MIXTURE_HPP_
#define TRANSPORT_MODELS_IMMISCIBLE_INCOMPRESSIBLE_TWO_PHASE_MIXTURE_IMMISCIBLE_INCOMPRESSIBLE_TWO_PHASE_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::immiscibleIncompressibleTwoPhaseMixture
// Description
//   An immiscible incompressible two-phase mixture transport model

#include "incompressible_two_phase_mixture.hpp"
#include "interface_properties.hpp"


namespace mousse {

class immiscibleIncompressibleTwoPhaseMixture
:
  public incompressibleTwoPhaseMixture,
  public interfaceProperties
{
public:
  // Constructors
    //- Construct from components
    immiscibleIncompressibleTwoPhaseMixture
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  virtual ~immiscibleIncompressibleTwoPhaseMixture()
  {}
  // Member Functions
    //- Correct the transport and interface properties
    virtual void correct()
    {
      incompressibleTwoPhaseMixture::correct();
      interfaceProperties::correct();
    }
    //- Read base transportProperties dictionary
    virtual bool read()
    {
      return
        incompressibleTwoPhaseMixture::read()
      && interfaceProperties::read();
    }
};

}  // namespace mousse

#endif

