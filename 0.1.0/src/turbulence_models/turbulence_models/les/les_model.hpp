// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModel
// Description
//   Templated abstract base class for LES SGS models
// SourceFiles
//   les_model.cpp
#ifndef les_model_hpp_
#define les_model_hpp_
#include "turbulence_model.hpp"
#include "les_delta.hpp"
namespace mousse
{
template<class BasicTurbulenceModel>
class LESModel
:
  public BasicTurbulenceModel
{
protected:
  // Protected data
    //- LES coefficients dictionary
    dictionary LESDict_;
    //- Turbulence on/off flag
    Switch turbulence_;
    //- Flag to print the model coeffs at run-time
    Switch printCoeffs_;
    //- Model coefficients dictionary
    dictionary coeffDict_;
    //- Lower limit of k
    dimensionedScalar kMin_;
    //- Run-time selectable delta model
    autoPtr<mousse::LESdelta> delta_;
  // Protected Member Functions
    //- Print model coefficients
    virtual void printCoeffs(const word& type);
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("LES");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      LESModel,
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
    LESModel
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
    //- Disallow default bitwise copy construct
    LESModel(const LESModel&) = delete;
    //- Disallow default bitwise assignment
    LESModel& operator=(const LESModel&) = delete;
  // Selectors
    //- Return a reference to the selected LES model
    static autoPtr<LESModel> New
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
  virtual ~LESModel()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
    // Access
      //- Const access to the coefficients dictionary
      virtual const dictionary& coeffDict() const
      {
        return coeffDict_;
      }
      //- Return the lower allowable limit for k (default: SMALL)
      const dimensionedScalar& kMin() const
      {
        return kMin_;
      }
      //- Allow kMin to be changed
      dimensionedScalar& kMin()
      {
        return kMin_;
      }
      //- Access function to filter width
      inline const volScalarField& delta() const
      {
        return delta_();
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
#   include "les_model.cpp"
#endif
#endif
