// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::viscosityModel
// Description
// SourceFiles
//   viscosity_model.cpp
#ifndef VISCOSITY_MODEL_HPP_
#define VISCOSITY_MODEL_HPP_
#include "dictionary.hpp"
#include "vol_fields.hpp"
#include "dimensioned_types.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
class viscosityModel
{
  // Private member functions
    //- Disallow default bitwise copy construct
    viscosityModel(const viscosityModel&);
    //- Disallow default bitwise assignment
    void operator=(const viscosityModel&);
protected:
  // Protected data
    const dictionary& dict_;
public:
  //- Runtime type information
  TYPE_NAME("viscosityModel");
  // Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    viscosityModel,
    dictionary,
    (
      const dictionary& dict
    ),
    (dict)
  );
  // Constructors
    //- Construct from components
    viscosityModel(const dictionary& dict);
  // Selectors
    static autoPtr<viscosityModel> New
    (
      const dictionary& dict
    );
  //- Destructor
  virtual ~viscosityModel();
  // Member Functions
    virtual tmp<volScalarField> nu
    (
      const volScalarField& alpha1,
      const volScalarField& Theta,
      const volScalarField& g0,
      const volScalarField& rho1,
      const volScalarField& da,
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
