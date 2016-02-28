// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallLubricationModel
// Description
// SourceFiles
//   wall_lubrication_model.cpp
//   new_wall_lubrication_model.cpp
#ifndef wall_lubrication_model_hpp_
#define wall_lubrication_model_hpp_
#include "wall_dependent_model.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class phasePair;
class wallLubricationModel
:
  public wallDependentModel
{
protected:
  // Protected data
    //- Phase pair
    const phasePair& pair_;
public:
  //- Runtime type information
  TYPE_NAME("wallLubricationModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      wallLubricationModel,
      dictionary,
      (
        const dictionary& dict,
        const phasePair& pair
      ),
      (dict, pair)
    );
  // Static data members
    //- Coefficient dimensions
    static const dimensionSet dimF;
  // Constructors
    //- Construct from components
    wallLubricationModel
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~wallLubricationModel();
  // Selectors
    static autoPtr<wallLubricationModel> New
    (
      const dictionary& dict,
      const phasePair& pair
    );
  // Member Functions
    //- Return phase-intensive wall lubrication force
    virtual tmp<volVectorField> Fi() const = 0;
    //- Return wall lubrication force
    virtual tmp<volVectorField> F() const;
    //- Return face wall lubrication force
    virtual tmp<surfaceScalarField> Ff() const;
};
}  // namespace mousse
#endif
