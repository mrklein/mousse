#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_ASPECT_RATIO_CONTROL_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_ASPECT_RATIO_CONTROL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellAspectRatioControl
// Description
// SourceFiles
//   cell_aspect_ratio_control.cpp
#include "dictionary.hpp"
#include "vector.hpp"
#include "scalar.hpp"
namespace mousse
{
class cellAspectRatioControl
{
  // Private data
    const dictionary aspectRatioDict_;
    const scalar aspectRatio_;
    vector aspectRatioDirection_;
public:
  // Constructors
    //- Construct from dictionary
    cellAspectRatioControl(const dictionary& motionDict);
    //- Disallow default bitwise copy construct
    cellAspectRatioControl(const cellAspectRatioControl&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const cellAspectRatioControl&) = delete;
  //- Destructor
  virtual ~cellAspectRatioControl();
  // Member Functions
    // Query
      void updateCellSizeAndFaceArea
      (
        vector& alignmentDir,
        scalar& targetFaceArea,
        scalar& targetCellSize
      ) const;
      void updateDeltaVector
      (
        const vector& alignmentDir,
        const scalar targetCellSize,
        const scalar rABMag,
        vector& delta
      ) const;
};
}  // namespace mousse
#endif
