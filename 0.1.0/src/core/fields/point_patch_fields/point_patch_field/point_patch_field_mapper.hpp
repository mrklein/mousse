// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointPatchFieldMapper
// Description
//   mousse::pointPatchFieldMapper
#ifndef point_patch_field_mapper_hpp_
#define point_patch_field_mapper_hpp_
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
