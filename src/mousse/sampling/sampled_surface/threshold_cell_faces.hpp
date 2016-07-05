#ifndef SAMPLING_SAMPLED_SURFACE_THRESHOLD_CELL_FACES_HPP_
#define SAMPLING_SAMPLED_SURFACE_THRESHOLD_CELL_FACES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thresholdCellFaces
// Description
//   Selects the mesh cell faces specified by a threshold value.
//   Non-triangulated by default.

#include "meshed_surface.hpp"


namespace mousse {

class polyMesh;
class thresholdCellFaces
:
  public MeshedSurface<face>
{
  //- Private typedefs for convenience
    typedef MeshedSurface<face> MeshStorage;
    //- Reference to mesh
    const polyMesh& mesh_;
    //- For every face the original cell in mesh
    labelList meshCells_;
  // Private Member Functions
    void calculate
    (
      const scalarField&,
      const scalar lowerThreshold,
      const scalar upperThreshold,
      const bool triangulate
    );
public:
  //- Runtime type information
  TYPE_NAME("thresholdCellFaces");
  // Constructors
    //- Construct from mesh, field and threshold value
    thresholdCellFaces
    (
      const polyMesh&,
      const scalarField&,
      const scalar lowerThreshold,
      const scalar upperThreshold,
      const bool triangulate = false
    );
  // Member Functions
    //- For every face original cell in mesh
    labelList& meshCells()
    {
      return meshCells_;
    }
    //- For every face original cell in mesh
    const labelList& meshCells() const
    {
      return meshCells_;
    }
};

}  // namespace mousse

#endif

