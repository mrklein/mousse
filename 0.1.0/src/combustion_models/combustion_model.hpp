// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::combustionModel
// Description
//   Base class for combustion models
// SourceFiles
//   combustion_model.cpp
#ifndef combustion_model_hpp_
#define combustion_model_hpp_
#include "iodictionary.hpp"
#include "turbulent_fluid_thermo_model.hpp"
namespace mousse
{
class combustionModel
:
  public IOdictionary
{
protected:
  // Protected data
    //- Reference to the turbulence model
    compressibleTurbulenceModel* turbulencePtr_;
    //- Reference to the mesh database
    const fvMesh& mesh_;
    //- Active
    Switch active_;
    //- Dictionary of the model
    dictionary coeffs_;
    //- Model type
    const word modelType_;
    //- Phase name
    const word phaseName_;
public:
  //- Runtime type information
  TYPE_NAME("combustionModel");
  // Constructors
    //- Construct from components
    combustionModel
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName=word::null
    );
    //- Disallow copy construct
    combustionModel(const combustionModel&) = delete;
    //- Disallow default bitwise assignment
    combustionModel& operator=(const combustionModel&) = delete;
  //- Destructor
  virtual ~combustionModel();
  // Member Functions
    // Access
      //- Return const access to the mesh database
      inline const fvMesh& mesh() const;
      //- Return const access to phi
      inline const surfaceScalarField& phi() const;
      //- Return const access to rho
      virtual tmp<volScalarField> rho() const = 0;
      //- Return access to turbulence
      inline const compressibleTurbulenceModel& turbulence() const;
      //- Set turbulence
      inline void setTurbulence
      (
        compressibleTurbulenceModel& turbModel
      );
      //- Is combustion active?
      inline const Switch& active() const;
      //- Return const dictionary of the model
      inline const dictionary& coeffs() const;
  // Evolution
    //- Correct combustion rate
    virtual void correct() = 0;
    //- Fuel consumption rate matrix, i.e. source term for fuel equation
    virtual tmp<fvScalarMatrix> R(volScalarField& Y) const = 0;
    //- Heat release rate calculated from fuel consumption rate matrix
    virtual tmp<volScalarField> dQ() const = 0;
    //-  Return source for enthalpy equation [kg/m/s3]
    virtual tmp<volScalarField> Sh() const;
  // IO
    //- Update properties from given dictionary
    virtual bool read();
};
}  // namespace mousse
#include "combustion_model_i.hpp"
#endif
