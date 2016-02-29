#ifndef FINITE_VOLUME_FV_MESH_FV_PATCHES_BASIC_COUPLED_FV_PATCH_HPP_
#define FINITE_VOLUME_FV_MESH_FV_PATCHES_BASIC_COUPLED_FV_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coupledFvPatch
// Description
//   An abstract base class for patches that couple regions of the
//   computational domain e.g. cyclic and processor-processor links.
// SourceFiles
//   coupled_fv_patch.cpp
#include "fv_patch.hpp"
#include "ldu_interface.hpp"
#include "coupled_poly_patch.hpp"
namespace mousse
{
class coupledFvPatch
:
  public lduInterface,
  public fvPatch
{
  // Private data
    const coupledPolyPatch& coupledPolyPatch_;
protected:
  // Protected Member Functions
    //- Make patch weighting factors
    virtual void makeWeights(scalarField&) const = 0;
public:
  friend class surfaceInterpolation;
  //- Runtime type information
  TYPE_NAME(coupledPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    coupledFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      fvPatch(patch, bm),
      coupledPolyPatch_(refCast<const coupledPolyPatch>(patch))
    {}
  //- Destructor
  virtual ~coupledFvPatch();
  // Member Functions
    // Access
      //- Return true because this patch is coupled
      virtual bool coupled() const
      {
        return coupledPolyPatch_.coupled();
      }
      //- Are the cyclic planes parallel.
      virtual bool parallel() const = 0;
      //- Return face transformation tensor.
      virtual const tensorField& forwardT() const = 0;
      //- Return neighbour-cell transformation tensor.
      virtual const tensorField& reverseT() const = 0;
      //- Return faceCell addressing
      virtual const labelUList& faceCells() const
      {
        return fvPatch::faceCells();
      }
      //- Return delta (P to N) vectors across coupled patch
      virtual tmp<vectorField> delta() const = 0;
    // Interface transfer functions
      //- Return the values of the given internal data adjacent to
      //  the interface as a field
      virtual tmp<labelField> interfaceInternalField
      (
        const labelUList& internalData
      ) const = 0;
      //- Inherit initInternalFieldTransfer from lduInterface
      using lduInterface::initInternalFieldTransfer;
      //- Initialise neighbour field transfer
      virtual void initInternalFieldTransfer
      (
        const Pstream::commsTypes,
        labelUList& /*iF - internal field*/
      ) const
      {}
      //- Return neighbour field
      virtual tmp<labelField> internalFieldTransfer
      (
        const Pstream::commsTypes commsType,
        const labelUList& iF
      ) const = 0;
};
}  // namespace mousse
#endif
