#ifndef THERMOPHYSICAL_MODELS_BASIC_HE_PSI_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_BASIC_HE_PSI_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hePsiThermo
// Description
//   Energy for a mixture based on compressibility

#include "psi_thermo.hpp"
#include "he_thermo.hpp"


namespace mousse {

template<class BasicPsiThermo, class MixtureType>
class hePsiThermo
:
  public heThermo<BasicPsiThermo, MixtureType>
{
  // Private Member Functions
    //- Calculate the thermo variables
    void calculate();
public:
  //- Runtime type information
  TYPE_NAME("hePsiThermo");
  // Constructors
    //- Construct from mesh and phase name
    hePsiThermo
    (
      const fvMesh&,
      const word& phaseName
    );
    //- Disable construct as copy
    hePsiThermo(const hePsiThermo<BasicPsiThermo, MixtureType>&) = delete;
  //- Destructor
  virtual ~hePsiThermo();
  // Member functions
    //- Update properties
    virtual void correct();
};

}  // namespace mousse

#include "he_psi_thermo.ipp"

#endif
