// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyDualMesh
// Description
//   Creates dual of polyMesh.
//   Every polyMesh point becomes a cell on the dual mesh
//   Every polyMesh cell and patchFace becomes a point on the dual mesh.
// SourceFiles
//   poly_dual_mesh.cpp
#ifndef poly_dual_mesh_hpp_
#define poly_dual_mesh_hpp_
#include "poly_mesh.hpp"
#include "label_io_list.hpp"
#include "type_info.hpp"
namespace mousse
{
class polyDualMesh
:
  public polyMesh
{
  // Private data
    //- From polyMesh cell to my point
    labelIOList cellPoint_;
    //- From polyMesh boundary face (face-mesh.nInternalFaces()) to my point
    labelIOList boundaryFacePoint_;
  // Private Member Functions
    static labelList getFaceOrder
    (
      const labelList& faceOwner,
      const labelList& faceNeighbour,
      const cellList& cells,
      label& nInternalFaces
    );
    static void getPointEdges
    (
      const primitivePatch& patch,
      const label faceI,
      const label pointI,
      label& e0,
      label& e1
    );
    static labelList collectPatchSideFace
    (
      const polyPatch& patch,
      const label patchToDualOffset,
      const labelList& edgeToDualPoint,
      const labelList& pointToDualPoint,
      const label pointI,
      label& edgeI
    );
    static void collectPatchInternalFace
    (
      const polyPatch& patch,
      const label patchToDualOffset,
      const labelList& edgeToDualPoint,
      const label pointI,
      const label startEdgeI,
      labelList& dualFace2,
      labelList& featEdgeIndices2
    );
    static void splitFace
    (
      const polyPatch& patch,
      const labelList& pointToDualPoint,
      const label pointI,
      const labelList& dualFace,
      const labelList& featEdgeIndices,
      DynamicList<face>& dualFaces,
      DynamicList<label>& dualOwner,
      DynamicList<label>& dualNeighbour,
      DynamicList<label>& dualRegion
    );
    static void dualPatch
    (
      const polyPatch& patch,
      const label patchToDualOffset,
      const labelList& edgeToDualPoint,
      const labelList& pointToDualPoint,
      const pointField& dualPoints,
      DynamicList<face>& dualFaces,
      DynamicList<label>& dualOwner,
      DynamicList<label>& dualNeighbour,
      DynamicList<label>& dualRegion
    );
    void calcDual
    (
      const polyMesh& mesh,
      const labelList& featureEdges,
      const labelList& featurePoints
    );
    //- Disallow default bitwise copy construct
    polyDualMesh(const polyDualMesh&);
    //- Disallow default bitwise assignment
    void operator=(const polyDualMesh&);
public:
  //- Runtime type information
  CLASS_NAME("polyDualMesh");
  // Constructors
    //- Construct from IOobject
    polyDualMesh(const IOobject&);
    //- Construct from polyMesh and list of edges and points to represent.
    //  Feature edge and point labels are in local addressing of a patch
    //  over all boundary faces.
    polyDualMesh
    (
      const polyMesh&,
      const labelList& featureEdges,
      const labelList& featurePoints
    );
    //- Construct from polyMesh and feature edge angle. Uses calcFeatures
    //  below to determine feature edges and points.
    polyDualMesh
    (
      const polyMesh&,
      const scalar featureCos
    );
    //- Helper function to create feature edges and points based on
    //  feature angle and patches.
    static void calcFeatures
    (
      const polyMesh&,
      const scalar featureCos,
      labelList& featureEdges,
      labelList& featurePoints
    );
  //- Destructor
  ~polyDualMesh();
  // Member Functions
    // Access
      //- From polyMesh cell to dual point
      const labelIOList& cellPoint() const
      {
        return cellPoint_;
      }
      //- From polyMesh patch face to dual point
      const labelIOList& boundaryFacePoint() const
      {
        return boundaryFacePoint_;
      }
};
}  // namespace mousse
#endif
