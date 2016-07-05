#ifndef CORE_MESHES_POINT_MESH_POINT_MAPPER_HPP_
#define CORE_MESHES_POINT_MESH_POINT_MAPPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointMapper
// Description
//   This object provides mapping and fill-in information for point data
//   between the two meshes after the topological change.  It is
//   constructed from mapPolyMesh.

#include "morph_field_mapper.hpp"


namespace mousse {

// Forward declaration of classes
class pointMesh;
class mapPolyMesh;
class polyMesh;


class pointMapper
:
  public morphFieldMapper
{
  // Private data

    //- Reference to pointMesh
    const pointMesh& pMesh_;

    //- Reference to mapPolyMesh
    const mapPolyMesh& mpm_;

    //- Are there any inserted (unmapped) points
    bool insertedPoints_;

    //- Is the mapping direct
    bool direct_;

  // Demand-driven private data

    //- Direct addressing (only one for of addressing is used)
    mutable labelList* directAddrPtr_;

    //- Interpolated addressing (only one for of addressing is used)
    mutable labelListList* interpolationAddrPtr_;

    //- Interpolation weights
    mutable scalarListList* weightsPtr_;

    //- Inserted points
    mutable labelList* insertedPointLabelsPtr_;

  // Private Member Functions

    //- Calculate addressing for mapping with inserted points
    void calcAddressing() const;

    //- Clear out local storage
    void clearOut();

public:

  // Constructors

    //- Construct from mapPolyMesh
    pointMapper(const pointMesh&, const mapPolyMesh& mpm);

    //- Disallow default bitwise copy construct
    pointMapper(const pointMapper&) = delete;

    //- Disallow default bitwise assignment
    pointMapper& operator=(const pointMapper&) = delete;

  //- Destructor
  virtual ~pointMapper();

  // Member Functions

    //- Return size
    virtual label size() const;

    //- Return size before mapping
    virtual label sizeBeforeMapping() const;

    //- Is the mapping direct
    virtual bool direct() const
    {
      return direct_;
    }

    //- Are there unmapped values? I.e. do all size() elements get
    //  get value
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

    //- Are there any inserted points
    bool insertedObjects() const
    {
      return insertedPoints_;
    }

    //- Return list of inserted points
    const labelList& insertedObjectLabels() const;

};

}  // namespace mousse
#endif
