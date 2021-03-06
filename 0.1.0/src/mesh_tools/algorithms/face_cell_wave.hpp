#ifndef MESH_TOOLS_ALGORITHMS_MESH_WAVE_FACE_CELL_WAVE_HPP_
#define MESH_TOOLS_ALGORITHMS_MESH_WAVE_FACE_CELL_WAVE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FaceCellWave
// Description
//   Wave propagation of information through grid. Every iteration
//   information goes through one layer of cells. Templated on information
//   that is transferred.
//   Handles parallel and cyclics and non-parallel cyclics.
//   Note: whether to propagate depends on the return value of Type::update
//   which returns true (i.e. propagate) if the value changes by more than a
//   certain tolerance.
//   This tolerance can be very strict for normal face-cell and parallel
//   cyclics (we use a value of 0.01 just to limit propagation of small changes)
//   but for non-parallel cyclics this tolerance can be critical and if chosen
//   too small can lead to non-convergence.

#include "bool_list.hpp"
#include "label_list.hpp"
#include "primitive_fields_fwd.hpp"
#include "class_name.hpp"


namespace mousse {

// Forward declaration of classes
class polyMesh;
class polyPatch;
TEMPLATE_NAME(FaceCellWave);


template<class Type, class TrackingData = int>
class FaceCellWave
:
  public FaceCellWaveName
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
    //- Information for all faces
    UList<Type>& allFaceInfo_;
    //- Information for all cells
    UList<Type>& allCellInfo_;
    //- Additional data to be passed into container
    TrackingData& td_;
    //- Has face changed
    boolList changedFace_;
    //- List of changed faces
    labelList changedFaces_;
    //- Number of changed faces
    label nChangedFaces_;
    // Cells that have changed
    boolList changedCell_;
    labelList changedCells_;
    label nChangedCells_;
    //- Contains cyclics
    const bool hasCyclicPatches_;
    //- Contains cyclicAMI
    const bool hasCyclicAMIPatches_;
    //- Number of evaluations
    label nEvals_;
    //- Number of unvisited cells/faces
    label nUnvisitedCells_;
    label nUnvisitedFaces_;
  // Private Member Functions
    //- Updates cellInfo with information from neighbour. Updates all
    //  statistics.
    bool updateCell
    (
      const label cellI,
      const label neighbourFaceI,
      const Type& neighbourInfo,
      const scalar tol,
      Type& cellInfo
    );
    //- Updates faceInfo with information from neighbour. Updates all
    //  statistics.
    bool updateFace
    (
      const label faceI,
      const label neighbourCellI,
      const Type& neighbourInfo,
      const scalar tol,
      Type& faceInfo
    );
    //- Updates faceInfo with information from same face. Updates all
    //  statistics.
    bool updateFace
    (
      const label faceI,
      const Type& neighbourInfo,
      const scalar tol,
      Type& faceInfo
    );
    // Parallel, cyclic
      //- Debugging: check info on both sides of cyclic
      void checkCyclic(const polyPatch& pPatch) const;
      //- Has cyclic patch?
      template<class PatchType>
      bool hasPatch() const;
      //- Merge received patch data into global data
      void mergeFaceInfo
      (
        const polyPatch& patch,
        const label nFaces,
        const labelList&,
        const List<Type>&
      );
      //- Extract info for single patch only
      label getChangedPatchFaces
      (
        const polyPatch& patch,
        const label startFaceI,
        const label nFaces,
        labelList& changedPatchFaces,
        List<Type>& changedPatchFacesInfo
      ) const;
      //- Handle leaving domain. Implementation referred to Type
      void leaveDomain
      (
        const polyPatch& patch,
        const label nFaces,
        const labelList& faceLabels,
        List<Type>& faceInfo
      ) const;
      //- Handle leaving domain. Implementation referred to Type
      void enterDomain
      (
        const polyPatch& patch,
        const label nFaces,
        const labelList& faceLabels,
        List<Type>& faceInfo
      ) const;
      //- Offset face labels by constant value
      static void offset
      (
        const polyPatch& patch,
        const label off,
        const label nFaces,
        labelList& faces
      );
      //- Apply transformation to Type
      void transform
      (
        const tensorField& rotTensor,
        const label nFaces,
        List<Type>& faceInfo
      );
      //- Merge data from across processor boundaries
      void handleProcPatches();
      //- Merge data from across cyclics
      void handleCyclicPatches();
      //- Merge data from across AMI cyclics
      void handleAMICyclicPatches();
   // Private static data
      static const scalar geomTol_;
      static scalar propagationTol_;
      //- Used as default trackdata value to satisfy default template
      //  argument.
      static int dummyTrackData_;
public:
  // Static Functions
    //- Access to tolerance
    static scalar propagationTol()
    {
      return propagationTol_;
    }
    //- Change tolerance
    static void setPropagationTol(const scalar tol)
    {
      propagationTol_ = tol;
    }
  // Constructors
    // Construct from mesh. Use setFaceInfo and iterate() to do actual
    // calculation.
    FaceCellWave
    (
      const polyMesh&,
      UList<Type>& allFaceInfo,
      UList<Type>& allCellInfo,
      TrackingData& td = dummyTrackData_
    );
    //- Construct from mesh and list of changed faces with the Type
    //  for these faces. Iterates until nothing changes or maxIter reached.
    //  (maxIter can be 0)
    FaceCellWave
    (
      const polyMesh&,
      const labelList& initialChangedFaces,
      const List<Type>& changedFacesInfo,
      UList<Type>& allFaceInfo,
      UList<Type>& allCellInfo,
      const label maxIter,
      TrackingData& td = dummyTrackData_
    );
    //- Disallow default bitwise copy construct
    FaceCellWave(const FaceCellWave&) = delete;
    //- Disallow default bitwise assignment
    FaceCellWave& operator=(const FaceCellWave&) = delete;
  // Member Functions
    // Access
      //- Access allFaceInfo
      UList<Type>& allFaceInfo()
      {
        return allFaceInfo_;
      }
      //- Access allCellInfo
      UList<Type>& allCellInfo()
      {
        return allCellInfo_;
      }
      //- Additional data to be passed into container
      const TrackingData& data() const
      {
        return td_;
      }
      //- Access mesh
      const polyMesh& mesh() const
      {
        return mesh_;
      }
      //- Get number of unvisited cells, i.e. cells that were not (yet)
      //  reached from walking across mesh. This can happen from
      //  - not enough iterations done
      //  - a disconnected mesh
      //  - a mesh without walls in it
      label getUnsetCells() const;
      //- Get number of unvisited faces
      label getUnsetFaces() const;
    // Edit
      //- Set initial changed faces
      void setFaceInfo
      (
        const labelList& changedFaces,
        const List<Type>& changedFacesInfo
      );
      //- Propagate from face to cell. Returns total number of cells
      //  (over all processors) changed.
      label faceToCell();
      //- Propagate from cell to face. Returns total number of faces
      //  (over all processors) changed. (Faces on processorpatches are
      //  counted double)
      label cellToFace();
      //- Iterate until no changes or maxIter reached.  Returns actual
      //  number of iterations.
      label iterate(const label maxIter);
};
}  // namespace mousse

#include "face_cell_wave.ipp"

#endif
