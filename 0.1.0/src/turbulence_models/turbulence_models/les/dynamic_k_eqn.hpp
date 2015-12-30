// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::dynamicKEqn
// Group
//   grpLESTurbulence
// Description
//   Dynamic one equation eddy-viscosity model
//   Eddy viscosity SGS model using a modeled balance equation to simulate
//   the behaviour of k in which a dynamic procedure is applied to evaluate the
//   coefficients.
//   Reference:
//   \verbatim
//     Kim, W and Menon, S. (1995).
//     A new dynamic one-equation subgrid-scale model for
//     large eddy simulation.
//     In 33rd Aerospace Sciences Meeting and Exhibit, Reno, NV, 1995.
//   \endverbatim
//   There are no default model coefficients but the filter used for KK must be
//   supplied, e.g.
//   \verbatim
//     dynamicKEqnCoeffs
//     {
//       filter simple;
//     }
//   \endverbatim
// SourceFiles
//   dynamic_k_eqn.cpp
#ifndef dynamic_k_eqn_hpp_
#define dynamic_k_eqn_hpp_
#include "les_eddy_viscosity.hpp"
#include "simple_filter.hpp"
namespace mousse
{
namespace LESModels
{
template<class BasicTurbulenceModel>
class dynamicKEqn
:
  public LESeddyViscosity<BasicTurbulenceModel>
{
  // Private Member Functions
    // Disallow default bitwise copy construct and assignment
    dynamicKEqn(const dynamicKEqn&);
    dynamicKEqn& operator=(const dynamicKEqn&);
protected:
  // Protected data
    // Fields
      volScalarField k_;
    // Filters
      simpleFilter simpleFilter_;
      autoPtr<LESfilter> filterPtr_;
      LESfilter& filter_;
  // Protected Member Functions
    //- Calculate Ck by filtering the velocity field U
    volScalarField Ck
    (
      const volSymmTensorField& D,
      const volScalarField& KK
    ) const;
    //- Calculate Ce by filtering the velocity field U
    volScalarField Ce
    (
      const volSymmTensorField& D,
      const volScalarField& KK
    ) const;
    volScalarField Ce() const;
    //- Update sub-grid eddy-viscosity
    void correctNut
    (
      const volSymmTensorField& D,
      const volScalarField& KK
    );
    virtual void correctNut();
    virtual tmp<fvScalarMatrix> kSource() const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TypeName("dynamicKEqn");
  // Constructors
    //- Construct from components
    dynamicKEqn
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
  //- Destructor
  virtual ~dynamicKEqn()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
    //- Return SGS kinetic energy
    virtual tmp<volScalarField> k() const
    {
      return k_;
    }
    //- Return sub-grid disipation rate
    virtual tmp<volScalarField> epsilon() const;
    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField("DkEff", this->nut_ + this->nu())
      );
    }
    //- Correct Eddy-Viscosity and related properties
    virtual void correct();
};
}  // namespace LESModels
}  // namespace mousse
#ifdef NoRepository
#   include "dynamic_k_eqn.cpp"
#endif
#endif
