// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceTensionModel
// Description
// SourceFiles
//   surface_tension_model.cpp
//   new_aspect_ratio_model.cpp
#ifndef SURFACE_TENSION_MODEL_HPP_
#define SURFACE_TENSION_MODEL_HPP_
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class phasePair;
class surfaceTensionModel
:
  public regIOobject
{
protected:
  // Protected data
    //- Phase pair
    const phasePair& pair_;
public:
  //- Runtime type information
  TYPE_NAME("surfaceTensionModel");
  // Declare runtime construction
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    surfaceTensionModel,
    dictionary,
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    ),
    (dict, pair, registerObject)
  );
  // Static data members
    //- Coefficient dimensions
    static const dimensionSet dimSigma;
  // Constructors
    //- Construct from a dictionary and a phase pair
    surfaceTensionModel
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~surfaceTensionModel();
  // Selectors
    static autoPtr<surfaceTensionModel> New
    (
      const dictionary& dict,
      const phasePair& pair
    );
  // Member Functions
    //- Aspect ratio
    virtual tmp<volScalarField> sigma() const = 0;
    //- Dummy write for regIOobject
    bool writeData(Ostream& os) const;
};
}  // namespace mousse
#endif
