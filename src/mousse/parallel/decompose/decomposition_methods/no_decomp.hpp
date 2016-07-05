#ifndef PARALLEL_DECOMPOSE_DECOMPOSITION_METHODS_NO_DECOMP_HPP_
#define PARALLEL_DECOMPOSE_DECOMPOSITION_METHODS_NO_DECOMP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noDecomp
// Description
//   Dummy decomposition method

#include "decomposition_method.hpp"


namespace mousse {

class noDecomp
:
  public decompositionMethod
{
public:
  //- Runtime type information
  TYPE_NAME("noDecomp");
  // Constructors
    //- Construct given the decomposition dictionary
    noDecomp(const dictionary& decompositionDict);
    //- Disallow default bitwise copy construct and assignment
    noDecomp& operator=(const noDecomp&) = delete;
    noDecomp(const noDecomp&) = delete;
  //- Destructor
  virtual ~noDecomp()
  {}
  // Member Functions
    //- Manual decompose does not care about proc boundaries - is all
    //  up to the user.
    virtual bool parallelAware() const
    {
      return true;
    }
    //- Return for every coordinate the wanted processor number. Use the
    //  mesh connectivity (if needed)
    virtual labelList decompose
    (
      const polyMesh&,
      const pointField& cc,
      const scalarField& /*cWeights*/
    )
    {
      return labelList(cc.size(), Pstream::myProcNo());
    }
    //- Return for every coordinate the wanted processor number. Explicitly
    //  provided connectivity - does not use mesh_.
    //  The connectivity is equal to mesh.cellCells() except for
    //  - in parallel the cell numbers are global cell numbers (starting
    //    from 0 at processor0 and then incrementing all through the
    //    processors)
    //  - the connections are across coupled patches
    virtual labelList decompose
    (
      const labelListList& globalCellCells,
      const pointField& /*cc*/,
      const scalarField& /*cWeights*/
    )
    {
      return labelList(globalCellCells.size(), Pstream::myProcNo());
    }
};

}  // namespace mousse

#endif

