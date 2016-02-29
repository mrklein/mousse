#ifndef THERMOPHYSICAL_MODELS_BASIC_BASIC_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_BASIC_BASIC_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicThermo
// Description
//   Abstract base-class for fluid and solid thermodynamic properties
// SourceFiles
//   basic_thermo.cpp
#include "vol_fields.hpp"
#include "type_info.hpp"
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "word_io_list.hpp"
namespace mousse
{
class basicThermo
:
  public IOdictionary
{
protected:
  // Protected data
    //- Phase-name
    const word& phaseName_;
    // Fields
      //- Pressure [Pa]
      volScalarField& p_;
      //- Temperature [K]
      volScalarField T_;
      //- Laminar thermal diffusuvity [kg/m/s]
      volScalarField alpha_;
    //- Should the dpdt term be included in the enthalpy equation
    Switch dpdt_;
  // Protected Member Functions
    //- Construct as copy (not implemented)
    basicThermo(const basicThermo&);
    volScalarField& lookupOrConstruct
    (
      const fvMesh& mesh,
      const char* name
    ) const;
    //- Return the enthalpy/internal energy field boundary types
    //  by interrogating the temperature field boundary types
    wordList heBoundaryTypes();
    //- Return the enthalpy/internal energy field boundary base types
    //  by interrogating the temperature field boundary types
    wordList heBoundaryBaseTypes();
public:
  //- Runtime type information
  TYPE_NAME("basicThermo");
  //- Declare run-time constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    basicThermo,
    fvMesh,
    (const fvMesh& mesh, const word& phaseName),
    (mesh, phaseName)
  );
  // Constructors
    //- Construct from mesh and phase name
    basicThermo
    (
      const fvMesh&,
      const word& phaseName
    );
    //- Construct from mesh, dictionary and phase name
    basicThermo
    (
      const fvMesh&,
      const dictionary&,
      const word& phaseName
    );
  // Selectors
    //- Generic lookup for each of the related thermodynamics packages
    template<class Thermo, class Table>
    static typename Table::iterator lookupThermo
    (
      const dictionary& thermoDict,
      Table* tablePtr
    );
    //- Generic New for each of the related thermodynamics packages
    template<class Thermo>
    static autoPtr<Thermo> New
    (
      const fvMesh&,
      const word& phaseName=word::null
    );
    //- Generic New for each of the related thermodynamics packages
    template<class Thermo>
    static autoPtr<Thermo> New
    (
      const fvMesh&,
      const dictionary&,
      const word& phaseName=word::null
    );
    //- Specialisation of the Generic New for basicThermo
    static autoPtr<basicThermo> New
    (
      const fvMesh&,
      const word& phaseName=word::null
    );
  //- Destructor
  virtual ~basicThermo();
  // Member functions
    static const word dictName;
    static word phasePropertyName
    (
      const word& name,
      const word& phaseName
    )
    {
      return IOobject::groupName(name, phaseName);
    }
    word phasePropertyName(const word& name) const
    {
      return basicThermo::phasePropertyName(name, phaseName_);
    }
    static const basicThermo& lookupThermo(const fvPatchScalarField& pf);
    //- Check that the thermodynamics package is consistent
    //  with energy forms supported by the application
    void validate
    (
      const string& app,
      const word&
    ) const;
    //- Check that the thermodynamics package is consistent
    //  with energy forms supported by the application
    void validate
    (
      const string& app,
      const word&,
      const word&
    ) const;
    //- Check that the thermodynamics package is consistent
    //  with energy forms supported by the application
    void validate
    (
      const string& app,
      const word&,
      const word&,
      const word&
    ) const;
    //- Check that the thermodynamics package is consistent
    //  with energy forms supported by the application
    void validate
    (
      const string& app,
      const word&,
      const word&,
      const word&,
      const word&
    ) const;
    //- Split name of thermo package into a list of the components names
    static wordList splitThermoName
    (
      const word& thermoName,
      const int nCmpt
    );
    //- Update properties
    virtual void correct() = 0;
    //- Return true if the equation of state is incompressible
    //  i.e. rho != f(p)
    virtual bool incompressible() const = 0;
    //- Return true if the equation of state is isochoric
    //  i.e. rho = const
    virtual bool isochoric() const = 0;
    //- Should the dpdt term be included in the enthalpy equation
    Switch dpdt() const
    {
      return dpdt_;
    }
    // Access to thermodynamic state variables
      //- Pressure [Pa]
      //  Non-const access allowed for transport equations
      virtual volScalarField& p();
      //- Pressure [Pa]
      virtual const volScalarField& p() const;
      //- Density [kg/m^3]
      virtual tmp<volScalarField> rho() const = 0;
      //- Density for patch [kg/m^3]
      virtual tmp<scalarField> rho(const label patchi) const = 0;
      //- Enthalpy/Internal energy [J/kg]
      //  Non-const access allowed for transport equations
      virtual volScalarField& he() = 0;
      //- Enthalpy/Internal energy [J/kg]
      virtual const volScalarField& he() const = 0;
      //- Enthalpy/Internal energy
      //  for given pressure and temperature [J/kg]
      virtual tmp<volScalarField> he
      (
        const volScalarField& p,
        const volScalarField& T
      ) const = 0;
      //- Enthalpy/Internal energy for cell-set [J/kg]
      virtual tmp<scalarField> he
      (
        const scalarField& p,
        const scalarField& T,
        const labelList& cells
      ) const = 0;
      //- Enthalpy/Internal energy for patch [J/kg]
      virtual tmp<scalarField> he
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const = 0;
      //- Chemical enthalpy [J/kg]
      virtual tmp<volScalarField> hc() const = 0;
      //- Temperature from enthalpy/internal energy for cell-set
      virtual tmp<scalarField> THE
      (
        const scalarField& h,
        const scalarField& p,
        const scalarField& T0,      // starting temperature
        const labelList& cells
      ) const = 0;
      //- Temperature from enthalpy/internal energy for patch
      virtual tmp<scalarField> THE
      (
        const scalarField& h,
        const scalarField& p,
        const scalarField& T0,      // starting temperature
        const label patchi
      ) const = 0;
    // Fields derived from thermodynamic state variables
      //- Temperature [K]
      virtual const volScalarField& T() const;
      //- Temperature [K]
      //  Non-const access allowed for transport equations
      virtual volScalarField& T();
      //- Heat capacity at constant pressure [J/kg/K]
      virtual tmp<volScalarField> Cp() const = 0;
      //- Heat capacity at constant pressure for patch [J/kg/K]
      virtual tmp<scalarField> Cp
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const = 0;
      //- Heat capacity at constant volume [J/kg/K]
      virtual tmp<volScalarField> Cv() const = 0;
      //- Heat capacity at constant volume for patch [J/kg/K]
      virtual tmp<scalarField> Cv
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const = 0;
      //- Gamma = Cp/Cv []
      virtual tmp<volScalarField> gamma() const = 0;
      //- Gamma = Cp/Cv for patch []
      virtual tmp<scalarField> gamma
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const = 0;
      //- Heat capacity at constant pressure/volume [J/kg/K]
      virtual tmp<volScalarField> Cpv() const = 0;
      //- Heat capacity at constant pressure/volume for patch [J/kg/K]
      virtual tmp<scalarField> Cpv
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const = 0;
      //- Heat capacity ratio []
      virtual tmp<volScalarField> CpByCpv() const = 0;
      //- Heat capacity ratio for patch []
      virtual tmp<scalarField> CpByCpv
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const = 0;
    // Access to transport state variables
      //- Thermal diffusivity for enthalpy of mixture [kg/m/s]
      virtual const volScalarField& alpha() const;
      //- Thermal diffusivity for enthalpy of mixture for patch [kg/m/s]
      virtual const scalarField& alpha
      (
        const label patchi
      ) const;
    // Fields derived from transport state variables
      //- Thermal diffusivity for temperature of mixture [J/m/s/K]
      virtual tmp<volScalarField> kappa() const = 0;
      //- Thermal diffusivity for temperature
      //  of mixture for patch [J/m/s/K]
      virtual tmp<scalarField> kappa
      (
        const label patchi
      ) const = 0;
      //- Effective thermal diffusivity for temperature
      //  of mixture [J/m/s/K]
      virtual tmp<volScalarField> kappaEff
      (
        const volScalarField&
      ) const = 0;
      //- Effective thermal diffusivity for temperature
      //  of mixture for patch [J/m/s/K]
      virtual tmp<scalarField> kappaEff
      (
        const scalarField& alphat,
        const label patchi
      ) const = 0;
      //- Effective thermal diffusivity of mixture [kg/m/s]
      virtual tmp<volScalarField> alphaEff
      (
        const volScalarField& alphat
      ) const = 0;
      //- Effective thermal diffusivity of mixture for patch [kg/m/s]
      virtual tmp<scalarField> alphaEff
      (
        const scalarField& alphat,
        const label patchi
      ) const = 0;
    //- Read thermophysical properties dictionary
    virtual bool read();
};
}  // namespace mousse
#ifdef NoRepository
#   include "basic_thermo_templates.cpp"
#endif
#endif
