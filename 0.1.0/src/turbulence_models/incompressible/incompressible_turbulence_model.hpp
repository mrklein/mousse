// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::incompressibleTurbulenceModel
// Description
//   Abstract base class for turbulence models (RAS, LES and laminar).
// SourceFiles
//   incompressible_turbulence_model.cpp
#ifndef incompressible_turbulence_model_hpp_
#define incompressible_turbulence_model_hpp_
#include "turbulence_model.hpp"
#include "geometric_one_field.hpp"
namespace mousse
{
// Forward declarations
class fvMesh;
class incompressibleTurbulenceModel
:
  public turbulenceModel
{
protected:
  // Protected data
    geometricOneField rho_;
  // Protected member functions
    //- ***HGW Temporary function to be removed when the run-time selectable
    //  thermal transport layer is complete
    virtual void correctNut()
    {}
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    incompressibleTurbulenceModel(const incompressibleTurbulenceModel&);
    //- Disallow default bitwise assignment
    void operator=(const incompressibleTurbulenceModel&);
public:
  //- Runtime type information
  TypeName("incompressibleTurbulenceModel");
  // Constructors
    //- Construct from components
    incompressibleTurbulenceModel
    (
      const geometricOneField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const word& propertiesName
    );
  //- Destructor
  virtual ~incompressibleTurbulenceModel()
  {}
  // Member Functions
    //- Return the laminar dynamic viscosity
    virtual tmp<volScalarField> mu() const;
    //- Return the laminar dynamic viscosity on patch
    virtual tmp<scalarField> mu(const label patchi) const;
    //- Return the turbulence dynamic viscosity
    virtual tmp<volScalarField> mut() const;
    //- Return the turbulence dynamic viscosity on patch
    virtual tmp<scalarField> mut(const label patchi) const;
    //- Return the effective dynamic viscosity
    virtual tmp<volScalarField> muEff() const;
    //- Return the effective dynamic viscosity on patch
    virtual tmp<scalarField> muEff(const label patchi) const;
    //- Return the effective stress tensor including the laminar stress
    virtual tmp<volSymmTensorField> devReff() const = 0;
    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevReff(volVectorField& U) const = 0;
};
}  // namespace mousse
#endif
