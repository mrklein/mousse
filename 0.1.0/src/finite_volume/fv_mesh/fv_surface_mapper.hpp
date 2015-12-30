// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvSurfaceMapper
// Description
//   FV surface mapper.
// SourceFiles
//   fv_surface_mapper.cpp
#ifndef fv_surface_mapper_hpp_
#define fv_surface_mapper_hpp_
#include "morph_field_mapper.hpp"
#include "fv_mesh.hpp"
#include "face_mapper.hpp"
#include "hash_set.hpp"
#include "map_poly_mesh.hpp"
namespace mousse
{
// Forward declaration of classes
class fvSurfaceMapper
:
  public morphFieldMapper
{
  // Private data
    //- Reference to mesh
    const fvMesh& mesh_;
    //- Reference to face mapper
    const faceMapper& faceMap_;
  // Demand-driven private data
    //- Direct addressing (only one for of addressing is used)
    mutable labelList* directAddrPtr_;
    //- Interpolated addressing (only one for of addressing is used)
    mutable labelListList* interpolationAddrPtr_;
    //- Interpolation weights
    mutable scalarListList* weightsPtr_;
    //- Inserted faces
    mutable labelList* insertedObjectLabelsPtr_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    fvSurfaceMapper(const fvSurfaceMapper&);
    //- Disallow default bitwise assignment
    void operator=(const fvSurfaceMapper&);
    //- Calculate addressing
    void calcAddressing() const;
    //- Clear out local storage
    void clearOut();
public:
  // Constructors
    //- Construct from components
    fvSurfaceMapper
    (
      const fvMesh& mesh,
      const faceMapper& fMapper
    );
  //- Destructor
  virtual ~fvSurfaceMapper();
  // Member Functions
    //- Return size
    virtual label size() const
    {
      return mesh_.nInternalFaces();
    }
    //- Return size of field before mapping
    virtual label sizeBeforeMapping() const
    {
      return faceMap_.internalSizeBeforeMapping();
    }
    //- Is the mapping direct
    virtual bool direct() const
    {
      return faceMap_.direct();
    }
    //- Has unmapped elements
    virtual bool hasUnmapped() const
    {
      return insertedObjects();
    }
    //- Return direct addressing
    virtual const labelUList& directAddressing() const;
    //- Return interpolated addressing
    virtual const labelListList& addressing() const;
    //- Return interpolaion weights
    virtual const scalarListList& weights() const;
    //- Are there any inserted faces
    virtual bool insertedObjects() const
    {
      return faceMap_.insertedObjects();
    }
    //- Return list of inserted faces
    virtual const labelList& insertedObjectLabels() const;
    //- Return flux flip map
    const labelHashSet& flipFaceFlux() const
    {
      return faceMap_.flipFaceFlux();
    }
};
}  // namespace mousse
#endif
