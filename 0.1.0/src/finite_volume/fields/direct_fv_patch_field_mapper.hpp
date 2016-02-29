#ifndef FINITE_VOLUME_FIELDS_DIRECT_FV_PATCH_FIELD_MAPPER_HPP_
#define FINITE_VOLUME_FIELDS_DIRECT_FV_PATCH_FIELD_MAPPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::directFvPatchFieldMapper
// Description
//   direct fvPatchFieldMapper
#include "fv_patch_field_mapper.hpp"
namespace mousse
{
class directFvPatchFieldMapper
:
  public fvPatchFieldMapper
{
  //- Addressing from new back to old
  const labelUList& directAddressing_;
  //- Does map contain any unmapped values
  bool hasUnmapped_;
public:
  // Constructors
    //- Construct given addressing
    directFvPatchFieldMapper(const labelUList& directAddressing)
    :
      directAddressing_(directAddressing),
      hasUnmapped_(false)
    {
      if (directAddressing_.size() && min(directAddressing_) < 0)
      {
        hasUnmapped_ = true;
      }
    }
  //- Destructor
  virtual ~directFvPatchFieldMapper()
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
