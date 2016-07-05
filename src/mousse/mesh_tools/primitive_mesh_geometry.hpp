#ifndef MESH_TOOLS_PRIMITIVE_MESH_GEOMETRY_PRIMITIVE_MESH_GEOMETRY_HPP_
#define MESH_TOOLS_PRIMITIVE_MESH_GEOMETRY_PRIMITIVE_MESH_GEOMETRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::primitiveMeshGeometry
// Description
//   Updateable mesh geometry + checking routines.

#include "point_fields.hpp"
#include "hash_set.hpp"


namespace mousse {

class primitiveMeshGeometry
{
    //- Reference to primitiveMesh.
    const primitiveMesh& mesh_;
    //- Uptodate copy of face areas
    vectorField faceAreas_;
    //- Uptodate copy of face centres
    vectorField faceCentres_;
    //- Uptodate copy of cell centres
    vectorField cellCentres_;
    //- Uptodate copy of cell volumes
    scalarField cellVolumes_;
  // Private Member Functions
    //- Update face areas and centres on selected faces.
    void updateFaceCentresAndAreas
    (
      const pointField& p,
      const labelList& changedFaces
    );
    //- Update cell volumes and centres on selected cells. Requires
    //  cells and faces to be consistent set.
    void updateCellCentresAndVols
    (
      const labelList& changedCells,
      const labelList& changedFaces
    );
public:
  CLASS_NAME("primitiveMeshGeometry");
  // Constructors
    //- Construct from mesh
    primitiveMeshGeometry(const primitiveMesh&);
  // Member Functions
    // Access
      const primitiveMesh& mesh() const
      {
        return mesh_;
      }
      const vectorField& faceAreas() const
      {
        return faceAreas_;
      }
      const vectorField& faceCentres() const
      {
        return faceCentres_;
      }
      const vectorField& cellCentres() const
      {
        return cellCentres_;
      }
      const scalarField& cellVolumes() const
      {
        return cellVolumes_;
      }
    // Edit
      //- Take over properties from mesh
      void correct();
      //- Recalculate on selected faces. Recalculates cell properties
      //  on owner and neighbour of these cells.
      void correct
      (
        const pointField& p,
        const labelList& changedFaces
      );
      //- Helper function: get affected cells from faces
      labelList affectedCells(const labelList& changedFaces) const;
    // Checking of selected faces with supplied geometry (mesh only used for
    // topology). Parallel aware.
      static bool checkFaceDotProduct
      (
        const bool report,
        const scalar orthWarn,
        const primitiveMesh&,
        const vectorField& cellCentres,
        const vectorField& faceAreas,
        const labelList& checkFaces,
        labelHashSet* setPtr
      );
      static bool checkFacePyramids
      (
        const bool report,
        const scalar minPyrVol,
        const primitiveMesh&,
        const vectorField& cellCentres,
        const pointField& p,
        const labelList& checkFaces,
        labelHashSet*
      );
      static bool checkFaceSkewness
      (
        const bool report,
        const scalar internalSkew,
        const scalar boundarySkew,
        const primitiveMesh& mesh,
        const vectorField& cellCentres,
        const vectorField& faceCentres,
        const vectorField& faceAreas,
        const labelList& checkFaces,
        labelHashSet* setPtr
      );
      static bool checkFaceWeights
      (
        const bool report,
        const scalar warnWeight,
        const primitiveMesh& mesh,
        const vectorField& cellCentres,
        const vectorField& faceCentres,
        const vectorField& faceAreas,
        const labelList& checkFaces,
        labelHashSet* setPtr
      );
      static bool checkFaceAngles
      (
        const bool report,
        const scalar maxDeg,
        const primitiveMesh& mesh,
        const vectorField& faceAreas,
        const pointField& p,
        const labelList& checkFaces,
        labelHashSet* setPtr
      );
      static bool checkFaceTwist
      (
        const bool report,
        const scalar minTwist,
        const primitiveMesh&,
        const vectorField& faceAreas,
        const vectorField& faceCentres,
        const pointField& p,
        const labelList& checkFaces,
        labelHashSet* setPtr
      );
      static bool checkFaceArea
      (
        const bool report,
        const scalar minArea,
        const primitiveMesh&,
        const vectorField& faceAreas,
        const labelList& checkFaces,
        labelHashSet* setPtr
      );
      static bool checkCellDeterminant
      (
        const bool report,
        const scalar minDet,
        const primitiveMesh&,
        const vectorField& faceAreas,
        const labelList& checkFaces,
        const labelList& affectedCells,
        labelHashSet* setPtr
      );
    // Checking of selected faces with local geometry. Uses above static
    // functions. Parallel aware.
      bool checkFaceDotProduct
      (
        const bool report,
        const scalar orthWarn,
        const labelList& checkFaces,
        labelHashSet* setPtr
      ) const;
      bool checkFacePyramids
      (
        const bool report,
        const scalar minPyrVol,
        const pointField& p,
        const labelList& checkFaces,
        labelHashSet* setPtr
      ) const;
      bool checkFaceSkewness
      (
        const bool report,
        const scalar internalSkew,
        const scalar boundarySkew,
        const labelList& checkFaces,
        labelHashSet* setPtr
      ) const;
      bool checkFaceWeights
      (
        const bool report,
        const scalar warnWeight,
        const labelList& checkFaces,
        labelHashSet* setPtr
      ) const;
      bool checkFaceAngles
      (
        const bool report,
        const scalar maxDeg,
        const pointField& p,
        const labelList& checkFaces,
        labelHashSet* setPtr
      ) const;
      bool checkFaceTwist
      (
        const bool report,
        const scalar minTwist,
        const pointField& p,
        const labelList& checkFaces,
        labelHashSet* setPtr
      ) const;
      bool checkFaceArea
      (
        const bool report,
        const scalar minArea,
        const labelList& checkFaces,
        labelHashSet* setPtr
      ) const;
      bool checkCellDeterminant
      (
        const bool report,
        const scalar warnDet,
        const labelList& checkFaces,
        const labelList& affectedCells,
        labelHashSet* setPtr
      ) const;
};

}  // namespace mousse
#endif
