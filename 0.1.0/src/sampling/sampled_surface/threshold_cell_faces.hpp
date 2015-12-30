// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thresholdCellFaces
// Description
//   Selects the mesh cell faces specified by a threshold value.
//   Non-triangulated by default.
// SourceFiles
//   threshold_cell_faces.cpp
#ifndef threshold_cell_faces_hpp_
#define threshold_cell_faces_hpp_
#include "meshed_surface.hpp"
namespace mousse
{
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
  TypeName("thresholdCellFaces");
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
