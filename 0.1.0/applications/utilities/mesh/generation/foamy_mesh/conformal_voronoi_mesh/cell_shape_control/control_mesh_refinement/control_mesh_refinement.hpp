// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::controlMeshRefinement
// Description
// SourceFiles
//   control_mesh_refinement_i.hpp
//   control_mesh_refinement.cpp
//   control_mesh_refinement_io.cpp
#ifndef control_mesh_refinement_hpp_
#define control_mesh_refinement_hpp_
#include "cell_shape_control.hpp"
#include "cell_shape_control_mesh.hpp"
#include "cell_size_and_alignment_controls.hpp"
#include "conformation_surfaces.hpp"
#include "background_mesh_decomposition.hpp"
namespace mousse
{
class controlMeshRefinement
{
  // Private data
    const cellShapeControl& shapeController_;
    cellShapeControlMesh& mesh_;
    const cellSizeAndAlignmentControls& sizeControls_;
    const conformationSurfaces& geometryToConformTo_;
  // Private Member Functions
    scalar calcFirstDerivative
    (
      const mousse::point& a,
      const scalar& cellSizeA,
      const mousse::point& b,
      const scalar& cellSizeB
    ) const;
    scalar calcSecondDerivative
    (
      const mousse::point& a,
      const scalar& cellSizeA,
      const mousse::point& midPoint,
      const scalar& cellSizeMid,
      const mousse::point& b,
      const scalar& cellSizeB
    ) const
    {
      return (cellSizeA - 2*cellSizeMid + cellSizeB)/magSqr((a - b)/2);
    }
    bool detectEdge
    (
      const mousse::point& startPt,
      const mousse::point& endPt,
      pointHit& pointFound,
      const scalar tolSqr,
      const scalar secondDerivTolSqr
    ) const;
    pointHit findDiscontinuities(const linePointRef& l) const;
    //- Disallow default bitwise copy construct
    controlMeshRefinement(const controlMeshRefinement&);
    //- Disallow default bitwise assignment
    void operator=(const controlMeshRefinement&);
public:
    //- Runtime type information
    ClassName("controlMeshRefinement");
  // Constructors
    //- Construct null
    controlMeshRefinement(cellShapeControl& shapeController);
  //- Destructor
  ~controlMeshRefinement();
  // Member Functions
    // Edit
      void initialMeshPopulation
      (
        const autoPtr<backgroundMeshDecomposition>& decomposition
      );
      label refineMesh
      (
        const autoPtr<backgroundMeshDecomposition>& decomposition
      );
};
}  // namespace mousse
#endif
