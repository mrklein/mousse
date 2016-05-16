#ifndef PARALLEL_DECOMPOSE_DECOMPOSITION_METHODS_MANUAL_DECOMP_HPP_
#define PARALLEL_DECOMPOSE_DECOMPOSITION_METHODS_MANUAL_DECOMP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::manualDecomp
// Description
//   Decomposition given a cell-to-processor association in a file

#include "decomposition_method.hpp"


namespace mousse {

class manualDecomp
:
  public decompositionMethod
{
  // Private data
    fileName decompDataFile_;
public:
  //- Runtime type information
  TYPE_NAME("manual");
  // Constructors
    //- Construct given the decomposition dictionary
    manualDecomp(const dictionary& decompositionDict);
    //- Disallow default bitwise copy construct and assignment
    manualDecomp& operator=(const manualDecomp&) = delete;
    manualDecomp(const manualDecomp&) = delete;
  //- Destructor
  virtual ~manualDecomp()
  {}
  // Member Functions
    //- Manual decompose does not care about proc boundaries - is all
    //  up to the user.
    virtual bool parallelAware() const { return true; }
    //- Return for every coordinate the wanted processor number. Use the
    //  mesh connectivity (if needed)
    virtual labelList decompose
    (
      const polyMesh& mesh,
      const pointField& cc,
      const scalarField& cWeights
    );
    //- Return for every coordinate the wanted processor number. Explicitly
    //  provided connectivity - does not use mesh_.
    //  The connectivity is equal to mesh.cellCells() except for
    //  - in parallel the cell numbers are global cell numbers (starting
    //    from 0 at processor0 and then incrementing all through the
    //    processors)
    //  - the connections are across coupled patches
    virtual labelList decompose
    (
      const labelListList& /*globalCellCells*/,
      const pointField&,
      const scalarField& /*cWeights*/
    )
    {
      NOT_IMPLEMENTED
      (
        "decompose(const labelListList&, const pointField&"
        ", const scalarField&)"
      );
      return labelList{0};
    }
};

}  // namespace mousse

#endif

