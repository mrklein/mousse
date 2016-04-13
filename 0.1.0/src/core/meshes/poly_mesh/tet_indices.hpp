#ifndef CORE_MESHES_POLY_MESH_TET_INDICES_HPP_
#define CORE_MESHES_POLY_MESH_TET_INDICES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetIndices
// Description
//   Storage and named access for the indices of a tet which is part of
//   the decomposition of a cell.
//   Tets are designated by
//   - cell (of course)
//   - face on cell
//   - three points on face (faceBasePt, facePtA, facePtB)
//    When constructing from a mesh and index in the face (tetPtI):
//     - faceBasePt is the mesh.tetBasePtIs() base point
//     - facePtA is tetPtI away from faceBasePt
//     - facePtB is next one after/before facePtA
//     e.g.:
//       +---+
//       |2 /|
//       | / |
//       |/ 1|  <- tetPt (so 1 for first triangle, 2 for second)
//       +---+
//       ^
//      faceBasePt

#include "label.hpp"
#include "tetrahedron.hpp"
#include "tri_point_ref.hpp"
#include "poly_mesh.hpp"
#include "tri_face.hpp"
#include "face.hpp"


namespace mousse {

class tetIndices
{
  // Private data
  //- Cell that this is a decomposed tet of
  label cellI_;
  //- Face that holds this decomposed tet
  label faceI_;
  //- Base point on the face
  label faceBasePtI_;
  //- Point on the face such that the right-hand circulation
  //      {faceBasePtI_, facePtAI_, facePtBI_}
  //  forms a triangle that points out of the tet
  label facePtAI_;
  //- Point on the face such that the right-hand circulation
  //      {faceBasePtI_, facePtAI_, facePtBI_}
  //  forms a triangle that points out of the tet
  label facePtBI_;
  //- Point on the face, *relative to the base point*, which
  //  characterises this tet on the face.
  label tetPtI_;
public:
  // Constructors
    //- Construct null
    tetIndices();
    //- Construct from components
    tetIndices
    (
      label cellI,
      label faceI,
      label faceBasePtI,
      label facePtAI,
      label facePtBI,
      label tetPtI
    );
    //- Construct from cell, face, pt description of tet
    tetIndices
    (
      label cellI,
      label faceI,
      label tetPtI,
      const polyMesh& mesh
    );
  //- Destructor
  ~tetIndices();
  // Member Functions
    // Access
      //- Return the cell
      inline label cell() const;
      //- Return the face
      inline label face() const;
      //- Return the face base point
      inline label faceBasePt() const;
      //- Return face point A
      inline label facePtA() const;
      //- Return face point B
      inline label facePtB() const;
      //- Return the characterising tetPtI
      inline label tetPt() const;
      //- Return the geometry corresponding to this tet from the
      //  supplied mesh
      inline tetPointRef tet(const polyMesh& mesh) const;
      //- Return the geometry corresponding to this tet from the
      //  supplied mesh using the old positions
      inline tetPointRef oldTet(const polyMesh& mesh) const;
      //- Return the geometry corresponding to the tri on the
      //  mesh face for this tet from the supplied mesh. Normal of
      //  the tri points out of the cell.
      inline triPointRef faceTri(const polyMesh& mesh) const;
      //- Return the point indices corresponding to the tri on the mesh
      //  face for this tet from the supplied mesh. Normal of
      //  the tri points out of the cell.
      inline triFace faceTriIs(const polyMesh& mesh) const;
      //- Return the geometry corresponding to the tri on the
      //  mesh face for this tet from the supplied mesh using
      //  the old position
      inline triPointRef oldFaceTri(const polyMesh& mesh) const;
    // Edit
      //- Return non-const access to the cell
      inline label& cell();
      //- Return non-const access to the face
      inline label& face();
      //- Return non-const access to the face base point
      inline label& faceBasePt();
      //- Return non-const access to face point A
      inline label& facePtA();
      //- Return non-const access to face point B
      inline label& facePtB();
      //- Return non-const access to the characterising tetPtI
      inline label& tetPt();
  // Member Operators
    inline bool operator==(const tetIndices&) const;
    inline bool operator!=(const tetIndices&) const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, tetIndices&);
    friend Ostream& operator<<(Ostream&, const tetIndices&);
};

}  // namespace mousse


// Member Functions 
mousse::label mousse::tetIndices::cell() const
{
  return cellI_;
}


mousse::label mousse::tetIndices::face() const
{
  return faceI_;
}


mousse::label mousse::tetIndices::faceBasePt() const
{
  return faceBasePtI_;
}


mousse::label mousse::tetIndices::facePtA() const
{
  return facePtAI_;
}


mousse::label mousse::tetIndices::facePtB() const
{
  return facePtBI_;
}


mousse::label mousse::tetIndices::tetPt() const
{
  return tetPtI_;
}


mousse::tetPointRef mousse::tetIndices::tet(const polyMesh& mesh) const
{
  const pointField& pPts = mesh.points();
  const faceList& pFaces = mesh.faces();
  const vectorField& pC = mesh.cellCentres();
  const mousse::face& f = pFaces[faceI_];
  return {pC[cellI_],
          pPts[f[faceBasePtI_]],
          pPts[f[facePtAI_]],
          pPts[f[facePtBI_]]};
}


mousse::tetPointRef mousse::tetIndices::oldTet(const polyMesh& mesh) const
{
  const pointField& oldPPts = mesh.oldPoints();
  const faceList& pFaces = mesh.faces();
  // We need to reconstruct the old Cc from oldPoints (it isn't
  // stored)
  point oldC = mesh.cells()[cellI_].centre
  (
    oldPPts,
    pFaces
  );
  const mousse::face& f = pFaces[faceI_];
  return {oldC,
          oldPPts[f[faceBasePtI_]],
          oldPPts[f[facePtAI_]],
          oldPPts[f[facePtBI_]]};
}


mousse::triPointRef mousse::tetIndices::faceTri(const polyMesh& mesh) const
{
  const pointField& pPts = mesh.points();
  const faceList& pFaces = mesh.faces();
  const mousse::face& f = pFaces[faceI_];
  return {pPts[f[faceBasePtI_]],
          pPts[f[facePtAI_]],
          pPts[f[facePtBI_]]};
}


mousse::triFace mousse::tetIndices::faceTriIs(const polyMesh& mesh) const
{
  const faceList& pFaces = mesh.faces();
  const mousse::face& f = pFaces[faceI_];
  return {f[faceBasePtI_],
          f[facePtAI_],
          f[facePtBI_]};
}


mousse::triPointRef mousse::tetIndices::oldFaceTri(const polyMesh& mesh) const
{
  const pointField& oldPPts = mesh.oldPoints();
  const faceList& pFaces = mesh.faces();
  const mousse::face& f = pFaces[faceI_];
  return {oldPPts[f[faceBasePtI_]],
          oldPPts[f[facePtAI_]],
          oldPPts[f[facePtBI_]]};
}


mousse::label& mousse::tetIndices::cell()
{
  return cellI_;
}


mousse::label& mousse::tetIndices::face()
{
  return faceI_;
}


mousse::label& mousse::tetIndices::faceBasePt()
{
  return faceBasePtI_;
}


mousse::label& mousse::tetIndices::facePtA()
{
  return facePtAI_;
}


mousse::label& mousse::tetIndices::facePtB()
{
  return facePtBI_;
}


mousse::label& mousse::tetIndices::tetPt()
{
  return tetPtI_;
}


// Member Operators 
inline bool mousse::tetIndices::operator==(const mousse::tetIndices& rhs) const
{
  return (cell() == rhs.cell()
          && face() == rhs.face()
          && faceBasePt() == rhs.faceBasePt()
          && facePtA() == rhs.facePtA()
          && facePtB() == rhs.facePtB()
          && tetPt() == rhs.tetPt());
}


inline bool mousse::tetIndices::operator!=(const mousse::tetIndices& rhs) const
{
  return !(*this == rhs);
}

#endif
