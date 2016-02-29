#ifndef UTILITIES_MESH_GENERATION_EXTRUDE_EXTRUDE_MESH_EXTRUDED_MESH_EXTRUDED_MESH_HPP_
#define UTILITIES_MESH_GENERATION_EXTRUDE_EXTRUDE_MESH_EXTRUDED_MESH_EXTRUDED_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudedMesh
// Description
// SourceFiles
//   extruded_mesh.cpp
//   extruded_mesh_templates.cpp
#include "poly_mesh.hpp"
#include "extrude_model.hpp"
namespace mousse
{
class extrudedMesh
:
  public polyMesh
{
  // Private data
    const extrudeModel& model_;
  // Private Member Functions
    //- Do edge and face use points in same order?
    static bool sameOrder(const face&, const edge&);
    //- Construct and return the extruded mesh points
    template<class Face, template<class> class FaceList, class PointField>
    Xfer<pointField> extrudedPoints
    (
      const PrimitivePatch<Face, FaceList, PointField>& extrudePatch,
      const extrudeModel&
    );
    //- Construct and return the extruded mesh faces
    template<class Face, template<class> class FaceList, class PointField>
    Xfer<faceList> extrudedFaces
    (
      const PrimitivePatch<Face, FaceList, PointField>& extrudePatch,
      const extrudeModel&
    );
    //- Construct and return the extruded mesh cells
    template<class Face, template<class> class FaceList, class PointField>
    Xfer<cellList> extrudedCells
    (
      const PrimitivePatch<Face, FaceList, PointField>& extrudePatch,
      const extrudeModel&
    );
    //- Disallow default bitwise copy construct
    extrudedMesh(const extrudedMesh&);
    //- Disallow default bitwise assignment
    void operator=(const extrudedMesh&);
public:
  // Constructors
    //- Construct from the primitivePatch to extrude
    template<class Face, template<class> class FaceList, class PointField>
    extrudedMesh
    (
      const IOobject&,
      const PrimitivePatch<Face, FaceList, PointField>& extrudePatch,
      const extrudeModel&
    );
};
}  // namespace mousse
#ifdef NoRepository
  #include "extruded_mesh_templates.cpp"
#endif
#endif
