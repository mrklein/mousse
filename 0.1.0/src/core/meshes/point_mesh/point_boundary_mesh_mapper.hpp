// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointBoundaryMeshMapper
// Description
//   mousse::pointBoundaryMeshMapper

#ifndef point_boundary_mesh_mapper_hpp_
#define point_boundary_mesh_mapper_hpp_

#include "ptr_list.hpp"
#include "point_patch_mapper.hpp"

namespace mousse
{

class pointBoundaryMeshMapper
:
  public PtrList<pointPatchMapper>
{
public:

  // Constructors

    //- Construct from components
    pointBoundaryMeshMapper
    (
      const pointMesh& mesh,
      const pointMapper& pointMap,
      const mapPolyMesh& mpm
    )
    :
      PtrList<pointPatchMapper>{mesh.boundary().size()}
    {
      const pointBoundaryMesh& patches = mesh.boundary();
      FOR_ALL(patches, patchI)
      {
        set
        (
          patchI,
          new pointPatchMapper
          (
            patches[patchI],
            pointMap,
            mpm
          )
        );
      }
    }

    //- Disallow default bitwise copy construct
    pointBoundaryMeshMapper(const pointBoundaryMeshMapper&) = delete;

    //- Disallow default bitwise assignment
    pointBoundaryMeshMapper& operator=
    (
      const pointBoundaryMeshMapper&
    ) = delete;

};
}  // namespace mousse
#endif
