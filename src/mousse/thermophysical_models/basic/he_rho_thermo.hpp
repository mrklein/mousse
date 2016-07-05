#ifndef THERMOPHYSICAL_MODELS_BASIC_HE_RHO_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_BASIC_HE_RHO_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::heRhoThermo
// Description
//   Energy for a mixture based on density

#include "rho_thermo.hpp"
#include "he_thermo.hpp"


namespace mousse {

template<class BasicPsiThermo, class MixtureType>
class heRhoThermo
:
  public heThermo<BasicPsiThermo, MixtureType>
{
  // Private Member Functions
    //- Calculate the thermo variables
    void calculate();
public:
  //- Runtime type information
  TYPE_NAME("heRhoThermo");
  // Constructors
    //- Construct from mesh and phase name
    heRhoThermo
    (
      const fvMesh&,
      const word& phaseName
    );
    //- Disable construct as copy
    heRhoThermo(const heRhoThermo<BasicPsiThermo, MixtureType>&) = delete;
  //- Destructor
  virtual ~heRhoThermo();
  // Member functions
    //- Update properties
    virtual void correct();
};

}  // namespace mousse

#include "he_rho_thermo.ipp"

#endif
