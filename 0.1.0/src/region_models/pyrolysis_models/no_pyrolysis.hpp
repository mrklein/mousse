#ifndef REGION_MODELS_PYROLYSIS_MODELS_NO_PYROLYSIS_HPP_
#define REGION_MODELS_PYROLYSIS_MODELS_NO_PYROLYSIS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noPyrolysis
// Description
//   Dummy surface pyrolysis model for 'none'
// SourceFiles
//   no_pyrolysis.cpp
#include "pyrolysis_model.hpp"
#include "vol_fields_fwd.hpp"
#include "basic_solid_chemistry_model.hpp"
#include "radiation_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace pyrolysisModels
{
class noPyrolysis
:
  public pyrolysisModel
{
protected:
  // Protected member functions
    //- Read control parameters from dictionary
    virtual bool read();
    //- Read control parameters from dictionary
    virtual bool read(const dictionary& dict);
    //- Reset solidChemistryModel and solidThermo pointers
    void constructThermoChemistry();
    //- Reference to the solid chemistry model
    autoPtr<basicSolidChemistryModel> solidChemistry_;
    //- Reference to solid thermo
    autoPtr<solidReactionThermo> solidThermo_;
    //- Pointer to radiation model
    autoPtr<radiation::radiationModel> radiation_;
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from type name and mesh
    noPyrolysis
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& regionType
    );
    //- Construct from type name and mesh and dict
    noPyrolysis
    (
      const word& modelType,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& regionType
    );
    //- Disallow default bitwise copy construct
    noPyrolysis(const noPyrolysis&) = delete;
    //- Disallow default bitwise assignment
    noPyrolysis& operator=(const noPyrolysis&) = delete;
  //- Destructor
  virtual ~noPyrolysis();
  // Member Functions
    // Fields
      //- Return density [kg/m3]
      virtual const volScalarField& rho() const;
      //- Return const temperature [K]
      virtual const volScalarField& T() const;
      //- Return specific heat capacity [J/kg/K]
      virtual const tmp<volScalarField> Cp() const;
      //- Return the region absorptivity [1/m]
      virtual tmp<volScalarField> kappaRad() const;
      //- Return the region thermal conductivity [W/m/k]
      virtual tmp<volScalarField> kappa() const;
      //- Return the total gas mass flux to primary region [kg/m2/s]
      virtual const surfaceScalarField& phiGas() const;
    // Evolution
      //- Pre-evolve region
      virtual void preEvolveRegion();
      //- Evolve the pyrolysis equations
      virtual void evolveRegion();
};
}  // namespace pyrolysisModels
}  // namespace regionModels
}  // namespace mousse
#endif
