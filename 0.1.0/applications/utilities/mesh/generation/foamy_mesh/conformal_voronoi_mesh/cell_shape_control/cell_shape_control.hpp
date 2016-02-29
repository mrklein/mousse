#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_SHAPE_CONTROL_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_SHAPE_CONTROL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellShapeControl
// Description
// SourceFiles
//   cell_shape_control.cpp
#include "dictionary.hpp"
#include "auto_ptr.hpp"
#include "tensor.hpp"
#include "point.hpp"
#include "primitive_fields_fwd.hpp"
#include "point_field_fwd.hpp"
#include "time.hpp"
#include "searchable_surfaces.hpp"
#include "conformation_surfaces.hpp"
#include "cell_aspect_ratio_control.hpp"
#include "cell_size_and_alignment_controls.hpp"
#include "cell_shape_control_mesh.hpp"
#include "background_mesh_decomposition.hpp"
#include "cv_controls.hpp"
namespace mousse
{
class cellShapeControl
:
  public dictionary
{
  // Private data
    // const Time& runTime_;
    // const searchableSurfaces& allGeometry_;
    const conformationSurfaces& geometryToConformTo_;
    const scalar defaultCellSize_;
    const scalar minimumCellSize_;
    cellShapeControlMesh shapeControlMesh_;
    cellAspectRatioControl aspectRatio_;
    cellSizeAndAlignmentControls sizeAndAlignment_;
public:
  //- Runtime type information
  CLASS_NAME("cellShapeControl");
  // Constructors
    //- Construct from dictionary and references to conformalVoronoiMesh and
    //  searchableSurfaces
    cellShapeControl
    (
      const Time& runTime,
      const cvControls& foamyHexMeshControls,
      const searchableSurfaces& allGeometry,
      const conformationSurfaces& geometryToConformTo
    );
    //- Disallow default bitwise copy construct
    cellShapeControl(const cellShapeControl&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const cellShapeControl&) = delete;
  //- Destructor
  ~cellShapeControl();
  // Member Functions
    // Access
      inline const scalar& defaultCellSize() const;
      inline cellShapeControlMesh& shapeControlMesh();
      inline const cellShapeControlMesh& shapeControlMesh() const;
      inline const cellAspectRatioControl& aspectRatio() const;
      inline const cellSizeAndAlignmentControls& sizeAndAlignment() const;
      inline const scalar& minimumCellSize() const;
    // Query
      //- Return the cell size at the given location
      scalar cellSize(const point& pt) const;
      scalarField cellSize(const pointField& pts) const;
      //- Return the cell alignment at the given location
      tensor cellAlignment(const point& pt) const;
      void cellSizeAndAlignment
      (
        const point& pt,
        scalar& size,
        tensor& alignment
      ) const;
};
}  // namespace mousse

// Member Functions 
inline mousse::cellShapeControlMesh&
mousse::cellShapeControl::shapeControlMesh()
{
  return shapeControlMesh_;
}

inline const mousse::cellShapeControlMesh&
mousse::cellShapeControl::shapeControlMesh() const
{
  return shapeControlMesh_;
}

inline const mousse::scalar& mousse::cellShapeControl::defaultCellSize() const
{
  return defaultCellSize_;
}

inline const mousse::cellAspectRatioControl&
mousse::cellShapeControl::aspectRatio() const
{
  return aspectRatio_;
}

inline const mousse::cellSizeAndAlignmentControls&
mousse::cellShapeControl::sizeAndAlignment() const
{
  return sizeAndAlignment_;
}

inline const mousse::scalar& mousse::cellShapeControl::minimumCellSize() const
{
  return minimumCellSize_;
}
#endif
