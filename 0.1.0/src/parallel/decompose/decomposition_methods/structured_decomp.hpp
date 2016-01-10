// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::structuredDecomp
// Description
//   Decomposition by walking out decomposition of patch cells mesh.
// SourceFiles
//   structured_decomp.cpp
#ifndef structured_decomp_hpp_
#define structured_decomp_hpp_
#include "decomposition_method.hpp"
namespace mousse
{
class structuredDecomp
:
  public decompositionMethod
{
  // Private data
    dictionary methodDict_;
    wordReList patches_;
    autoPtr<decompositionMethod> method_;
public:
  //- Runtime type information
  TYPE_NAME("structured");
  // Constructors
    //- Construct given the decomposition dictionary
    structuredDecomp(const dictionary& decompositionDict);
    //- Disallow default bitwise copy construct and assignment
    structuredDecomp& operator=(const structuredDecomp&) = delete;
    structuredDecomp(const structuredDecomp&) = delete;
  //- Destructor
  virtual ~structuredDecomp()
  {}
  // Member Functions
    //- Is method parallel aware (i.e. does it synchronize domains across
    //  proc boundaries)
    virtual bool parallelAware() const;
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
