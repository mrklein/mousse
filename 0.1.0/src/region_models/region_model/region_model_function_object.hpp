// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionModelFunctionObject
// Description
//   Region model function object base class
// SourceFiles
//   region_model_function_object.cpp
//   region_model_function_object_new.cpp
#ifndef region_model_function_object_hpp_
#define region_model_function_object_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
namespace regionModels
{
class regionModel;
class regionModelFunctionObject
{
protected:
  // Protected data
    //- Dictionary
    dictionary dict_;
    //- Reference to the region model
    regionModel& owner_;
    //- Model type name
    word modelType_;
public:
  //- Runtime type information
  TypeName("regionModelFunctionObject");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    regionModelFunctionObject,
    dictionary,
    (
      const dictionary& dict,
      regionModel& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    regionModelFunctionObject(regionModel& owner);
    //- Construct from dictionary
    regionModelFunctionObject
    (
      const dictionary& dict,
      regionModel& owner,
      const word& modelType
    );
    //- Construct copy
    regionModelFunctionObject(const regionModelFunctionObject& ppm);
    //- Construct and return a clone
    virtual autoPtr<regionModelFunctionObject> clone() const
    {
      return autoPtr<regionModelFunctionObject>
      (
        new regionModelFunctionObject(*this)
      );
    }
  //- Destructor
  virtual ~regionModelFunctionObject();
  //- Selector
  static autoPtr<regionModelFunctionObject> New
  (
    const dictionary& dict,
    regionModel& owner,
    const word& modelType
  );
  // Member Functions
    // Evaluation
      //- Pre-evolve region hook
      virtual void preEvolveRegion();
      //- Post-evolve region hook
      virtual void postEvolveRegion();
  // I-O
    //- write
    virtual void write() const;
};
}  // namespace regionModels
}  // namespace mousse
#endif
