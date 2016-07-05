#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELD_MAPPER_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELD_MAPPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvPatchFieldMapper
// Description
//   mousse::fvPatchFieldMapper

#include "primitive_fields.hpp"
#include "field_mapper.hpp"


namespace mousse {

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
