#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROLS_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROLS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellSizeAndAlignmentControls
// Description
// SourceFiles
//   cell_size_and_alignment_controls.cpp
#include "dictionary.hpp"
#include "cell_size_and_alignment_control.hpp"
namespace mousse
{
class cellSizeAndAlignmentControls
{
  // Private data
    const dictionary& shapeControlDict_;
    const conformationSurfaces& geometryToConformTo_;
    PtrList<cellSizeAndAlignmentControl> controlFunctions_;
    const scalar defaultCellSize_;
  // Private Member Functions
    bool evalCellSizeFunctions
    (
      const point& pt,
      scalar& minSize,
      label& maxPriority
    ) const;
public:
    //- Runtime type information
    CLASS_NAME("cellSizeAndAlignmentControls");
  // Constructors
    //- Construct from dictionary
    cellSizeAndAlignmentControls
    (
      const Time& runTime,
      const dictionary& shapeControlDict,
      const conformationSurfaces& geometryToConformTo,
      const scalar& defaultCellSize
    );
    //- Disallow default bitwise copy construct
    cellSizeAndAlignmentControls(const cellSizeAndAlignmentControls&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const cellSizeAndAlignmentControls&) = delete;
  //- Destructor
  virtual ~cellSizeAndAlignmentControls();
  // Member Functions
    // Access
      inline const PtrList<cellSizeAndAlignmentControl>&
      controlFunctions() const
      {
        return controlFunctions_;
      }
      inline const conformationSurfaces& geometryToConformTo() const
      {
        return geometryToConformTo_;
      }
    // Query
      scalar cellSize(const point& pt) const;
      scalar cellSize(const point& pt, label& maxPriority) const;
};
}  // namespace mousse
#endif
