// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshTriangulation
// Description
//   Triangulation of mesh faces. Generates (multiply connected) trisurface.
//   All patch faces keep their patchID as triangle region.
//   Internal faces get the supplied region number.
// SourceFiles
//   mesh_triangulation.cpp
#ifndef mesh_triangulation_hpp_
#define mesh_triangulation_hpp_
#include "tri_surface.hpp"
#include "type_info.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class primitiveMesh;
class meshTriangulation
:
  public triSurface
{
  // Private data
    //- Number of triangles in this that are internal to the surface.
    label nInternalFaces_;
    //- From triangle to mesh face
    labelList faceMap_;
  // Private Member Functions
    //- Is face internal to the subset.
    static bool isInternalFace
    (
      const primitiveMesh&,
      const boolList& includedCell,
      const label faceI
    );
    //- Find boundary faces of subset.
    static void getFaces
    (
      const primitiveMesh&,
      const boolList& includedCell,
      boolList& faceIsCut,
      label& nFaces,
      label& nInternalFaces
    );
    //- Add triangulation of face to triangles. Optionally reverse.
    void insertTriangles
    (
      const triFaceList&,
      const label faceI,
      const label regionI,
      const bool reverse,
      List<labelledTri>& triangles,
      label& triI
    );
public:
  CLASS_NAME("meshTriangulation");
  // Constructors
    //- Construct null
    meshTriangulation();
    //- Construct from selected mesh cell and region number to be used
    //  for triangles resulting from internal faces. (all boundary triangles
    //  get polyMesh patch id).
    //  faceCentreDecomposition = true  : decomposition around face centre
    //                            false : decomposition using
    //                                    existing vertices
    meshTriangulation
    (
      const polyMesh&,
      const label internalFacesPatch,
      const boolList& includedCell,
      const bool faceCentreDecomposition = false
    );
  // Member Functions
    //- Number of triangles in *this which are internal to the surface
    label nInternalFaces() const
    {
      return nInternalFaces_;
    }
    //- From triangle to mesh face
    const labelList& faceMap() const
    {
      return faceMap_;
    }
};
}  // namespace mousse
#endif
