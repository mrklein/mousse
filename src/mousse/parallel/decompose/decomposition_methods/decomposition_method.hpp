#ifndef PARALLEL_DECOMPOSE_DECOMPOSITION_METHODS_DECOMPOSITION_METHOD_HPP_
#define PARALLEL_DECOMPOSE_DECOMPOSITION_METHODS_DECOMPOSITION_METHOD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::decompositionMethod
// Description
//   Abstract base class for decomposition

#include "poly_mesh.hpp"
#include "point_field.hpp"
#include "compact_list_list.hpp"


namespace mousse {

class decompositionMethod
{
protected:
  // Protected data
    const dictionary& decompositionDict_;
    label nProcessors_;
public:
  //- Runtime type information
  TYPE_NAME("decompositionMethod");
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      decompositionMethod,
      dictionary,
      (
        const dictionary& decompositionDict
      ),
      (decompositionDict)
    );
  // Selectors
    //- Return a reference to the selected decomposition method
    static autoPtr<decompositionMethod> New
    (
      const dictionary& decompositionDict
    );
  // Constructors
    //- Construct given the decomposition dictionary
    decompositionMethod(const dictionary& decompositionDict)
    :
      decompositionDict_{decompositionDict},
      nProcessors_
      {
        readLabel(decompositionDict.lookup("numberOfSubdomains"))
      }
    {}
    //- Disallow default bitwise copy construct and assignment
    decompositionMethod(const decompositionMethod&) = delete;
    decompositionMethod& operator=(const decompositionMethod&) = delete;
  //- Destructor
  virtual ~decompositionMethod()
  {}
  // Member Functions
    label nDomains() const { return nProcessors_; }
    //- Is method parallel aware (i.e. does it synchronize domains across
    //  proc boundaries)
    virtual bool parallelAware() const = 0;
    // No topology (implemented by geometric decomposers)
      //- Return for every coordinate the wanted processor number.
      virtual labelList decompose
      (
        const pointField&,
        const scalarField& /*pointWeights*/
      )
      {
        NOT_IMPLEMENTED
        (
          "decompositionMethod:decompose(const pointField&"
          ", const scalarField&)"
        );
        return labelList{0};
      }
      //- Like decompose but with uniform weights on the points
      virtual labelList decompose(const pointField&)
      {
        NOT_IMPLEMENTED
        (
          "decompositionMethod:decompose(const pointField&)"
        );
        return labelList{0};
      }
    // Topology provided by mesh
      //- Return for every coordinate the wanted processor number. Use the
      //  mesh connectivity (if needed)
      virtual labelList decompose
      (
        const polyMesh& mesh,
        const pointField& points,
        const scalarField& pointWeights
      ) = 0;
      //- Like decompose but with uniform weights on the points
      virtual labelList decompose(const polyMesh&, const pointField&);
      //- Return for every coordinate the wanted processor number. Gets
      //  passed agglomeration map (from fine to coarse cells) and coarse
      //  cell
      //  location. Can be overridden by decomposers that provide this
      //  functionality natively. Coarse cells are local to the processor
      //  (if in parallel). If you want to have coarse cells spanning
      //  processors use the globalCellCells instead.
      virtual labelList decompose
      (
        const polyMesh& mesh,
        const labelList& cellToRegion,
        const pointField& regionPoints,
        const scalarField& regionWeights
      );
      //- Like decompose but with uniform weights on the regions
      virtual labelList decompose
      (
        const polyMesh& mesh,
        const labelList& cellToRegion,
        const pointField& regionPoints
      );
    // Topology provided explicitly addressing
      //- Return for every coordinate the wanted processor number.
      //  The connectivity is equal to mesh.cellCells() except for
      //  - in parallel the cell numbers are global cell numbers
      //    (starting
      //    from 0 at processor0 and then incrementing all through the
      //    processors)
      //  - the connections are across coupled patches
      virtual labelList decompose
      (
        const labelListList& globalCellCells,
        const pointField& cc,
        const scalarField& cWeights
      ) = 0;
      //- Like decompose but with uniform weights on the cells
      virtual labelList decompose
      (
        const labelListList& globalCellCells,
        const pointField& cc
      );
    // Other
      //- Helper: determine (local or global) cellCells from mesh
      //  agglomeration. Agglomeration is local to the processor.
      //  local  : connections are in local indices. Coupled across
      //           cyclics but not processor patches.
      //  global : connections are in global indices. Coupled across
      //            cyclics and processor patches.
      static void calcCellCells
      (
        const polyMesh& mesh,
        const labelList& agglom,
        const label nLocalCoarse,
        const bool global,
        CompactListList<label>& cellCells
      );
      //- Helper: extract constraints:
      //  blockedface: existing faces where owner and neighbour on same
      //               proc
      //  explicitConnections: sets of boundary faces  ,,     ,,
      //  specifiedProcessorFaces: groups of faces with all cells on
      //  same processor.
      void setConstraints
      (
        const polyMesh& mesh,
        boolList& blockedFace,
        PtrList<labelList>& specifiedProcessorFaces,
        labelList& specifiedProcessor,
        List<labelPair>& explicitConnections
      );
      // Decompose a mesh with constraints:
      // - blockedFace : whether owner and neighbour should be on same
      //   processor
      // - specifiedProcessorFaces, specifiedProcessor : sets of faces
      //   that should go to same processor (as specified in
      //   specifiedProcessor, can be -1)
      // - explicitConnections : connections between baffle faces
      //   (blockedFace should be false on these). Owner and
      //   neighbour on same processor.
      // Set all to zero size to have unconstrained decomposition.
      virtual labelList decompose
      (
        const polyMesh& mesh,
        const scalarField& cellWeights,
        const boolList& blockedFace,
        const PtrList<labelList>& specifiedProcessorFaces,
        const labelList& specifiedProcessor,
        const List<labelPair>& explicitConnections
      );
      //- Decompose a mesh. Apply all constraints from decomposeParDict
      //  ('preserveFaceZones' etc). Calls either
      //  - no constraints, empty weights:
      //      decompose(mesh, cellCentres())
      //  - no constraints, set weights:
      //      decompose(mesh, cellCentres(), cellWeights)
      //  - valid constraints:
      //      decompose(mesh, cellToRegion, regionPoints, regionWeights)
      labelList decompose
      (
        const polyMesh& mesh,
        const scalarField& cWeights
      );
};

}  // namespace mousse

#endif

