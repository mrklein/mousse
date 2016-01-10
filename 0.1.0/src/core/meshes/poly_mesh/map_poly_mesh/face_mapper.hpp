// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceMapper
// Description
//   This object provides mapping and fill-in information for face data
//   between the two meshes after the topological change.  It is
//   constructed from mapPolyMesh.
// SourceFiles
//   face_mapper.cpp

#ifndef face_mapper_hpp_
#define face_mapper_hpp_

#include "morph_field_mapper.hpp"
#include "hash_set.hpp"

namespace mousse
{

// Forward declaration of classes
class polyMesh;
class mapPolyMesh;

class faceMapper
:
  public morphFieldMapper
{
  // Private data

    //- Reference to polyMesh
    const polyMesh& mesh_;

    //- Reference to mapPolyMesh
    const mapPolyMesh& mpm_;

    //- Are there any inserted (unmapped) faces
    bool insertedFaces_;

    //- Is the mapping direct
    bool direct_;

  // Demand-driven private data

    //- Direct addressing (only one for of addressing is used)
    mutable labelList* directAddrPtr_;

    //- Interpolated addressing (only one for of addressing is used)
    mutable labelListList* interpolationAddrPtr_;

    //- Interpolation weights
    mutable scalarListList* weightsPtr_;

    //- Inserted faces
    mutable labelList* insertedFaceLabelsPtr_;

  // Private Member Functions

    //- Calculate addressing for mapping with inserted faces
    void calcAddressing() const;

    //- Clear out local storage
    void clearOut();

public:

  // Constructors

    //- Construct from mapPolyMesh
    faceMapper(const mapPolyMesh& mpm);

    //- Disallow default bitwise copy construct
    faceMapper(const faceMapper&) = delete;

    //- Disallow default bitwise assignment
    faceMapper& operator=(const faceMapper&) = delete;

  //- Destructor
  virtual ~faceMapper();

  // Member Functions

    //- Return size
    virtual label size() const;

    //- Return size of field before mapping
    virtual label sizeBeforeMapping() const;

    //- Return number of internal faces before mapping
    virtual label internalSizeBeforeMapping() const;

    //- Is the mapping direct
    virtual bool direct() const
    {
      return direct_;
    }

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

    //- Return flux flip map
    virtual const labelHashSet& flipFaceFlux() const;

    //- Return number of old internalFaces
    virtual label nOldInternalFaces() const;

    //- Return old patch starts
    virtual const labelList& oldPatchStarts() const;

    //- Return old patch sizes
    virtual const labelList& oldPatchSizes() const;

    //- Are there any inserted faces
    virtual bool insertedObjects() const
    {
      return insertedFaces_;
    }

    //- Return list of inserted faces
    virtual const labelList& insertedObjectLabels() const;

};

}  // namespace mousse

#endif
