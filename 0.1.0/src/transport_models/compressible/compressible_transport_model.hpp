// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressibleTransportModel
// Description
//   Base-class for all transport models used by the compressible turbulence
//   models.
// SourceFiles
//   compressible_transport_model.cpp
#ifndef compressible_transport_model_hpp_
#define compressible_transport_model_hpp_
#include "primitive_fields_fwd.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
class compressibleTransportModel
{
  // Private Member Functions
    //- Disallow copy construct
    compressibleTransportModel(const compressibleTransportModel&);
    //- Disallow default bitwise assignment
    void operator=(const compressibleTransportModel&);
public:
  //- Runtime type information
  TypeName("compressibleTransportModel");
  // Constructors
    //- Construct from components
    compressibleTransportModel
    ();
  //- Destructor
  virtual ~compressibleTransportModel();
  // Member Functions
    //- Return the dynamic laminar viscosity
    virtual tmp<volScalarField> mu() const = 0;
    //- Return the dynamic laminar viscosity for patch
    virtual tmp<scalarField> mu(const label patchi) const = 0;
    //- Return the laminar viscosity
    virtual tmp<volScalarField> nu() const = 0;
    //- Return the laminar viscosity for patch
    virtual tmp<scalarField> nu(const label patchi) const = 0;
};
}  // namespace mousse
#endif
