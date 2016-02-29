#ifndef THERMOPHYSICAL_MODELS_BASIC_RHO_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_BASIC_RHO_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rhoThermo
// Description
//   Basic thermodynamic properties based on density
// SourceFiles
//   rho_thermo.cpp
#include "fluid_thermo.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class rhoThermo
:
  public fluidThermo
{
protected:
  // Protected data
    //- Density field [kg/m^3]
    //  Named 'rhoThermo' to avoid (potential) conflict with solver density
    volScalarField rho_;
    //- Compressibility [s^2/m^2]
    volScalarField psi_;
    //- Dynamic viscosity [kg/m/s]
    volScalarField mu_;
  // Protected Member Functions
    //- Construct as copy (not implemented)
    rhoThermo(const rhoThermo&);
public:
  //- Runtime type information
  TYPE_NAME("rhoThermo");
  //- Declare run-time constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    rhoThermo,
    fvMesh,
    (const fvMesh& mesh, const word& phaseName),
    (mesh, phaseName)
  );
  // Constructors
    //- Construct from mesh and phase name
    rhoThermo
    (
      const fvMesh&,
      const word& phaseName
    );
    //- Construct from mesh, dictionary and phase name
    rhoThermo
    (
      const fvMesh&,
      const dictionary&,
      const word& phaseName
    );
  //- Selector
  static autoPtr<rhoThermo> New
  (
    const fvMesh&,
    const word& phaseName=word::null
  );
  //- Destructor
  virtual ~rhoThermo();
  // Member functions
    // Fields derived from thermodynamic state variables
      //- Density [kg/m^3]
      virtual tmp<volScalarField> rho() const;
      //- Density for patch [kg/m^3]
      virtual tmp<scalarField> rho(const label patchi) const;
      //- Return non-const access to the local density field [kg/m^3]
      virtual volScalarField& rho();
      //- Compressibility [s^2/m^2]
      virtual const volScalarField& psi() const;
    // Access to transport state variables
      //- Dynamic viscosity of mixture [kg/m/s]
      virtual tmp<volScalarField> mu() const;
      //- Dynamic viscosity of mixture for patch [kg/m/s]
      virtual tmp<scalarField> mu(const label patchi) const;
};
}  // namespace mousse
#endif
