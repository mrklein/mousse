// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressibleTurbulenceModel
// Description
//   Abstract base class for turbulence models (RAS, LES and laminar).
// SourceFiles
//   compressible_turbulence_model.cpp
#ifndef compressible_turbulence_model_hpp_
#define compressible_turbulence_model_hpp_
#include "_turbulence_model.hpp"
namespace mousse
{
// Forward declarations
class fvMesh;
class compressibleTurbulenceModel
:
  public turbulenceModel
{
protected:
  // Protected data
    const volScalarField& rho_;
  // Protected member functions
    //- ***HGW Temporary function to be removed when the run-time selectable
    //  thermal transport layer is complete
    virtual void correctNut()
    {}
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    compressibleTurbulenceModel(const compressibleTurbulenceModel&);
    //- Disallow default bitwise assignment
    void operator=(const compressibleTurbulenceModel&);
public:
  //- Runtime type information
  TypeName("compressibleTurbulenceModel");
  // Constructors
    //- Construct from components
    compressibleTurbulenceModel
    (
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const word& propertiesName
    );
  //- Destructor
  virtual ~compressibleTurbulenceModel()
  {}
  // Member functions
    //- Return the density field
    const volScalarField& rho() const
    {
      return rho_;
    }
    //- Return the volumetric flux field
    virtual tmp<surfaceScalarField> phi() const;
    //- Return the effective stress tensor including the laminar stress
    virtual tmp<volSymmTensorField> devRhoReff() const = 0;
    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevRhoReff(volVectorField& U) const = 0;
    //- Correct the turbulence thermal diffusivity for energy transport
    virtual void correctEnergyTransport();
};
}  // namespace mousse
#endif
