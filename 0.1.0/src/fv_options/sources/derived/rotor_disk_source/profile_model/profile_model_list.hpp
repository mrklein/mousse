#ifndef FV_OPTIONS_SOURCES_DERIVED_ROTOR_DISK_SOURCE_PROFILE_MODEL_PROFILE_MODEL_LIST_HPP_
#define FV_OPTIONS_SOURCES_DERIVED_ROTOR_DISK_SOURCE_PROFILE_MODEL_PROFILE_MODEL_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::profileModelList
// Description
//   Base class for profile models
// SourceFiles
//   profile_model_list.cpp
#include "ptr_list.hpp"
#include "profile_model.hpp"
namespace mousse
{
class profileModelList
:
  public PtrList<profileModel>
{
protected:
  // Protected data
    //- Dictionary
    const dictionary dict_;
public:
  //- Constructor
  profileModelList(const dictionary& dict, const bool readFields = true);
  //- Destructor
  ~profileModelList();
  // Member Functions
    //- Set blade->profile addressing
    void connectBlades
    (
      const List<word>& names,
      List<label>& addr
    ) const;
};
}  // namespace mousse
#endif
