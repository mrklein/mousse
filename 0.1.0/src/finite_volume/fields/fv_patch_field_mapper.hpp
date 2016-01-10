// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvPatchFieldMapper
// Description
//   mousse::fvPatchFieldMapper

#ifndef fv_patch_field_mapper_hpp_
#define fv_patch_field_mapper_hpp_

#include "primitive_fields.hpp"
#include "field_mapper.hpp"

namespace mousse
{
class fvPatchFieldMapper
:
  public FieldMapper
{
public:
  // Constructors
    //- Null constructor
    fvPatchFieldMapper()
    {}
};
}  // namespace mousse

#endif
