// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorFvPatch
// Description
//   Processor patch.
// SourceFiles
//   processor_fv_patch.cpp
#ifndef processor_fv_patch_hpp_
#define processor_fv_patch_hpp_
#include "coupled_fv_patch.hpp"
#include "processor_ldu_interface.hpp"
#include "processor_poly_patch.hpp"
namespace mousse
{
class processorFvPatch
:
  public coupledFvPatch,
  public processorLduInterface
{
  // Private Data
    const processorPolyPatch& procPolyPatch_;
protected:
  // Protected Member functions
    //- Make patch weighting factors
    void makeWeights(scalarField&) const;
public:
  //- Runtime type information
  TYPE_NAME(processorPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    processorFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      coupledFvPatch(patch, bm),
      procPolyPatch_(refCast<const processorPolyPatch>(patch))
    {}
  // Member functions
    //- Return communicator used for comms
    virtual label comm() const
    {
      return procPolyPatch_.comm();
    }
    //- Return processor number
    virtual int myProcNo() const
    {
      return procPolyPatch_.myProcNo();
    }
    //- Return neigbour processor number
    virtual int neighbProcNo() const
    {
      return procPolyPatch_.neighbProcNo();
    }
    //- Return message tag used for sending
    virtual int tag() const
    {
      return UPstream::msgType();
    }
    //- Return true if running parallel
    virtual bool coupled() const
    {
      if (Pstream::parRun())
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    const processorPolyPatch& procPolyPatch() const
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
    //- Return neighbour-cell transformation tensor.
    virtual const tensorField& reverseT() const
    {
      return procPolyPatch_.reverseT();
    }
    //- Return delta (P to N) vectors across coupled patch
    virtual tmp<vectorField> delta() const;
    // Interface transfer functions
      //- Return the values of the given internal data adjacent to
      //  the interface as a field
      virtual tmp<labelField> interfaceInternalField
      (
        const labelUList& internalData
      ) const;
      //- Inherit initInternalFieldTransfer from coupledFvPatch
      using coupledFvPatch::initInternalFieldTransfer;
      //- Initialise neighbour field transfer
      virtual void initInternalFieldTransfer
      (
        const Pstream::commsTypes commsType,
        const labelUList& internalData
      ) const;
      //- Return neighbour field
      virtual tmp<labelField> internalFieldTransfer
      (
        const Pstream::commsTypes commsType,
        const labelUList& internalData
      ) const;
};
}  // namespace mousse
#endif
