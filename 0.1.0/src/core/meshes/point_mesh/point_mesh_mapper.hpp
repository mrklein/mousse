#ifndef CORE_MESHES_POINT_MESH_POINT_MESH_MAPPER_HPP_
#define CORE_MESHES_POINT_MESH_POINT_MESH_MAPPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointMeshMapper
// Description
//   Class holds all the necessary information for mapping fields associated
//   with pointMesh.

#include "point_mapper.hpp"
#include "point_boundary_mesh_mapper.hpp"


namespace mousse {

// Forward declaration of classes
class pointMesh;
class mapPolyMesh;


class pointMeshMapper
{
  // Private data

    //- Reference to mesh
    const pointMesh& mesh_;

    //- Point mapper
    pointMapper pointMap_;

    //- Boundary mapper
    pointBoundaryMeshMapper boundaryMap_;

public:

  // Constructors

    //- Construct from pointMesh
    pointMeshMapper(const pointMesh& mesh, const mapPolyMesh& mpm)
    :
      mesh_{mesh},
      pointMap_{mesh, mpm},
      boundaryMap_{mesh, pointMap_, mpm}
    {}

    //- Disallow default bitwise copy construct
    pointMeshMapper(const pointMeshMapper&) = delete;

    //- Disallow default bitwise assignment
    void operator=(const pointMeshMapper&) = delete;

  // Member Functions

    //- Return reference to mesh fields belong to
    const pointMesh& mesh() const
    {
      return mesh_;
    }

    //- Return reference to objectRegistry storing fields. Can be
    //  removed once fields stored on pointMesh.
    const objectRegistry& thisDb() const
    {
      return mesh_();
    }

    //- Return point mapper
    const morphFieldMapper& pointMap() const
    {
      return pointMap_;
    }

    //- Return boundary mapper
    const pointBoundaryMeshMapper& boundaryMap() const
    {
      return boundaryMap_;
    }

};

}  // namespace mousse

#endif
