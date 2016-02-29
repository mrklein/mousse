#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_POINT_PATCH_FIELD_MAPPER_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_POINT_PATCH_FIELD_MAPPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointPatchFieldMapper
// Description
//   mousse::pointPatchFieldMapper
#include "primitive_fields.hpp"
#include "field_mapper.hpp"
namespace mousse
{
class pointPatchFieldMapper
:
  public FieldMapper
{
public:
  // Constructors
    //- Null constructor
    pointPatchFieldMapper()
    {}
};
}  // namespace mousse
#endif
