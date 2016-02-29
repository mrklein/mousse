#ifndef CORE_MESHES_POINT_MESH_POINT_MESH_HPP_
#define CORE_MESHES_POINT_MESH_POINT_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointMesh
// Description
//   Mesh representing a set of points created from polyMesh.


#include "geo_mesh.hpp"
#include "_mesh_object.hpp"
#include "poly_mesh.hpp"
#include "point_boundary_mesh.hpp"

namespace mousse
{

class pointMesh
:
  public MeshObject<polyMesh, UpdateableMeshObject, pointMesh>,
  public GeoMesh<polyMesh>
{
  // Permanent data

    //- Boundary mesh
    pointBoundaryMesh boundary_;

  // Private Member Functions

    //- Map all fields
    void mapFields(const mapPolyMesh&);

public:

  // Declare name of the class and its debug switch
  CLASS_NAME("pointMesh");

  typedef pointMesh Mesh;
  typedef pointBoundaryMesh BoundaryMesh;

  // Constructors

    //- Construct from polyMesh
    explicit pointMesh(const polyMesh& pMesh);

    //- Disallow default bitwise copy construct
    pointMesh(const pointMesh&) = delete;

    //- Disallow default bitwise assignment
    pointMesh& operator=(const pointMesh&) = delete;

  //- Destructor
  ~pointMesh();

  // Member Functions

    //- Return number of points
    label size() const
    {
      return size(*this);
    }

    //- Return number of points
    static label size(const Mesh& mesh)
    {
      return mesh.GeoMesh<polyMesh>::mesh_.nPoints();
    }

    //- Return reference to boundary mesh
    const pointBoundaryMesh& boundary() const
    {
      return boundary_;
    }

    //- Return parallel info
    const globalMeshData& globalData() const
    {
      return GeoMesh<polyMesh>::mesh_.globalData();
    }

    //- Return database. For now is its polyMesh.
    const objectRegistry& thisDb() const
    {
      return GeoMesh<polyMesh>::mesh_.thisDb();
    }

    // Mesh motion

      //- Move points
      bool movePoints();

      //- Update the mesh corresponding to given map
      void updateMesh(const mapPolyMesh& mpm);

  // Member Operators

    bool operator!=(const pointMesh& pm) const
    {
      return &pm != this;
    }

    bool operator==(const pointMesh& pm) const
    {
      return &pm == this;
    }

};

}  // namespace mousse

#endif
