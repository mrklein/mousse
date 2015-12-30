// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModel
// Description
//   Templated abstract base class for RAS turbulence models
// SourceFiles
//   ras_model.cpp
#ifndef ras_model_hpp_
#define ras_model_hpp_
#include "turbulence_model.hpp"
namespace mousse
{
template<class BasicTurbulenceModel>
class RASModel
:
  public BasicTurbulenceModel
{
protected:
  // Protected data
    //- RAS coefficients dictionary
    dictionary RASDict_;
    //- Turbulence on/off flag
    Switch turbulence_;
    //- Flag to print the model coeffs at run-time
    Switch printCoeffs_;
    //- Model coefficients dictionary
    dictionary coeffDict_;
    //- Lower limit of k
    dimensionedScalar kMin_;
    //- Lower limit of epsilon
    dimensionedScalar epsilonMin_;
    //- Lower limit for omega
    dimensionedScalar omegaMin_;
  // Protected Member Functions
    //- Print model coefficients
    virtual void printCoeffs(const word& type);
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    RASModel(const RASModel&);
    //- Disallow default bitwise assignment
    void operator=(const RASModel&);
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TypeName("RAS");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      RASModel,
      dictionary,
      (
        const alphaField& alpha,
        const rhoField& rho,
        const volVectorField& U,
        const surfaceScalarField& alphaRhoPhi,
        const surfaceScalarField& phi,
        const transportModel& transport,
        const word& propertiesName
      ),
      (alpha, rho, U, alphaRhoPhi, phi, transport, propertiesName)
    );
  // Constructors
    //- Construct from components
    RASModel
    (
      const word& type,
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName
    );
  // Selectors
    //- Return a reference to the selected RAS model
    static autoPtr<RASModel> New
    (
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName
    );
  //- Destructor
  virtual ~RASModel()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
    // Access
      //- Return the lower allowable limit for k (default: SMALL)
      const dimensionedScalar& kMin() const
      {
        return kMin_;
      }
      //- Return the lower allowable limit for epsilon (default: SMALL)
      const dimensionedScalar& epsilonMin() const
      {
        return epsilonMin_;
      }
      //- Return the lower allowable limit for omega (default: SMALL)
      const dimensionedScalar& omegaMin() const
      {
        return omegaMin_;
      }
      //- Allow kMin to be changed
      dimensionedScalar& kMin()
      {
        return kMin_;
      }
      //- Allow epsilonMin to be changed
      dimensionedScalar& epsilonMin()
      {
        return epsilonMin_;
      }
      //- Allow omegaMin to be changed
      dimensionedScalar& omegaMin()
      {
        return omegaMin_;
      }
      //- Const access to the coefficients dictionary
      virtual const dictionary& coeffDict() const
      {
        return coeffDict_;
      }
    //- Return the effective viscosity
    virtual tmp<volScalarField> nuEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField
        (
          IOobject::groupName("nuEff", this->U_.group()),
          this->nut() + this->nu()
        )
      );
    }
    //- Return the effective viscosity on patch
    virtual tmp<scalarField> nuEff(const label patchi) const
    {
      return this->nut(patchi) + this->nu(patchi);
    }
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();
};
}  // namespace mousse
#ifdef NoRepository
#   include "ras_model.cpp"
#endif
#endif
