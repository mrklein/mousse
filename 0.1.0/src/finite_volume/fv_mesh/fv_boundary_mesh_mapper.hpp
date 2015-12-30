// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvBoundaryMeshMapper
// Description
//   mousse::fvBoundaryMeshMapper
#ifndef fv_boundary_mesh_mapper_hpp_
#define fv_boundary_mesh_mapper_hpp_
#include "ptr_list.hpp"
#include "fv_patch_mapper.hpp"
namespace mousse
{
class fvBoundaryMeshMapper
:
  public PtrList<fvPatchMapper>
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    fvBoundaryMeshMapper(const fvBoundaryMeshMapper&);
    //- Disallow default bitwise assignment
    void operator=(const fvBoundaryMeshMapper&);
public:
  // Constructors
    //- Construct from components
    fvBoundaryMeshMapper
    (
      const fvMesh& mesh,
      const faceMapper& faceMap
    )
    :
      PtrList<fvPatchMapper>(mesh.boundary().size())
    {
      const fvBoundaryMesh& patches = mesh.boundary();
      forAll(patches, patchI)
      {
        set
        (
          patchI,
          new fvPatchMapper
          (
            patches[patchI],
            faceMap
          )
        );
      }
    }
};
}  // namespace mousse
#endif
