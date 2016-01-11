// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidThermo
// Description
//   Fundamental solid thermodynamic properties
// SourceFiles
//   solid_thermo.cpp
#ifndef solid_thermo_hpp_
#define solid_thermo_hpp_
#include "vol_fields.hpp"
#include "run_time_selection_tables.hpp"
#include "fv_mesh.hpp"
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "basic_thermo.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class solidThermo
:
  public basicThermo
{
protected:
  // Protected data
    //- Density field [kg/m^3]
    //  Named 'rhoThermo' to avoid (potential) conflict with solver density
    volScalarField rho_;
public:
  //- Runtime type information
  TYPE_NAME("solidThermo");
  // Declare run-time constructor selection tables
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    solidThermo,
    fvMesh,
    (const fvMesh& mesh, const word& phaseName),
    (mesh, phaseName)
  );
  // Declare run-time constructor selection tables
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    solidThermo,
    dictionary,
    (const fvMesh& mesh, const dictionary& dict, const word& phaseName),
    (mesh, dict, phaseName)
  );
  // Constructors
    //- Construct from mesh and phase name
    solidThermo
    (
      const fvMesh&,
      const word& phaseName
    );
    //- Construct from mesh, dictionary and phase name
    solidThermo
    (
      const fvMesh&,
      const dictionary& dict,
      const word& phaseName
    );
    //- Return a pointer to a new solidThermo created from
    //  the solidThermophysicalProperties dictionary
    static autoPtr<solidThermo> New
    (
      const fvMesh&,
      const word& phaseName=word::null
    );
    //- Return a pointer to a new solidThermo created from
    //  local dictionary
    static autoPtr<solidThermo> New
    (
      const fvMesh&,
      const dictionary&,
      const word& phaseName=word::null
    );
  //- Destructor
  virtual ~solidThermo();
  // Member functions
    // Fields derived from thermodynamic state variables
      //- Density [kg/m^3]
      virtual tmp<volScalarField> rho() const;
      //- Density for patch [kg/m^3]
      virtual tmp<scalarField> rho(const label patchi) const;
      //- Return non-const access to the local density field [kg/m^3]
      virtual volScalarField& rho();
      //- Thermal conductivity [W/m/K]
      virtual tmp<volVectorField> Kappa() const = 0;
      //- Return true if thermal conductivity is isotropic
      virtual bool isotropic() const = 0;
    // Per patch calculation
      //- Anisotropic thermal conductivity [W/m/K]
      virtual tmp<vectorField> Kappa
      (
        const label patchI
      ) const = 0;
  // I-O
    //- Read thermophysicalProperties dictionary
    virtual bool read();
};
}  // namespace mousse
#endif
