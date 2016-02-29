#ifndef CORE_MESHES_POLY_MESH_MAP_POLY_MESH_MORPH_FIELD_MAPPER_HPP_
#define CORE_MESHES_POLY_MESH_MAP_POLY_MESH_MORPH_FIELD_MAPPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::morphFieldMapper
// Description
//   Abstract base class to hold the Field mapping for mesh morphs.

#include "field.hpp"
#include "map.hpp"
#include "field_mapper.hpp"

namespace mousse
{
class morphFieldMapper
:
  public FieldMapper
{
public:
  // Constructors
    //- Null constructor
    morphFieldMapper()
    {}
  //- Destructor
  virtual ~morphFieldMapper()
  {}
  // Member Functions
    //- Return size of field before mapping
    virtual label sizeBeforeMapping() const = 0;
    //- Are there any inserted objects
    virtual bool insertedObjects() const = 0;
    //- Return list of inserted objects
    virtual const labelList& insertedObjectLabels() const = 0;
};
}  // namespace mousse
#endif
