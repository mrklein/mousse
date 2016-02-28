// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::granularPressureModel
// SourceFiles
//   granular_pressure_model.cpp
#ifndef granular_pressure_model_hpp_
#define granular_pressure_model_hpp_
#include "dictionary.hpp"
#include "vol_fields.hpp"
#include "dimensioned_types.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
class granularPressureModel
{
protected:
  // Protected data
    const dictionary& dict_;
public:
  //- Runtime type information
  TYPE_NAME("granularPressureModel");
  // Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    granularPressureModel,
    dictionary,
    (
      const dictionary& dict
    ),
    (dict)
  );
  // Constructors
    //- Construct from components
    granularPressureModel(const dictionary& dict);
    //- Disallow default bitwise copy construct
    granularPressureModel(const granularPressureModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const granularPressureModel&) = delete;
  // Selectors
    static autoPtr<granularPressureModel> New
    (
      const dictionary& dict
    );
  //- Destructor
  virtual ~granularPressureModel();
  // Member Functions
    //- Granular pressure coefficient
    virtual tmp<volScalarField> granularPressureCoeff
    (
      const volScalarField& alpha1,
      const volScalarField& g0,
      const volScalarField& rho1,
      const dimensionedScalar& e
    ) const = 0;
    //- Derivative of the granular pressure coefficient
    virtual tmp<volScalarField> granularPressureCoeffPrime
    (
      const volScalarField& alpha1,
      const volScalarField& g0,
      const volScalarField& g0prime,
      const volScalarField& rho1,
      const dimensionedScalar& e
    ) const = 0;
    virtual bool read()
    {
      return true;
    }
};
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
