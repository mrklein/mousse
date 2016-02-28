// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::conductivityModel
// SourceFiles
//   conductivity_model.cpp
#ifndef conductivity_model_hpp_
#define conductivity_model_hpp_
#include "dictionary.hpp"
#include "vol_fields.hpp"
#include "dimensioned_types.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
class conductivityModel
{
protected:
  // Protected data
    const dictionary& dict_;
public:
  //- Runtime type information
  TYPE_NAME("conductivityModel");
  // Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    conductivityModel,
    dictionary,
    (
      const dictionary& dict
    ),
    (dict)
  );
  // Constructors
    //- Construct from components
    conductivityModel(const dictionary& dict);
    //- Disallow default bitwise copy construct
    conductivityModel(const conductivityModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const conductivityModel&) = delete;
  // Selectors
    static autoPtr<conductivityModel> New
    (
      const dictionary& dict
    );
  //- Destructor
  virtual ~conductivityModel();
  // Member Functions
    virtual tmp<volScalarField> kappa
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
