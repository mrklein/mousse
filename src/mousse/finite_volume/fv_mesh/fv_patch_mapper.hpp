#ifndef FINITE_VOLUME_FV_MESH_FV_PATCH_MAPPER_HPP_
#define FINITE_VOLUME_FV_MESH_FV_PATCH_MAPPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvPatchMapper
// Description
//   Mapping class for a fvPatchField.

#include "fv_patch_field_mapper.hpp"
#include "face_mapper.hpp"
#include "fv_patch.hpp"
#include "primitive_fields.hpp"


namespace mousse {

// Forward declaration of classes
class fvPatch;
class mapPolyMesh;


class fvPatchMapper
:
  public fvPatchFieldMapper
{
  // Private data
    //- Reference to patch
    const fvPatch& patch_;
    //- Reference to face mapper
    const faceMapper& faceMap_;
    //- Size before mapping
    const label sizeBeforeMapping_;
  // Demand-driven private data
    mutable bool hasUnmapped_;
    //- Direct addressing (only one for of addressing is used)
    mutable labelList* directAddrPtr_;
    //- Interpolated addressing (only one for of addressing is used)
    mutable labelListList* interpolationAddrPtr_;
    //- Interpolation weights
    mutable scalarListList* weightsPtr_;
  // Private Member Functions
    //- Calculate addressing for mapping with inserted cells
    void calcAddressing() const;
    //- Clear out local storage
    void clearOut();
public:
  // Static data members
  // Constructors
    //- Construct from mappers
    fvPatchMapper
    (
      const fvPatch& patch,
      const faceMapper& faceMap
    );
    //- Disallow default bitwise copy construct
    fvPatchMapper(const fvPatchMapper&) = delete;
    //- Disallow default bitwise assignment
    fvPatchMapper& operator=(const fvPatchMapper&) = delete;
  //- Destructor
  virtual ~fvPatchMapper();
  // Member Functions
    //- Return size
    virtual label size() const
    {
      return patch_.size();
    }
    //- Return size of field before mapping
    virtual label sizeBeforeMapping() const
    {
      return sizeBeforeMapping_;
    }
    //- Is the mapping direct
    virtual bool direct() const
    {
      return faceMap_.direct();
    }
    virtual bool hasUnmapped() const
    {
      return hasUnmapped_;
    }
    //- Return direct addressing
    virtual const labelUList& directAddressing() const;
    //- Return interpolated addressing
    virtual const labelListList& addressing() const;
    //- Return interpolaion weights
    virtual const scalarListList& weights() const;
};
}  // namespace mousse
#endif
