// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicAMIFvPatch
// Description
//   Cyclic patch for Arbitrary Mesh Interface (AMI)
// SourceFiles
//   cyclic_ami_fv_patch.cpp
#ifndef cyclic_ami_fv_patch_hpp_
#define cyclic_ami_fv_patch_hpp_
#include "coupled_fv_patch.hpp"
#include "cyclic_ami_ldu_interface.hpp"
#include "cyclic_ami_poly_patch.hpp"
#include "fv_boundary_mesh.hpp"
namespace mousse
{
class cyclicAMIFvPatch
:
  public coupledFvPatch,
  public cyclicAMILduInterface
{
  // Private data
    const cyclicAMIPolyPatch& cyclicAMIPolyPatch_;
protected:
  // Protected Member functions
    //- Make patch weighting factors
    void makeWeights(scalarField&) const;
public:
  //- Runtime type information
  TYPE_NAME(cyclicAMIPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    cyclicAMIFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      coupledFvPatch(patch, bm),
      cyclicAMILduInterface(),
      cyclicAMIPolyPatch_(refCast<const cyclicAMIPolyPatch>(patch))
    {}
  // Member functions
    // Access
      //- Return local reference cast into the cyclic patch
      const cyclicAMIPolyPatch& cyclicAMIPatch() const
      {
        return cyclicAMIPolyPatch_;
      }
      //- Return neighbour
      virtual label neighbPatchID() const
      {
        return cyclicAMIPolyPatch_.neighbPatchID();
      }
      virtual bool owner() const
      {
        return cyclicAMIPolyPatch_.owner();
      }
      //- Return processor number
      virtual const cyclicAMIFvPatch& neighbPatch() const
      {
        return refCast<const cyclicAMIFvPatch>
        (
          this->boundaryMesh()[cyclicAMIPolyPatch_.neighbPatchID()]
        );
      }
      //- Return a reference to the AMI interpolator
      virtual const AMIPatchToPatchInterpolation& AMI() const
      {
        return cyclicAMIPolyPatch_.AMI();
      }
      //- Return true if applying the low weight correction
      virtual bool applyLowWeightCorrection() const
      {
        return cyclicAMIPolyPatch_.applyLowWeightCorrection();
      }
      //- Are the cyclic planes parallel
      virtual bool parallel() const
      {
        return cyclicAMIPolyPatch_.parallel();
      }
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return cyclicAMIPolyPatch_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return cyclicAMIPolyPatch_.reverseT();
      }
      const cyclicAMIFvPatch& neighbFvPatch() const
      {
        return refCast<const cyclicAMIFvPatch>
        (
          this->boundaryMesh()[cyclicAMIPolyPatch_.neighbPatchID()]
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
        const Field<Type>& fld,
        const UList<Type>& defaultValues = UList<Type>()
      ) const
      {
        return cyclicAMIPolyPatch_.interpolate(fld, defaultValues);
      }
      template<class Type>
      tmp<Field<Type> > interpolate
      (
        const tmp<Field<Type> >& tFld,
        const UList<Type>& defaultValues = UList<Type>()
      ) const
      {
        return cyclicAMIPolyPatch_.interpolate(tFld, defaultValues);
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
