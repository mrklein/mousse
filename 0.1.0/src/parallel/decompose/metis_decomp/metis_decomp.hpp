// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::metisDecomp
// Description
//   Metis domain decomposition
// SourceFiles
//   metis_decomp.cpp
#ifndef metis_decomp_hpp_
#define metis_decomp_hpp_
#include "decomposition_method.hpp"
namespace mousse
{
class metisDecomp
:
  public decompositionMethod
{
  // Private Member Functions
    //- Call Metis with options from dictionary.
    label decompose
    (
      const List<label>& adjncy,
      const List<label>& xadj,
      const scalarField& cellWeights,
      List<label>& finalDecomp
    );
public:
  //- Runtime type information
  TYPE_NAME("metis");
  // Constructors
    //- Construct given the decomposition dictionary
    metisDecomp(const dictionary&);
    //- Disallow default bitwise copy construct and assignment
    metisDecomp& operator=(const metisDecomp&) = delete;
    metisDecomp(const metisDecomp&) = delete;
  //- Destructor
  virtual ~metisDecomp()
  {}
  // Member Functions
    virtual bool parallelAware() const
    {
      // Metis does not know about proc boundaries
      return false;
    }
    //- Inherit decompose from decompositionMethod
    using decompositionMethod::decompose;
    //- Return for every coordinate the wanted processor number. Use the
    //  mesh connectivity (if needed)
    //  Weights get normalised so the minimum value is 1 before truncation
    //  to an integer so the weights should be multiples of the minimum
    //  value. The overall sum of weights might otherwise overflow.
    virtual labelList decompose
    (
      const polyMesh& mesh,
      const pointField& points,
      const scalarField& pointWeights
    );
    //- Return for every coordinate the wanted processor number. Gets
    //  passed agglomeration map (from fine to coarse cells) and coarse cell
    //  location. Can be overridden by decomposers that provide this
    //  functionality natively.
    //  See note on weights above.
    virtual labelList decompose
    (
      const polyMesh& mesh,
      const labelList& agglom,
      const pointField& regionPoints,
      const scalarField& regionWeights
    );
    //- Return for every coordinate the wanted processor number. Explicitly
    //  provided mesh connectivity.
    //  The connectivity is equal to mesh.cellCells() except for
    //  - in parallel the cell numbers are global cell numbers (starting
    //    from 0 at processor0 and then incrementing all through the
    //    processors)
    //  - the connections are across coupled patches
    //  See note on weights above.
    virtual labelList decompose
    (
      const labelListList& globalCellCells,
      const pointField& cc,
      const scalarField& cWeights
    );
};
}  // namespace mousse
#endif
