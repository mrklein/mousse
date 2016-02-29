#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_V2F_BASE_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_V2F_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::v2fBase
// Group
//   grpRASTurbulence
// Description
//   Abstract base-class for v2-f models to provide BCs access to the v2 and f
//   fields.
// SeeAlso
//   mousse::RASModels::v2f
// SourceFiles
//   v2f_base.cpp
#include "ras_model.hpp"
namespace mousse
{
namespace RASModels
{
class v2fBase
{
public:
  //- Runtime type information
  TYPE_NAME("v2fBase");
  // Constructors
    v2fBase()
    {}
  //- Destructor
  virtual ~v2fBase()
  {}
  // Member Functions
    //- Return turbulence stress normal to streamlines
    virtual tmp<volScalarField> v2() const = 0;
    //- Return the damping function
    virtual tmp<volScalarField> f() const = 0;
};
}  // namespace RASModels
}  // namespace mousse
#endif
