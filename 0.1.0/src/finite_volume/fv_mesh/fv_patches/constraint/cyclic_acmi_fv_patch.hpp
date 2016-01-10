// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicACMIFvPatch
// Description
//   Cyclic patch for Arbitrarily Coupled Mesh Interface (ACMI)
// SourceFiles
//   cyclic_acmi_fv_patch.cpp
#ifndef cyclic_acmi_fv_patch_hpp_
#define cyclic_acmi_fv_patch_hpp_
#include "coupled_fv_patch.hpp"
#include "cyclic_acmi_ldu_interface.hpp"
#include "cyclic_acmi_poly_patch.hpp"
#include "fv_boundary_mesh.hpp"
namespace mousse
{
class cyclicACMIFvPatch
:
  public coupledFvPatch,
  public cyclicACMILduInterface
{
  // Private data
    const cyclicACMIPolyPatch& cyclicACMIPolyPatch_;
protected:
  // Protected Member functions
    //- Update the patch areas after AMI update
    void updateAreas() const;
    //- Make patch weighting factors
    void makeWeights(scalarField&) const;
public:
  //- Runtime type information
  TYPE_NAME(cyclicACMIPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    cyclicACMIFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      coupledFvPatch(patch, bm),
      cyclicACMILduInterface(),
      cyclicACMIPolyPatch_(refCast<const cyclicACMIPolyPatch>(patch))
    {}
  // Member functions
    // Access
      //- Return local reference cast into the cyclic patch
      const cyclicACMIPolyPatch& cyclicACMIPatch() const
      {
        return cyclicACMIPolyPatch_;
      }
      //- Return neighbour
      virtual label neighbPatchID() const
      {
        return cyclicACMIPolyPatch_.neighbPatchID();
      }
      virtual bool owner() const
      {
        return cyclicACMIPolyPatch_.owner();
      }
      //- Return neighbour fvPatch
      virtual const cyclicACMIFvPatch& neighbPatch() const
      {
        return refCast<const cyclicACMIFvPatch>
        (
          this->boundaryMesh()[cyclicACMIPolyPatch_.neighbPatchID()]
        );
      }
      //- Return neighbour
      virtual label nonOverlapPatchID() const
      {
        return cyclicACMIPolyPatch_.nonOverlapPatchID();
      }
      //- Return non-overlapping fvPatch
      virtual const fvPatch& nonOverlapPatch() const
      {
        return this->boundaryMesh()[nonOverlapPatchID()];
      }
      //- Return a reference to the AMI interpolator
      virtual const AMIPatchToPatchInterpolation& AMI() const
      {
        const AMIPatchToPatchInterpolation& AMI =
          cyclicACMIPolyPatch_.AMI();
        updateAreas();
        return AMI;
      }
      //- Are the cyclic planes parallel
      virtual bool parallel() const
      {
        return cyclicACMIPolyPatch_.parallel();
      }
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return cyclicACMIPolyPatch_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return cyclicACMIPolyPatch_.reverseT();
      }
      const cyclicACMIFvPatch& neighbFvPatch() const
      {
        return refCast<const cyclicACMIFvPatch>
        (
          this->boundaryMesh()[cyclicACMIPolyPatch_.neighbPatchID()]
        );
      }
      //- Return true if this patch is coupled. This is equivalent
      //  to the coupledPolyPatch::coupled() if parallel running or
      //  both sides present, false otherwise
      virtual bool coupled() const;
      //- Return delta (P to N) vectors across coupled patch
      virtual tmp<vectorField> delta() const;
      template<class Type>
      tmp<Field<Type> > interpolate
      (
        const Field<Type>& fldCoupled
      ) const
      {
        updateAreas();
        return
          cyclicACMIPolyPatch_.cyclicAMIPolyPatch::interpolate
          (
            fldCoupled
          );
      }
      template<class Type>
      tmp<Field<Type> > interpolate
      (
        const tmp<Field<Type> >& tfldCoupled
      ) const
      {
        updateAreas();
        return
          cyclicACMIPolyPatch_.cyclicAMIPolyPatch::interpolate
          (
            tfldCoupled
          );
      }
      template<class Type>
      tmp<Field<Type> > interpolate
      (
        const Field<Type>& fldCoupled,
        const Field<Type>& fldNonOverlap
      ) const
      {
        updateAreas();
        return
          cyclicACMIPolyPatch_.interpolate
          (
            fldCoupled,
            fldNonOverlap
          );
      }
      template<class Type>
      tmp<Field<Type> > interpolate
      (
        const tmp<Field<Type> >& tFldCoupled,
        const tmp<Field<Type> >& tFldNonOverlap
      ) const
      {
        updateAreas();
        return
          cyclicACMIPolyPatch_.interpolate
          (
            tFldCoupled,
            tFldNonOverlap
          );
      }
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
