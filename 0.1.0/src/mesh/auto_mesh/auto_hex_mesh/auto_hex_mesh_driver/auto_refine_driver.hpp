// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::autoRefineDriver
// Description
// SourceFiles
//   auto_refine_driver.cpp
#ifndef auto_refine_driver_hpp_
#define auto_refine_driver_hpp_
#include "tree_bound_box.hpp"
namespace mousse
{
// Forward declaration of classes
class refinementParameters;
class snapParameters;
class meshRefinement;
class decompositionMethod;
class fvMeshDistribute;
class autoRefineDriver
{
  // Private data
    //- Mesh+surface
    meshRefinement& meshRefiner_;
    //- Reference to decomposition method
    decompositionMethod& decomposer_;
    //- Reference to mesh distribution engine
    fvMeshDistribute& distributor_;
    //- From surface region to patch
    const labelList globalToMasterPatch_;
    //- From surface region to patch
    const labelList globalToSlavePatch_;
  // Private Member Functions
    //- Refine all cells pierced by explicit feature edges
    label featureEdgeRefine
    (
      const refinementParameters& refineParams,
      const label maxIter,
      const label minRefine
    );
    //- Refine all cells interacting with the surface
    label surfaceOnlyRefine
    (
      const refinementParameters& refineParams,
      const label maxIter
    );
    //- Refine all cells in small gaps
    label gapOnlyRefine
    (
      const refinementParameters& refineParams,
      const label maxIter
    );
    //- Refine cells with almost all sides refined
    label danglingCellRefine
    (
      const refinementParameters& refineParams,
      const label nFaces,
      const label maxIter
    );
    //- Remove all cells within intersected region
    void removeInsideCells
    (
      const refinementParameters& refineParams,
      const label nBufferLayers
    );
    //- Remove all cells inside/outside shell
    label shellRefine
    (
      const refinementParameters& refineParams,
      const label maxIter
    );
    //- Add baffles and remove unreachable cells
    void baffleAndSplitMesh
    (
      const refinementParameters& refineParams,
      const snapParameters& snapParams,
      const bool handleSnapProblems,
      const dictionary& motionDict
    );
    //- Add zones
    void zonify(const refinementParameters& refineParams);
    void splitAndMergeBaffles
    (
      const refinementParameters& refineParams,
      const snapParameters& snapParams,
      const bool handleSnapProblems,
      const dictionary& motionDict
    );
    //- Merge refined boundary faces (from exposing coarser cell)
    void mergePatchFaces
    (
      const refinementParameters& refineParams,
      const dictionary& motionDict
    );
public:
  //- Runtime type information
  CLASS_NAME("autoRefineDriver");
  // Constructors
    //- Construct from components
    autoRefineDriver
    (
      meshRefinement& meshRefiner,
      decompositionMethod& decomposer,
      fvMeshDistribute& distributor,
      const labelList& globalToMasterPatch,
      const labelList& globalToSlavePatch
    );
    //- Disallow default bitwise copy construct
    autoRefineDriver(const autoRefineDriver&) = delete;
    //- Disallow default bitwise assignment
    autoRefineDriver& operator=(const autoRefineDriver&) = delete;
  // Member Functions
    //- Do all the refinement
    void doRefine
    (
      const dictionary& refineDict,
      const refinementParameters& refineParams,
      const snapParameters& snapParams,
      const bool prepareForSnapping,
      const dictionary& motionDict
    );
};
}  // namespace mousse
#endif
