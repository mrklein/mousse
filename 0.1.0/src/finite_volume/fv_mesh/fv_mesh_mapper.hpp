// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvMeshMapper
// Description
//   Class holds all the necessary information for mapping fields associated
//   with fvMesh.
// SourceFiles
//   fv_mesh_mapper.cpp
#ifndef fv_mesh_mapper_hpp_
#define fv_mesh_mapper_hpp_
#include "face_mapper.hpp"
#include "cell_mapper.hpp"
#include "fv_surface_mapper.hpp"
#include "fv_boundary_mesh_mapper.hpp"
namespace mousse
{
// Forward declaration of classes
class fvMesh;
class mapPolyMesh;
class fvMeshMapper
{
  // Private data
    //- Reference to mesh
    const fvMesh& mesh_;
    //- Face mapper
    faceMapper faceMap_;
    //- Cell mapper
    cellMapper cellMap_;
    //- Surface mapper (needs to be shortened for internal faces only)
    fvSurfaceMapper surfaceMap_;
    //- Boundary mapper
    fvBoundaryMeshMapper boundaryMap_;
public:
  // Constructors
    //- Construct from fvMesh
    fvMeshMapper(const fvMesh& mesh, const mapPolyMesh& mpm)
    :
      mesh_{mesh},
      faceMap_{mpm},
      cellMap_{mpm},
      surfaceMap_{mesh, faceMap_},
      boundaryMap_{mesh, faceMap_}
    {}
    //- Disallow default bitwise copy construct
    fvMeshMapper(const fvMeshMapper&) = delete;
    //- Disallow default bitwise assignment
    fvMeshMapper& operator=(const fvMeshMapper&) = delete;
  // Member Functions
    //- Return reference to mesh
    const fvMesh& mesh() const
    {
      return mesh_;
    }
    //- Return reference to objectRegistry storing fields. Can be
    //  removed once fields stored on pointMesh.
    const objectRegistry& thisDb() const
    {
      return mesh_;
    }
    //- Return volume mapper
    const morphFieldMapper& volMap() const
    {
      return cellMap_;
    }
    //- Return surface mapper
    const fvSurfaceMapper& surfaceMap() const
    {
      return surfaceMap_;
    }
    //- Return boundary mapper
    const fvBoundaryMeshMapper& boundaryMap() const
    {
      return boundaryMap_;
    }
};
}  // namespace mousse
#endif
