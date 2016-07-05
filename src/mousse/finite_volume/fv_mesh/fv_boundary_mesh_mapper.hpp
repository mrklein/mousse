#ifndef FINITE_VOLUME_FV_MESH_FV_BOUNDARY_MESH_MAPPER_HPP_
#define FINITE_VOLUME_FV_MESH_FV_BOUNDARY_MESH_MAPPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvBoundaryMeshMapper
// Description
//   mousse::fvBoundaryMeshMapper

#include "ptr_list.hpp"
#include "fv_patch_mapper.hpp"


namespace mousse {

class fvBoundaryMeshMapper
:
  public PtrList<fvPatchMapper>
{
public:
  // Constructors
    //- Construct from components
    fvBoundaryMeshMapper
    (
      const fvMesh& mesh,
      const faceMapper& faceMap
    )
    :
      PtrList<fvPatchMapper>{mesh.boundary().size()}
    {
      const fvBoundaryMesh& patches = mesh.boundary();
      FOR_ALL(patches, patchI) {
        set
        (
          patchI,
          new fvPatchMapper
          {
            patches[patchI],
            faceMap
          }
        );
      }
    }
    //- Disallow default bitwise copy construct
    fvBoundaryMeshMapper(const fvBoundaryMeshMapper&) = delete;
    //- Disallow default bitwise assignment
    fvBoundaryMeshMapper& operator=(const fvBoundaryMeshMapper&) = delete;
};
}  // namespace mousse
#endif
