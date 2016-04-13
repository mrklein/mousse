#ifndef CORE_MESHES_POLY_MESH_MAP_POLY_MESH_CELL_MAPPER_HPP_
#define CORE_MESHES_POLY_MESH_MAP_POLY_MESH_CELL_MAPPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellMapper
// Description
//   This object provides mapping and fill-in information for cell data
//   between the two meshes after the topological change.  It is
//   constructed from mapPolyMesh.

#include "morph_field_mapper.hpp"


namespace mousse {

// Forward declaration of classes
class polyMesh;
class mapPolyMesh;


class cellMapper
:
  public morphFieldMapper
{

  // Private data

    //- Reference to polyMesh
    const polyMesh& mesh_;

    //- Reference to mapPolyMesh
    const mapPolyMesh& mpm_;

    //- Are there any inserted (unmapped) cells
    bool insertedCells_;

    //- Is the mapping direct
    bool direct_;

  // Demand-driven private data

    //- Direct addressing (only one for of addressing is used)
    mutable labelList* directAddrPtr_;

    //- Interpolated addressing (only one for of addressing is used)
    mutable labelListList* interpolationAddrPtr_;

    //- Interpolation weights
    mutable scalarListList* weightsPtr_;

    //- Inserted cells
    mutable labelList* insertedCellLabelsPtr_;

  // Private Member Functions

    //- Calculate addressing for mapping with inserted cells
    void calcAddressing() const;

    //- Clear out local storage
    void clearOut();

public:

  // Constructors

    //- Construct from mapPolyMesh
    cellMapper(const mapPolyMesh& mpm);

    //- Disallow default bitwise copy construct
    cellMapper(const cellMapper&) = delete;

    //- Disallow default bitwise assignment
    cellMapper& operator=(const cellMapper&) = delete;

  //- Destructor
  virtual ~cellMapper();

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

    //- Are there any inserted cells
    virtual bool insertedObjects() const
    {
      return insertedCells_;
    }

    //- Return list of inserted cells
    const virtual labelList& insertedObjectLabels() const;

};

}  // namespace mousse
#endif
