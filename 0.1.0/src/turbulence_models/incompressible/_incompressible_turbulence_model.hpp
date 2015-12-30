// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IncompressibleTurbulenceModel
// Description
//   Templated abstract base class for single-phase incompressible
//   turbulence models.
// SourceFiles
//   _incompressible_turbulence_model.cpp
#ifndef _incompressible_turbulence_model_hpp_
#define _incompressible_turbulence_model_hpp_
#include "_turbulence_model.hpp"
#include "incompressible_turbulence_model.hpp"
#include "fv_matrix.hpp"
namespace mousse
{
template<class TransportModel>
class IncompressibleTurbulenceModel
:
  public TurbulenceModel
  <
    geometricOneField,
    geometricOneField,
    incompressibleTurbulenceModel,
    TransportModel
  >
{
public:
  typedef geometricOneField alphaField;
  typedef geometricOneField rhoField;
  typedef TransportModel transportModel;
  // Constructors
    //- Construct
    IncompressibleTurbulenceModel
    (
      const word& type,
      const geometricOneField& alpha,
      const geometricOneField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const TransportModel& transport,
      const word& propertiesName
    );
  // Selectors
    //- Return a reference to the selected turbulence model
    static autoPtr<IncompressibleTurbulenceModel> New
    (
      const volVectorField& U,
      const surfaceScalarField& phi,
      const TransportModel& trasportModel,
      const word& propertiesName = turbulenceModel::propertiesName
    );
  //- Destructor
  virtual ~IncompressibleTurbulenceModel()
  {}
  // Member Functions
    //- Return the effective stress tensor
    virtual tmp<volSymmTensorField> devReff() const;
    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevReff(volVectorField& U) const;
    //- Return the effective stress tensor
    virtual tmp<volSymmTensorField> devRhoReff() const;
    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevRhoReff(volVectorField& U) const;
    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevRhoReff
    (
      const volScalarField& rho,
      volVectorField& U
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_incompressible_turbulence_model.cpp"
#endif
#endif
