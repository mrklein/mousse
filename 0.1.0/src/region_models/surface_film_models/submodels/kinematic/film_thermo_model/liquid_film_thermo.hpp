// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::liquidFilmThermo
// Description
//   Liquid thermo model
// SourceFiles
//   liquid_film_thermo.cpp
#ifndef liquid_film_thermo_hpp_
#define liquid_film_thermo_hpp_
#include "film_thermo_model.hpp"
#include "liquid_properties.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Forward class declarations
class thermoSingleLayer;
class liquidFilmThermo
:
  public filmThermoModel
{
protected:
  // Protected data
    //- Liquid name
    word name_;
    //- Pointer to the liquid properties
    const liquidProperties* liquidPtr_;
    //- Flag to indicate that model owns the liquid object
    bool ownLiquid_;
    //- Flag to indicate that reference values of p and T should be used
    bool useReferenceValues_;
    //- Reference pressure [pa]
    scalar pRef_;
    //- Reference temperature [K]
    scalar TRef_;
  // Protected member functions
    //- Return a reference to a thermo film
    const thermoSingleLayer& thermoFilm() const;
    //- Initialise the liquid pointer
    void initLiquid(const dictionary& dict);
    //- Disallow default bitwise copy construct
    liquidFilmThermo(const liquidFilmThermo&);
    //- Disallow default bitwise assignment
    void operator=(const liquidFilmThermo&);
public:
  //- Runtime type information
  TypeName("liquid");
  // Constructors
    //- Construct from surface film model and dictionary
    liquidFilmThermo
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  //- Destructor
  virtual ~liquidFilmThermo();
  // Member Functions
    //- Return the specie name
    virtual const word& name() const;
    // Elemental access
      //- Return density [kg/m3]
      virtual scalar rho(const scalar p, const scalar T) const;
      //- Return dynamic viscosity [Pa.s]
      virtual scalar mu(const scalar p, const scalar T) const;
      //- Return surface tension [kg/s2]
      virtual scalar sigma(const scalar p, const scalar T) const;
      //- Return specific heat capacity [J/kg/K]
      virtual scalar Cp(const scalar p, const scalar T) const;
      //- Return thermal conductivity [W/m/K]
      virtual scalar kappa(const scalar p, const scalar T) const;
      //- Return diffusivity [m2/s]
      virtual scalar D(const scalar p, const scalar T) const;
      //- Return latent heat [J/kg]
      virtual scalar hl(const scalar p, const scalar T) const;
      //- Return vapour pressure [Pa]
      virtual scalar pv(const scalar p, const scalar T) const;
      //- Return molecular weight [kg/kmol]
      virtual scalar W() const;
      //- Return boiling temperature [K]
      virtual scalar Tb(const scalar p) const;
    // Field access
      //- Return density [kg/m3]
      virtual tmp<volScalarField> rho() const;
      //- Return dynamic viscosity [Pa.s]
      virtual tmp<volScalarField> mu() const;
      //- Return surface tension [kg/s2]
      virtual tmp<volScalarField> sigma() const;
      //- Return specific heat capacity [J/kg/K]
      virtual tmp<volScalarField> Cp() const;
      //- Return thermal conductivity [W/m/K]
      virtual tmp<volScalarField> kappa() const;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
