#ifndef MESH_TOOLS_ALGORITHMS_MESH_WAVE_MESH_WAVE_HPP_
#define MESH_TOOLS_ALGORITHMS_MESH_WAVE_MESH_WAVE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MeshWave
// Description
//   FaceCellWave plus data
// SourceFiles
//   mesh_wave.cpp
#include "face_cell_wave.hpp"
namespace mousse
{
TEMPLATE_NAME(MeshWave);
template<class Type, class TrackingData = int>
class MeshWave
:
  public MeshWaveName
{
  // Private data
    //- Wall information for all faces
    List<Type> allFaceInfo_;
    //- Wall information for all cells
    List<Type> allCellInfo_;
    //- Wave calculation engine.
    FaceCellWave<Type, TrackingData> calc_;
   // Private static data
      //- Used as default trackdata value to satisfy default template
      //  argument.
      static int dummyTrackData_;
public:
  // Constructors
    //- Construct from mesh and list of changed faces with the Type
    //  for these faces. Iterates until nothing changes or maxIter reached.
    //  (maxIter can be 0)
    MeshWave
    (
      const polyMesh& mesh,
      const labelList& initialChangedFaces,
      const List<Type>& changedFacesInfo,
      const label maxIter,
      TrackingData& td = dummyTrackData_
    );
    //- Construct from mesh, list of changed faces with the Type
    //  for these faces and initial field.
    //  Iterates until nothing changes or maxIter reached.
    //  (maxIter can be 0)
    MeshWave
    (
      const polyMesh& mesh,
      const labelList& initialChangedFaces,
      const List<Type>& changedFacesInfo,
      const List<Type>& allCellInfo,
      const label maxIter,
      TrackingData& td = dummyTrackData_
    );
    //- Disallow default bitwise copy construct
    MeshWave(const MeshWave&) = delete;
    //- Disallow default bitwise assignment
    MeshWave& operator=(const MeshWave&) = delete;
  // Member Functions
    //- Get allFaceInfo
    const List<Type>& allFaceInfo() const
    {
      return allFaceInfo_;
    }
    //- Get allCellInfo
    const List<Type>& allCellInfo() const
    {
      return allCellInfo_;
    }
    //- Additional data to be passed into container
    const TrackingData& data() const
    {
      return calc_.data();
    }
    //- Iterate until no changes or maxIter reached. Returns actual
    //  number of iterations.
    label iterate(const label maxIter)
    {
      return calc_.iterate(maxIter);
    }
    //- Get number of unvisited cells, i.e. cells that were not (yet)
    //  reached from walking across mesh. This can happen from
    //  - not enough iterations done
    //  - a disconnected mesh
    //  - a mesh without walls in it
    label getUnsetCells() const
    {
      return calc_.getUnsetCells();
    }
    //- Get number of unvisited faces
    label getUnsetFaces() const
    {
      return calc_.getUnsetFaces();
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "mesh_wave.cpp"
#endif
#endif
