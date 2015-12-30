// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudePatchMesh
// Description
//   Mesh at a patch created on the fly. The following entry should be used
//   on the field boundary dictionary:
//     // New Shell mesh data
//     extrudeModel    linearNormal;
//     linearNormalCoeffs
//     {
//       thickness       40e-6;
//     }
//     nLayers         50;
//     expansionRatio  1;
//     columnCells      true;
//     // Patch information
//     bottomCoeffs
//     {
//       name        "bottom";
//       type        mappedWall;
//       sampleMode  nearestPatchFace;
//       samplePatch fixedWalls;
//       offsetMode  uniform;
//       offset      (0 0 0);
//     }
//     topCoeffs
//     {
//       name        "top";
//       type        patch;
//     }
//     sideCoeffs
//     {
//       name        "side";
//       type        empty;
//     }
#ifndef extrude_patch_mesh_hpp_
#define extrude_patch_mesh_hpp_
#include "extrude_model.hpp"
#include "auto_ptr.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
namespace mousse
{
class extrudePatchMesh
:
  public fvMesh
{
private:
  // Private data
    //- Enumeration of patch IDs
    enum patchID
    {
      bottomPatchID,
      topPatchID,
      sidePatchID
    };
    //- Const reference to the patch from which this mesh is extruded
    const polyPatch& extrudedPatch_;
    //- Model dictionary
    dictionary dict_;
  // Private member functions
    //- Extrude mesh using polyPatches
    void extrudeMesh(const List<polyPatch*>& regionPatches);
public:
  //- Runtime type information
  TypeName("extrudePatchMesh");
  // Constructors
    //- Construct from mesh, patch and dictionary
    extrudePatchMesh
    (
      const fvMesh&,
      const fvPatch&,
      const dictionary&,
      const word
    );
    //- Construct from mesh, patch, dictionary and new mesh
    //  polyPatch information
    extrudePatchMesh
    (
      const fvMesh&,
      const fvPatch&,
      const dictionary&,
      const word,
      const List<polyPatch*>& polyPatches
    );
  //- Destructor
  virtual ~extrudePatchMesh();
  // Member functions
    // Access functions
      //- Return region mesh
      const fvMesh& regionMesh() const
      {
        return *this;
      }
      //- Return bottom patch
      const polyPatch& bottomPatch() const
      {
        return this->boundaryMesh()[bottomPatchID];
      }
      //- Return top patch
      const polyPatch& topPatch() const
      {
        return this->boundaryMesh()[topPatchID];
      }
      //- Return sides patch
      const polyPatch& sidesPatch() const
      {
        return this->boundaryMesh()[sidePatchID];
      }
      //- Return extruded patch
      const polyPatch& extrudedPatch() const
      {
        return extrudedPatch_;
      }
};
}  // namespace mousse
#endif
