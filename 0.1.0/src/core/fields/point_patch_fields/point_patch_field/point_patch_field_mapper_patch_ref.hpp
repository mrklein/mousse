#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_POINT_PATCH_FIELD_MAPPER_PATCH_REF_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_POINT_PATCH_FIELD_MAPPER_PATCH_REF_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointPatchFieldMapperPatchRef
// Description
//   mousse::pointPatchFieldMapperPatchRef


#include "point_patch_field_mapper.hpp"

namespace mousse
{

class pointPatchFieldMapperPatchRef
:
  public pointPatchFieldMapper
{
  // Private data
    const pointPatch& sourcePatch_;
    const pointPatch& targetPatch_;

public:

  // Constructors

    //- Construct from components
    pointPatchFieldMapperPatchRef
    (
      const pointPatch& source,
      const pointPatch& target
    )
    :
      sourcePatch_{source},
      targetPatch_{target}
    {}

    //- Disallow default bitwise copy construct
    pointPatchFieldMapperPatchRef
    (
      const pointPatchFieldMapperPatchRef&
    ) = delete;

    //- Disallow default bitwise assignment
    pointPatchFieldMapperPatchRef& operator=
    (
      const pointPatchFieldMapperPatchRef&
    ) = delete;

  // Member functions

    const pointPatch& sourcePatch() const
    {
      return sourcePatch_;
    }

    const pointPatch& targetPatch() const
    {
      return targetPatch_;
    }

};

}  // namespace mousse
#endif
