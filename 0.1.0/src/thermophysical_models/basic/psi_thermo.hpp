// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::psiThermo
// Description
//   Basic thermodynamic properties based on compressibility
// SourceFiles
//   psi_thermo.cpp
#ifndef psi_thermo_hpp_
#define psi_thermo_hpp_
#include "fluid_thermo.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class psiThermo
:
  public fluidThermo
{
protected:
  // Protected data
    //- Compressibility [s^2/m^2]
    volScalarField psi_;
    //- Dynamic viscosity [kg/m/s]
    volScalarField mu_;
  // Protected Member Functions
    //- Construct as copy (not implemented)
    psiThermo(const psiThermo&);
public:
  //- Runtime type information
  TypeName("psiThermo");
  //- Declare run-time constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    psiThermo,
    fvMesh,
    (const fvMesh& mesh, const word& phaseName),
    (mesh, phaseName)
  );
  // Constructors
    //- Construct from mesh and phase name
    psiThermo
    (
      const fvMesh&,
      const word& phaseName
    );
  //- Selector
  static autoPtr<psiThermo> New
  (
    const fvMesh& mesh,
    const word& phaseName=word::null
  );
  //- Destructor
  virtual ~psiThermo();
  // Member functions
    // Fields derived from thermodynamic state variables
      //- Density [kg/m^3] - uses current value of pressure
      virtual tmp<volScalarField> rho() const;
      //- Density for patch [kg/m^3]
      virtual tmp<scalarField> rho(const label patchi) const;
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
