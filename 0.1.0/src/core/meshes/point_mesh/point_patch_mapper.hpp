// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointPatchMapper
// Description
//   Mapping class for a pointPatchField.
// SourceFiles
//   point_patch_mapper.cpp

#ifndef point_patch_mapper_hpp_
#define point_patch_mapper_hpp_

#include "point_mapper.hpp"
#include "point_patch_field_mapper.hpp"
#include "point_patch.hpp"
#include "primitive_fields.hpp"

namespace mousse
{

// Forward declaration of classes
class pointPatch;
class mapPolyMesh;

class pointPatchMapper
:
  public pointPatchFieldMapper
{
  // Private data

    //- Reference to patch
    const pointPatch& patch_;

    //- Reference to point mapper for internal field
    const morphFieldMapper& pointMapper_;

    //- Reference to mapping data
    const mapPolyMesh& mpm_;

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

  // Constructors

    //- Construct from mappers
    pointPatchMapper
    (
      const pointPatch& patch,
      const pointMapper& pointMap,
      const mapPolyMesh& mpm
    );

    //- Disallow default bitwise copy construct
    pointPatchMapper
    (
      const pointPatchMapper&
    ) = delete;

    //- Disallow default bitwise assignment
    pointPatchMapper& operator=(const pointPatchMapper&) = delete;

  //- Destructor
  virtual ~pointPatchMapper();

  // Member Functions

    //- Return size
    virtual label size() const
    {
      return patch_.size();
    }

    virtual bool hasUnmapped() const
    {
      return hasUnmapped_;
    }

    //- Return size of field before mapping
    virtual label sizeBeforeMapping() const
    {
      return sizeBeforeMapping_;
    }

    //- Is the mapping direct
    virtual bool direct() const
    {
      return pointMapper_.direct();
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
