// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulentDispersionModel
// Description
// SourceFiles
//   turbulent_dispersion_model.cpp
//   new_turbulent_dispersion_model.cpp
#ifndef turbulent_dispersion_model_hpp_
#define turbulent_dispersion_model_hpp_
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class phasePair;
class turbulentDispersionModel
{
protected:
  // Protected data
    //- Phase pair
    const phasePair& pair_;
public:
  //- Runtime type information
  TYPE_NAME("turbulentDispersionModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      turbulentDispersionModel,
      dictionary,
      (
        const dictionary& dict,
        const phasePair& pair
      ),
      (dict, pair)
    );
  // Static data members
    //- Diffusivity dimensions
    static const dimensionSet dimD;
    //- Force dimensions
    static const dimensionSet dimF;
  // Constructors
    //- Construct from a dictionary and a phase pair
    turbulentDispersionModel
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~turbulentDispersionModel();
  // Selectors
    static autoPtr<turbulentDispersionModel> New
    (
      const dictionary& dict,
      const phasePair& pair
    );
  // Member Functions
    //- Turbulent diffusivity
    //  multiplying the gradient of the phase-fraction
    virtual tmp<volScalarField> D() const = 0;
    //- Turbulent dispersion force
    virtual tmp<volVectorField> F() const;
    //- Turbulent dispersion force on faces
    virtual tmp<surfaceScalarField> Ff() const;
};
}  // namespace mousse
#endif
