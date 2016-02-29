#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_FILM_THERMO_MODEL_FILM_THERMO_MODEL_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_FILM_THERMO_MODEL_FILM_THERMO_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::filmThermoModel
// Description
//   Base class for film thermo models
// SourceFiles
//   film_thermo_model.cpp
//   film_thermo_model_new.cpp
#include "film_sub_model_base.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class filmThermoModel
:
  public filmSubModelBase
{
public:
  //- Runtime type information
  TYPE_NAME("filmThermoModel");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      filmThermoModel,
      dictionary,
      (
        surfaceFilmModel& owner,
        const dictionary& dict
      ),
      (owner, dict)
    );
  // Constructors
    //- Construct null
    filmThermoModel(surfaceFilmModel& owner);
    //- Construct from type name, dictionary and surface film model
    filmThermoModel
    (
      const word& modelType,
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    filmThermoModel(const filmThermoModel&) = delete;
    //- Disallow default bitwise assignment
    filmThermoModel& operator=(const filmThermoModel&) = delete;
  // Selectors
    //- Return a reference to the selected phase change model
    static autoPtr<filmThermoModel> New
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  //- Destructor
  virtual ~filmThermoModel();
  // Member Functions
    //- Return the specie name
    virtual const word& name() const = 0;
    // Elemental access
      //- Return density [kg/m3]
      virtual scalar rho(const scalar p, const scalar T) const = 0;
      //- Return dynamic viscosity [Pa.s]
      virtual scalar mu(const scalar p, const scalar T) const = 0;
      //- Return surface tension [kg/s2]
      virtual scalar sigma(const scalar p, const scalar T) const = 0;
      //- Return specific heat capacity [J/kg/K]
      virtual scalar Cp(const scalar p, const scalar T) const = 0;
      //- Return thermal conductivity [W/m/K]
      virtual scalar kappa(const scalar p, const scalar T) const = 0;
      //- Return diffusivity [m2/s]
      virtual scalar D(const scalar p, const scalar T) const = 0;
      //- Return latent heat [J/kg]
      virtual scalar hl(const scalar p, const scalar T) const = 0;
      //- Return vapour pressure [Pa]
      virtual scalar pv(const scalar p, const scalar T) const = 0;
      //- Return molecular weight [kg/kmol]
      virtual scalar W() const = 0;
      //- Return boiling temperature [K]
      virtual scalar Tb(const scalar p) const = 0;
    // Field access
      //- Return density [kg/m3]
      virtual tmp<volScalarField> rho() const = 0;
      //- Return dynamic viscosity [Pa.s]
      virtual tmp<volScalarField> mu() const = 0;
      //- Return surface tension [kg/s2]
      virtual tmp<volScalarField> sigma() const = 0;
      //- Return specific heat capacity [J/kg/K]
      virtual tmp<volScalarField> Cp() const = 0;
      //- Return thermal conductivity [W/m/K]
      virtual tmp<volScalarField> kappa() const = 0;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
