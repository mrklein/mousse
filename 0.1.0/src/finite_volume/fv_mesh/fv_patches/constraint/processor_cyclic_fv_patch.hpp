// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorCyclicFvPatch
// Description
//   Processor patch.
// SourceFiles
//   processor_cyclic_fv_patch.cpp
#ifndef processor_cyclic_fv_patch_hpp_
#define processor_cyclic_fv_patch_hpp_
#include "processor_cyclic_poly_patch.hpp"
#include "processor_fv_patch.hpp"
namespace mousse
{
class processorCyclicFvPatch
:
  public processorFvPatch
{
  // Private Data
    const processorCyclicPolyPatch& procPolyPatch_;
public:
  //- Runtime type information
  TYPE_NAME(processorCyclicPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    processorCyclicFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      processorFvPatch(patch, bm),
      procPolyPatch_(refCast<const processorCyclicPolyPatch>(patch))
    {}
  // Member functions
    //- Return message tag used for sending
    virtual int tag() const
    {
      // Allocate from Pstream?
      return procPolyPatch_.tag();
    }
    const processorCyclicPolyPatch& procPolyPatch() const
    {
      return procPolyPatch_;
    }
    //- Are the cyclic planes parallel
    virtual bool parallel() const
    {
      return procPolyPatch_.parallel();
    }
    //- Return face transformation tensor
    virtual const tensorField& forwardT() const
    {
      return procPolyPatch_.forwardT();
    }
    //- Return neighbour-cell transformation tensor
    virtual const tensorField& reverseT() const
    {
      return procPolyPatch_.reverseT();
    }
};
}  // namespace mousse
#endif
