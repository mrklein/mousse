#ifndef PARALLEL_DECOMPOSE_DECOMPOSITION_METHODS_MULTI_LEVEL_DECOMP_HPP_
#define PARALLEL_DECOMPOSE_DECOMPOSITION_METHODS_MULTI_LEVEL_DECOMP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multiLevelDecomp
// Description
//   Decomposition given using consecutive application of decomposers.
// SourceFiles
//   multi_level_decomp.cpp
#include "decomposition_method.hpp"
namespace mousse
{
class multiLevelDecomp
:
  public decompositionMethod
{
  // Private data
    dictionary methodsDict_;
    PtrList<decompositionMethod> methods_;
  // Private Member Functions
    //- Given connectivity across processors work out connectivity
    //  for a (consistent) subset
    void subsetGlobalCellCells
    (
      const label nDomains,
      const label domainI,
      const labelList& dist,
      const labelListList& cellCells,
      const labelList& set,
      labelListList& subCellCells,
      labelList& cutConnections
    ) const;
    //- Decompose level methodI without addressing
    void decompose
    (
      const labelListList& pointPoints,
      const pointField& points,
      const scalarField& pointWeights,
      const labelList& pointMap,  // map back to original points
      const label levelI,
      labelField& finalDecomp
    );
public:
  //- Runtime type information
  TYPE_NAME("multiLevel");
  // Constructors
    //- Construct given the decomposition dictionary
    multiLevelDecomp(const dictionary& decompositionDict);
    //- Disallow default bitwise copy construct and assignment
    multiLevelDecomp& operator=(const multiLevelDecomp&) = delete;
    multiLevelDecomp(const multiLevelDecomp&) = delete;
  //- Destructor
  virtual ~multiLevelDecomp()
  {}
  // Member Functions
    //- Is method parallel aware (i.e. does it synchronize domains across
    //  proc boundaries)
    virtual bool parallelAware() const;
    //- Inherit decompose from decompositionMethod
    using decompositionMethod::decompose;
    //- Return for every coordinate the wanted processor number. Use the
    //  mesh connectivity (if needed)
    virtual labelList decompose
    (
      const polyMesh& mesh,
      const pointField& points,
      const scalarField& pointWeights
    );
    //- Return for every coordinate the wanted processor number. Explicitly
    //  provided connectivity - does not use mesh_.
    virtual labelList decompose
    (
      const labelListList& globalCellCells,
      const pointField& cc,
      const scalarField& cWeights
    );
};
}  // namespace mousse
#endif
