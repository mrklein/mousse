// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::immiscibleIncompressibleThreePhaseMixture
// Description
//   An immiscible incompressible two-phase mixture transport model
// SourceFiles
//   immiscible_incompressible_three_phase_mixture.cpp
#ifndef IMMISCIBLE_INCOMPRESSIBLE_THREE_PHASE_MIXTURE_HPP_
#define IMMISCIBLE_INCOMPRESSIBLE_THREE_PHASE_MIXTURE_HPP_
#include "incompressible_three_phase_mixture.hpp"
#include "three_phase_interface_properties.hpp"
namespace mousse
{
class immiscibleIncompressibleThreePhaseMixture
:
  public incompressibleThreePhaseMixture,
  public threePhaseInterfaceProperties
{
public:
  // Constructors
    //- Construct from components
    immiscibleIncompressibleThreePhaseMixture
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  virtual ~immiscibleIncompressibleThreePhaseMixture()
  {}
  // Member Functions
    //- Correct the transport and interface properties
    virtual void correct()
    {
      incompressibleThreePhaseMixture::correct();
      threePhaseInterfaceProperties::correct();
    }
};
}  // namespace mousse
#endif
