// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellAspectRatioControl
// Description
// SourceFiles
//   cell_aspect_ratio_control.cpp
#ifndef cell_aspect_ratio_control_hpp_
#define cell_aspect_ratio_control_hpp_
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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    cellAspectRatioControl(const cellAspectRatioControl&);
    //- Disallow default bitwise assignment
    void operator=(const cellAspectRatioControl&);
public:
  // Constructors
    //- Construct from dictionary
    cellAspectRatioControl(const dictionary& motionDict);
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
