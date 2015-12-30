// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicFvPatch
// Description
//   Cyclic-plane patch.
// SourceFiles
//   cyclic_fv_patch.cpp
#ifndef cyclic_fv_patch_hpp_
#define cyclic_fv_patch_hpp_
#include "coupled_fv_patch.hpp"
#include "cyclic_ldu_interface.hpp"
#include "cyclic_poly_patch.hpp"
#include "fv_boundary_mesh.hpp"
namespace mousse
{
class cyclicFvPatch
:
  public coupledFvPatch,
  public cyclicLduInterface
{
  // Private data
    const cyclicPolyPatch& cyclicPolyPatch_;
protected:
  // Protected Member functions
    //- Make patch weighting factors
    void makeWeights(scalarField&) const;
public:
  //- Runtime type information
  TypeName(cyclicPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    cyclicFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      coupledFvPatch(patch, bm),
      cyclicPolyPatch_(refCast<const cyclicPolyPatch>(patch))
    {}
  // Member functions
    // Access
      //- Return local reference cast into the cyclic patch
      const cyclicPolyPatch& cyclicPatch() const
      {
        return cyclicPolyPatch_;
      }
      //- Return neighbour
      virtual label neighbPatchID() const
      {
        return cyclicPolyPatch_.neighbPatchID();
      }
      virtual bool owner() const
      {
        return cyclicPolyPatch_.owner();
      }
      //- Return processor number
      virtual const cyclicFvPatch& neighbPatch() const
      {
        return refCast<const cyclicFvPatch>
        (
          this->boundaryMesh()[cyclicPolyPatch_.neighbPatchID()]
        );
      }
      //- Are the cyclic planes parallel
      virtual bool parallel() const
      {
        return cyclicPolyPatch_.parallel();
      }
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return cyclicPolyPatch_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return cyclicPolyPatch_.reverseT();
      }
      const cyclicFvPatch& neighbFvPatch() const
      {
        return refCast<const cyclicFvPatch>
        (
          this->boundaryMesh()[cyclicPolyPatch_.neighbPatchID()]
        );
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
      //- Return neighbour field
      virtual tmp<labelField> internalFieldTransfer
      (
        const Pstream::commsTypes commsType,
        const labelUList& internalData
      ) const;
};
}  // namespace mousse
#endif
