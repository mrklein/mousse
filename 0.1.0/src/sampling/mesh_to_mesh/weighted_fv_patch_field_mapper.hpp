// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::weightedFvPatchFieldMapper
// Description
//   FieldMapper with weighted mapping.
#ifndef weighted_fv_patch_field_mapper_hpp_
#define weighted_fv_patch_field_mapper_hpp_
#include "fv_patch_field_mapper.hpp"
namespace mousse
{
class weightedFvPatchFieldMapper
:
  public fvPatchFieldMapper
{
  const labelListList& addressing_;
  const scalarListList& weights_;
  bool hasUnmapped_;
public:
  // Constructors
    //- Construct given addressing
    weightedFvPatchFieldMapper
    (
      const labelListList& addressing,
      const scalarListList& weights
    )
    :
      addressing_(addressing),
      weights_(weights),
      hasUnmapped_(false)
    {
      forAll(addressing_, i)
      {
        if (addressing_[i].size() == 0)
        {
          hasUnmapped_ = true;
        }
      }
    }
  //- Destructor
  virtual ~weightedFvPatchFieldMapper()
  {}
  // Member Functions
    virtual label size() const
    {
      return addressing().size();
    }
    virtual bool direct() const
    {
      return false;
    }
    virtual bool hasUnmapped() const
    {
      return hasUnmapped_;
    }
    virtual const labelListList& addressing() const
    {
      return addressing_;
    }
    virtual const scalarListList& weights() const
    {
      return weights_;
    }
};
}  // namespace mousse
#endif
