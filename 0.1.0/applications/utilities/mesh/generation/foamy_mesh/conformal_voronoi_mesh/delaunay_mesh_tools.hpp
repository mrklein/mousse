// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DelaunayMeshTools
// Description
//   Collection of functions for operating on a Delaunay mesh. Includes:
//    - Functions for writing to an OBJ file
//    - Functions for extracting fields from the Delaunay triangulation
// SourceFiles
//   delaunay_mesh_tools.cpp
#ifndef delaunay_mesh_tools_hpp_
#define delaunay_mesh_tools_hpp_
#include "file_name.hpp"
#include "list.hpp"
#include "point.hpp"
#include "cgal_triangulation_3d_defs.hpp"
#include "indexed_vertex_enum.hpp"
namespace mousse
{
namespace DelaunayMeshTools
{
// OBJ writing
  //- Write list of points to file
  void writeOBJ(const fileName& fName, const List<mousse::point>& points);
  //- Write list of points to file
  void writeOBJ(const fileName& fName, const List<Vb>& points);
  //- Write an OBJ mesh consisting of points and faces
  void writeObjMesh
  (
    const fileName& fName,
    const pointField& points,
    const faceList& faces
  );
  //- Write Delaunay points in the range between (and including)
  //  type startPointType and endPointType to an OBJ file
  template<typename Triangulation>
  void writeOBJ
  (
    const fileName& fName,
    const Triangulation& t,
    const indexedVertexEnum::vertexType startPointType,
    const indexedVertexEnum::vertexType endPointType
  );
  //- Write Delaunay points of type pointType to .obj file
  template<typename Triangulation>
  void writeOBJ
  (
    const fileName& fName,
    const Triangulation& t,
    const indexedVertexEnum::vertexType pointType
  );
  //- Write the fixed Delaunay points to an OBJ file
  template<typename Triangulation>
  void writeFixedPoints(const fileName& fName, const Triangulation& t);
  //- Write the boundary Delaunay points to an OBJ file
  template<typename Triangulation>
  void writeBoundaryPoints(const fileName& fName, const Triangulation& t);
  //- Write the processor interface to an OBJ file
  template<typename Triangulation>
  void writeProcessorInterface
  (
    const fileName& fName,
    const Triangulation& t,
    const faceList& faces
  );
  //- Write the internal Delaunay vertices of the tessellation as a
  //  pointField that may be used to restart the meshing process
  template<typename Triangulation>
  void writeInternalDelaunayVertices
  (
    const fileName& instance,
    const Triangulation& t
  );
  //- Draws a tet cell to an output stream. The offset is supplied as the tet
  //  number to be drawn.
  template<typename CellHandle>
  void drawDelaunayCell(Ostream& os, const CellHandle& c, label offset = 0);
// Field extraction
  //- Extract all points in vertex-index order
  template<typename Triangulation>
  tmp<pointField> allPoints(const Triangulation& t);
}  // namespace DelaunayMeshTools
}  // namespace mousse
#ifdef NoRepository
  #include "delaunay_mesh_tools_templates.cpp"
#endif
#endif
