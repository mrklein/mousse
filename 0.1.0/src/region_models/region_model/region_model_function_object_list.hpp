// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionModelFunctionObjectList
// Description
//   List of cloud function objects
// SourceFiles
//   region_model_function_object_list_i.hpp
//   region_model_function_object_list.cpp
#ifndef region_model_function_object_list_hpp_
#define region_model_function_object_list_hpp_
#include "ptr_list.hpp"
#include "region_model_function_object.hpp"
namespace mousse
{
namespace regionModels
{
class regionModel;
class regionModelFunctionObjectList
:
  public PtrList<regionModelFunctionObject>
{
protected:
  // Protected Data
    //- Reference to the owner region model
    regionModel& owner_;
    //- Dictionary
    const dictionary dict_;
public:
  // Constructors
    //- Null constructor
    regionModelFunctionObjectList(regionModel& owner);
    //- Construct from mesh
    regionModelFunctionObjectList
    (
      regionModel& owner,
      const dictionary& dict,
      const bool readFields = true
    );
    //- Construct copy
    regionModelFunctionObjectList
    (
      const regionModelFunctionObjectList& rmfol
    );
  //- Destructor
  virtual ~regionModelFunctionObjectList();
  // Member Functions
    // Access
      //- Return const access to the cloud owner
      inline const regionModel& owner() const;
      //- Return refernce to the cloud owner
      inline regionModel& owner();
      //- Return the forces dictionary
      inline const dictionary& dict() const;
    // Evaluation
      //- Pre-evolve hook
      virtual void preEvolveRegion();
      //- Post-evolve hook
      virtual void postEvolveRegion();
};
}  // namespace regionModels
}  // namespace mousse
#include "region_model_function_object_list_i.hpp"
#endif
