#ifndef MESH_AUTO_MESH_AUTO_HEX_MESH_AUTO_HEX_MESH_DRIVER_REFINEMENT_PARAMETERS_REFINEMENT_PARAMETERS_HPP_
#define MESH_AUTO_MESH_AUTO_HEX_MESH_AUTO_HEX_MESH_DRIVER_REFINEMENT_PARAMETERS_REFINEMENT_PARAMETERS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::refinementParameters
// Description
//   Simple container to keep together refinement specific information.

#include "dictionary.hpp"
#include "point_field.hpp"
#include "switch.hpp"


namespace mousse {

// Class forward declarations
class polyMesh;


class refinementParameters
{
  // Private data
    //- Total number of cells
    const label maxGlobalCells_;
    //- Per processor max number of cells
    const label maxLocalCells_;
    //- When to stop refining
    const label minRefineCells_;
    //- Curvature
    scalar curvature_;
    //- Planarity criterion
    scalar planarAngle_;
    //- Number of layers between different refinement levels
    const label nBufferLayers_;
    //- Areas to keep
    const pointField keepPoints_;
    //- FaceZone faces allowed which have owner and neighbour in same
    //  cellZone?
    Switch allowFreeStandingZoneFaces_;
    //- Use old topology based problem-cell removal (cells with 8 points
    //  on surface)
    Switch useTopologicalSnapDetection_;
    //- Allowed load unbalance
    scalar maxLoadUnbalance_;
    Switch handleSnapProblems_;
public:
  // Constructors
    //- Construct from dictionary - new syntax
    refinementParameters(const dictionary& dict);
    //- Disallow default bitwise copy construct
    refinementParameters(const refinementParameters&) = delete;
    //- Disallow default bitwise assignment
    refinementParameters& operator=(const refinementParameters&) = delete;
  // Member Functions
    // Access
      //- Total number of cells
      label maxGlobalCells() const
      {
        return maxGlobalCells_;
      }
      //- Per processor max number of cells
      label maxLocalCells() const
      {
        return maxLocalCells_;
      }
      //- When to stop refining
      label minRefineCells() const
      {
        return minRefineCells_;
      }
      //- Curvature
      scalar curvature() const
      {
        return curvature_;
      }
      //- Angle when two intersections are considered to be planar
      scalar planarAngle() const
      {
        return planarAngle_;
      }
      //- Number of layers between different refinement levels
      label nBufferLayers() const
      {
        return nBufferLayers_;
      }
      //- Areas to keep
      const pointField& keepPoints() const
      {
        return keepPoints_;
      }
      //- Are zone faces allowed only inbetween different cell zones
      //  or also just free standing?
      bool allowFreeStandingZoneFaces() const
      {
        return allowFreeStandingZoneFaces_;
      }
      //- Use old topology based problem-cell removal
      //  (cells with 8 points on surface)
      bool useTopologicalSnapDetection() const
      {
        return useTopologicalSnapDetection_;
      }
      //- Allowed load unbalance
      scalar maxLoadUnbalance() const
      {
        return maxLoadUnbalance_;
      }
      bool handleSnapProblems() const
      {
        return handleSnapProblems_;
      }
    // Other
      //- Checks that cells are in mesh. Returns cells they are in.
      labelList findCells(const polyMesh&) const;
};

}  // namespace mousse

#endif

