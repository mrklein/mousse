// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellSizeAndAlignmentControls
// Description
// SourceFiles
//   cell_size_and_alignment_controls.cpp
#ifndef cell_size_and_alignment_controls_hpp_
#define cell_size_and_alignment_controls_hpp_
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
    //- Disallow default bitwise copy construct
    cellSizeAndAlignmentControls(const cellSizeAndAlignmentControls&);
    //- Disallow default bitwise assignment
    void operator=(const cellSizeAndAlignmentControls&);
public:
    //- Runtime type information
    ClassName("cellSizeAndAlignmentControls");
  // Constructors
    //- Construct from dictionary
    cellSizeAndAlignmentControls
    (
      const Time& runTime,
      const dictionary& shapeControlDict,
      const conformationSurfaces& geometryToConformTo,
      const scalar& defaultCellSize
    );
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
