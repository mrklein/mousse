#ifndef FV_OPTIONS_SOURCES_DERIVED_ROTOR_DISK_SOURCE_PROFILE_MODEL_PROFILE_MODEL_HPP_
#define FV_OPTIONS_SOURCES_DERIVED_ROTOR_DISK_SOURCE_PROFILE_MODEL_PROFILE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::profileModel
// Description
//   Base class for profile models

#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "dictionary.hpp"


namespace mousse {

class profileModel
{
protected:
  // Protected data
    //- Coefficients dictionary
    const dictionary dict_;
    //- Name of profile model
    const word name_;
    //- File name (optional)
    fileName fName_;
  // Protected Member Functions
    //- Return ture if file name is set
    bool readFromFile() const;
public:
  //- Runtime type information
  TYPE_NAME("profileModel");
    // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      profileModel,
      dictionary,
      (
        const dictionary& dict,
        const word& modelName
      ),
      (dict, modelName)
    );
  // Selectors
    //- Return a reference to the selected fvOption model
    static autoPtr<profileModel> New(const dictionary& dict);
  //- Constructor
  profileModel(const dictionary& dict, const word& modelName);
  //- Destructor
  virtual ~profileModel();
  // Member functions
    // Access
      //- Return const access to the source name
      const word& name() const;
    // Evaluation
      //- Return the Cd and Cl for a given angle-of-attack
      virtual void Cdl
      (
        const scalar alpha,
        scalar& Cd,
        scalar& Cl
      ) const = 0;
};

}  // namespace mousse

#endif

