#ifndef UTILITIES_MESH_MANIPULATION_POLY_DUAL_MESH_MESH_DUALISER_HPP_
#define UTILITIES_MESH_MANIPULATION_POLY_DUAL_MESH_MESH_DUALISER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshDualiser
// Description
//   Creates dual of polyMesh. Every point becomes a cell (or multiple cells
//   for feature points), a walk around every edge creates faces between them.
//   Put all points you want in the final mesh into featurePoints; all edge(mid)s
//   you want in the final mesh into featureEdges; all face(centre)s in
//   faceFaces.
//   Usually to preserve boundaries:
//     - all boundary faces are featureFaces
//     - all edges and points inbetween different patches are
//      featureEdges/points.
//   In same way you can also preserve internal faces (e.g. faceZones)

#include "dynamic_list.hpp"
#include "packed_bool_list.hpp"
#include "bool_list.hpp"
#include "type_info.hpp"


namespace mousse {

class polyMesh;
class polyTopoChange;


class meshDualiser
{
  // Private data
    const polyMesh& mesh_;
    //- From point on cell to dual cell. Either single entry or
    //  one entry per pointCells
    labelListList pointToDualCells_;
    //- From point to dual point (or -1 if not feature point).
    labelList pointToDualPoint_;
    //- From cell to dual point. All cells become point
    labelList cellToDualPoint_;
    //- From face to dual point (or -1 if not feature face)
    labelList faceToDualPoint_;
    //- From edge to dual point (or -1 if not feature edge)
    labelList edgeToDualPoint_;
  // Private Member Functions
    static void checkPolyTopoChange(const polyTopoChange&);
    static void dumpPolyTopoChange(const polyTopoChange&, const fileName&);
    //- Find dual cell given point and cell
    label findDualCell(const label cellI, const label pointI) const;
    //- Helper function to generate dualpoints on all boundary edges
    //  emanating from (boundary & feature) point
    void generateDualBoundaryEdges
    (
      const PackedBoolList&,
      const label pointI,
      polyTopoChange&
    );
    //- Check that owner and neighbour of face have same dual cell
    bool sameDualCell
    (
      const label faceI,
      const label pointI
    ) const;
    //- Add internal face
    label addInternalFace
    (
      const label masterPointI,
      const label masterEdgeI,
      const label masterFaceI,
      const bool edgeOrder,
      const label dualCell0,
      const label dualCell1,
      const DynamicList<label>& verts,
      polyTopoChange& meshMod
    ) const;
    //- Add boundary face
    label addBoundaryFace
    (
      const label masterPointI,
      const label masterEdgeI,
      const label masterFaceI,
      const label dualCellI,
      const label patchI,
      const DynamicList<label>& verts,
      polyTopoChange& meshMod
    ) const;
    //- Create internal faces walking around edge
    void createFacesAroundEdge
    (
      const bool splitFace,
      const PackedBoolList&,
      const label edgeI,
      const label startFaceI,
      polyTopoChange&,
      boolList& doneEFaces
    ) const;
    //- Create single internal face from internal face
    void createFaceFromInternalFace
    (
      const label faceI,
      label& fp,
      polyTopoChange&
    ) const;
    //- Creates boundary faces walking around point on patchI.
    void createFacesAroundBoundaryPoint
    (
      const label patchI,
      const label patchPointI,
      const label startFaceI,
      polyTopoChange&,
      boolList& donePFaces            // pFaces visited
    ) const;
public:
  //- Runtime type information
  CLASS_NAME("meshDualiser");
  // Constructors
    //- Construct from mesh
    meshDualiser(const polyMesh&);
    //- Disallow default bitwise copy construct
    meshDualiser(const meshDualiser&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const meshDualiser&) = delete;
  // Member Functions
    // Access
      //- From point on cell to dual cell. Either single entry or
      //  one entry per pointCells.
      const labelListList& pointToDualCells() const
      {
        return pointToDualCells_;
      }
      //- From point to dual point (or -1 if not feature point).
      const labelList& pointToDualPoint() const
      {
        return pointToDualPoint_;
      }
      //- From cell to dual point (at cell centre). All cells become
      //  points.
      const labelList& cellToDualPoint() const
      {
        return cellToDualPoint_;
      }
      //- From face to dual point (at face centre; or -1 if not
      //  feature face).
      const labelList& faceToDualPoint() const
      {
        return faceToDualPoint_;
      }
      //- From edge to dual point (at edge mid; or -1 if not feature
      //  edge).
      const labelList& edgeToDualPoint() const
      {
        return edgeToDualPoint_;
      }
    // Edit
      //- Insert all changes into meshMod to convert the polyMesh into
      //  its dual.
      //  featureFaces  : faces where we want a point at the face centre
      //  featureEdges  : edges           ,,                 edge mid
      //  featurePoints : points          ,,    point. Two variants:
      //      singleCellFeaturePoints : point becomes one dualcell.
      //          Use this for e.g. convex boundary points.
      //      multiCellFeaturePoints : one dualcell per original cell
      //          around point. Use this for e.g. concave boundary points
      //          since it prevents big concave boundary cells.
      void setRefinement
      (
        const bool splitFace,
        const labelList& featureFaces,
        const labelList& featureEdges,
        const labelList& singleCellFeaturePoints,
        const labelList& multiCellFeaturePoints,
        polyTopoChange& meshMod
      );
};

}  // namespace mousse

#endif

