// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::transportModel
// Description
//   Base-class for all transport models used by the incompressible turbulence
//   models.
// SourceFiles
//   transport_model.cpp
#ifndef transport_model_hpp_
#define transport_model_hpp_
#include "primitive_fields_fwd.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
class transportModel
{
public:
  //- Runtime type information
  TYPE_NAME("transportModel");
  // Constructors
    //- Construct from components
    transportModel
    ();
    //- Disallow copy construct
    transportModel(const transportModel&) = delete;
    //- Disallow default bitwise assignment
    transportModel& operator=(const transportModel&) = delete;
  //- Destructor
  virtual ~transportModel();
  // Member Functions
    //- Return the laminar viscosity
    virtual tmp<volScalarField> nu() const = 0;
    //- Return the laminar viscosity for patch
    virtual tmp<scalarField> nu(const label patchi) const = 0;
    //- Correct the laminar viscosity
    virtual void correct() = 0;
    //- Read transportProperties dictionary
    virtual bool read() = 0;
};
}  // namespace mousse
#endif
