// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceCollapser
// Description
//   Collapses faces into edges. Used to remove sliver faces (faces with small
//   area but non-zero span).
//   Passed in as
//   - face label
//   - the two indices in the face (fpA, fpB) which delimit the vertices to be
//    kept.
//   Takes the vertices outside the range fpA..fpB and projects them onto the
//   kept edges (edges using kept vertices only).
//   Note:
//   - Use in combination with edge collapse to cleanup meshes.
//   - Can not remove cells so will mess up trying to remove a face on a tet.
//   - WIP. Should be combined with edge collapsing and cell collapsing into
//    proper 'collapser'.
//   - Caller is responsible for making sure kept vertices (fpA..fpB) for one
//    face are not the vertices to be removed for another face.
// SourceFiles
//   face_collapser.cpp
#ifndef face_collapser_hpp_
#define face_collapser_hpp_
#include "label_list.hpp"
#include "point.hpp"
#include "map.hpp"
#include "hash_set.hpp"
#include "type_info.hpp"
#include "edge_list.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class polyTopoChange;
class mapPolyMesh;
class faceCollapser
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
  // Static Functions
    //- Insert labelList into labelHashSet. Optional excluded element.
    static void insert
    (
      const labelList& elems,
      const label excludeElem,
      labelHashSet& set
    );
    //- Find edge amongst candidate edges.
    static label findEdge
    (
      const edgeList& edges,
      const labelList& edgeLabels,
      const label v0,
      const label v1
    );
  // Private Member Functions
    //- Replace vertices in face
    void filterFace
    (
      const Map<labelList>& splitEdges,
      const label faceI,
      polyTopoChange& meshMod
    ) const;
    //- Disallow default bitwise copy construct
    faceCollapser(const faceCollapser&);
    //- Disallow default bitwise assignment
    void operator=(const faceCollapser&);
public:
  //- Runtime type information
  ClassName("faceCollapser");
  // Constructors
    //- Construct from mesh.
    faceCollapser(const polyMesh& mesh);
  // Member Functions
    // Edit
      //- Collapse faces along endpoints. Play commands into
      //  polyTopoChange to create mesh.
      void setRefinement
      (
        const labelList& faceLabels,
        const labelList& fpA,
        const labelList& fpB,
        polyTopoChange&
      ) const;
      //- Update stored quantities for new mesh labels.
      void updateMesh(const mapPolyMesh&)
      {}
};
}  // namespace mousse
#endif
