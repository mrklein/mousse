// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::liftModel
// Description
// SourceFiles
//   lift_model.cpp
//   new_lift_model.cpp
#ifndef lift_model_hpp_
#define lift_model_hpp_
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class phasePair;
class liftModel
{
protected:
  // Protected data
    //- Phase pair
    const phasePair& pair_;
public:
  //- Runtime type information
  TYPE_NAME("liftModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      liftModel,
      dictionary,
      (
        const dictionary& dict,
        const phasePair& pair
      ),
      (dict, pair)
    );
  // Static data members
    //- Force dimensions
    static const dimensionSet dimF;
  // Constructors
    //- Construct from a dictionary and a phase pair
    liftModel
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~liftModel();
  // Selectors
    static autoPtr<liftModel> New
    (
      const dictionary& dict,
      const phasePair& pair
    );
  // Member Functions
    //- Return lift coefficient
    virtual tmp<volScalarField> Cl() const = 0;
    //- Return phase-intensive lift force
    virtual tmp<volVectorField> Fi() const;
    //- Return lift force
    virtual tmp<volVectorField> F() const;
    //- Return face lift force
    virtual tmp<surfaceScalarField> Ff() const;
};
}  // namespace mousse
#endif
