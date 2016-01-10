// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetDecomposer
// Description
//   Decomposes polyMesh into tets.
// SourceFiles
//   tet_decomposer.cpp
#ifndef tet_decomposer_hpp_
#define tet_decomposer_hpp_
#include "dynamic_list.hpp"
#include "packed_bool_list.hpp"
#include "bool_list.hpp"
#include "type_info.hpp"
#include "named_enum.hpp"
namespace mousse
{
class polyMesh;
class polyTopoChange;
class face;
class mapPolyMesh;
class tetDecomposer
{
public:
  // Public data types
    enum decompositionType
    {
      FACE_CENTRE_TRIS, //- Faces decomposed into triangles
               //  using face-centre
      FACE_DIAG_TRIS    //- Faces decomposed into triangles diagonally
    };
    static const NamedEnum<decompositionType, 2> decompositionTypeNames;
private:
  // Private data
    const polyMesh& mesh_;
    //- From cell to tet point
    labelList cellToPoint_;
    //- From face to tet point
    labelList faceToPoint_;
    // Per face, per point (faceCentre) or triangle (faceDiag)
    // the added tet on the owner side
    labelListList faceOwnerCells_;
    // Per face, per point (faceCentre) or triangle (faceDiag)
    // the added tet on the neighbour side
    labelListList faceNeighbourCells_;
  // Private Member Functions
    //- Modify a face
    void modifyFace
    (
      polyTopoChange& meshMod,
      const face& f,
      const label faceI,
      const label own,
      const label nei,
      const label patchI,
      const label zoneI,
      const bool zoneFlip
    ) const;
    //- Add a face
    void addFace
    (
      polyTopoChange& meshMod,
      const face& f,
      const label own,
      const label nei,
      const label masterPointID,
      const label masterEdgeID,
      const label masterFaceID,
      const label patchI,
      const label zoneI,
      const bool zoneFlip
    ) const;
    //- Work out triangle index given the starting vertex in the face
    label triIndex(const label faceI, const label fp) const;
public:
  //- Runtime type information
  CLASS_NAME("tetDecomposer");
  // Constructors
    //- Construct from mesh
    tetDecomposer(const polyMesh&);
    //- Disallow default bitwise copy construct
    tetDecomposer(const tetDecomposer&) = delete;
    //- Disallow default bitwise assignment
    tetDecomposer& operator=(const tetDecomposer&) = delete;
  // Member Functions
    // Access
      //- From cell to tet point
      const labelList& cellToPoint() const
      {
        return cellToPoint_;
      }
      //- From face to tet point
      const labelList& faceToPoint() const
      {
        return faceToPoint_;
      }
      //- Per face, per point (faceCentre) or triangle (faceDiag)
      //  the added tet on the owner side
      const labelListList& faceOwnerCells() const
      {
        return faceOwnerCells_;
      }
      //- Per face, per point (faceCentre) or triangle (faceDiag)
      //  the added tet on the neighbour side
      const labelListList& faceNeighbourCells() const
      {
        return faceNeighbourCells_;
      }
    // Edit
      //- Insert all changes into meshMod to convert the polyMesh into
      //  tets.
      void setRefinement
      (
        const decompositionType decomposeType,
        polyTopoChange& meshMod
      );
      //- Force recalculation of locally stored data on topological change
      void updateMesh(const mapPolyMesh&);
};
}  // namespace mousse
#endif
