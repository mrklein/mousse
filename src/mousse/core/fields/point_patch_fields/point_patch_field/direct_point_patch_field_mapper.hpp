#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_DIRECT_POINT_PATCH_FIELD_MAPPER_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_DIRECT_POINT_PATCH_FIELD_MAPPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::directPointPatchFieldMapper
// Description
//   direct pointPatchFieldMapper

#include "point_patch_field_mapper.hpp"


namespace mousse {

class directPointPatchFieldMapper
:
  public pointPatchFieldMapper
{
  //- Addressing from new back to old
  const labelUList& directAddressing_;
  //- Does map contain any unmapped values
  bool hasUnmapped_;
public:
  // Constructors
    //- Construct given addressing
    directPointPatchFieldMapper(const labelUList& directAddressing)
    :
      directAddressing_{directAddressing},
      hasUnmapped_{false}
    {
      if (directAddressing_.size() && min(directAddressing_) < 0) {
        hasUnmapped_ = true;
      }
    }
  //- Destructor
  virtual ~directPointPatchFieldMapper()
  {}
  // Member Functions
    label size() const
    {
      return directAddressing_.size();
    }
    bool direct() const
    {
      return true;
    }
    bool hasUnmapped() const
    {
      return hasUnmapped_;
    }
    const labelUList& directAddressing() const
    {
      return directAddressing_;
    }
};

}  // namespace mousse

#endif
