// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SRF::rpm
// Description
//   Basic SRF model whereby angular velocity is specified in terms of
//   a (global) axis and revolutions-per-minute [rpm]
// SourceFiles
//   rpm.cpp
#ifndef SRFModelRpm_H
#define SRFModelRpm_H
#include "srf_model.hpp"
namespace mousse
{
namespace SRF
{
class rpm
:
  public SRFModel
{
  // Private data
    //- Revolutions per minute
    scalar rpm_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    rpm(const rpm&);
    //- Disallow default bitwise assignment
    void operator=(const rpm&);
public:
  //- Runtime type information
  TypeName("rpm");
  // Constructors
    //- Construct from components
    rpm(const volVectorField& U);
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
