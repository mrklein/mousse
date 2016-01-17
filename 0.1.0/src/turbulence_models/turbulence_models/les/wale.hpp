// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::WALE
// Group
//   grpLESTurbulence
// Description
//   The Wall-adapting local eddy-viscosity (WALE) SGS model.
//   Reference:
//   \verbatim
//     Nicoud, F., & Ducros, F. (1999).
//     Subgrid-scale stress modelling based on the square of the velocity
//     gradient tensor.
//     Flow, Turbulence and Combustion, 62(3), 183-200.
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     WALECoeffs
//     {
//       Ck                  0.094;
//       Ce                  1.048;e
//       Cw                  0.325;
//     }
//   \endverbatim
// SeeAlso
//   mousse::LESModels::Smagorinsky
// SourceFiles
//   wale.cpp
#ifndef wale_hpp_
#define wale_hpp_
#include "les_model.hpp"
#include "les_eddy_viscosity.hpp"
namespace mousse
{
namespace LESModels
{
template<class BasicTurbulenceModel>
class WALE
:
  public LESeddyViscosity<BasicTurbulenceModel>
{
protected:
  // Protected data
    dimensionedScalar Ck_;
    dimensionedScalar Cw_;
  // Protected Member Functions
    //- Return the deviatoric symmetric part of the square of the given
    //  velocity gradient field
    tmp<volSymmTensorField> Sd(const volTensorField& gradU) const;
    //- Return SGS kinetic energy
    //  calculated from the given velocity gradient
    tmp<volScalarField> k(const volTensorField& gradU) const;
    //- Update the SGS eddy-viscosity
    virtual void correctNut();
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("WALE");
  // Constructors
    //- Construct from components
    WALE
    (
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName,
      const word& type = typeName
    );
    // Disallow default bitwise copy construct and assignment
    WALE(const WALE&) = delete;
    WALE& operator=(const WALE&) = delete;
  //- Destructor
  virtual ~WALE()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
    //- Return SGS kinetic energy
    virtual tmp<volScalarField> k() const
    {
      return k(fvc::grad(this->U_));
    }
    //- Return sub-grid disipation rate
    virtual tmp<volScalarField> epsilon() const;
    //- Correct Eddy-Viscosity and related properties
    virtual void correct();
};
}  // namespace LESModels
}  // namespace mousse
#ifdef NoRepository
#   include "wale.cpp"
#endif
#endif
