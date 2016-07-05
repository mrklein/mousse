#ifndef FINITE_VOLUME_CFD_TOOLS_GENERAL_SRF_RPM_HPP_
#define FINITE_VOLUME_CFD_TOOLS_GENERAL_SRF_RPM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SRF::rpm
// Description
//   Basic SRF model whereby angular velocity is specified in terms of
//   a (global) axis and revolutions-per-minute [rpm]

#include "srf_model.hpp"


namespace mousse {
namespace SRF {

class rpm
:
  public SRFModel
{
  // Private data
    //- Revolutions per minute
    scalar rpm_;
public:
  //- Runtime type information
  TYPE_NAME("rpm");
  // Constructors
    //- Construct from components
    rpm(const volVectorField& U);
    //- Disallow default bitwise copy construct
    rpm(const rpm&) = delete;
    //- Disallow default bitwise assignment
    rpm& operator=(const rpm&) = delete;
  //- Destructor
  ~rpm();
  // Member functions
    // I-O
      //- Read
      bool read();
};
}  // namespace SRF
}  // namespace mousse
#endif
