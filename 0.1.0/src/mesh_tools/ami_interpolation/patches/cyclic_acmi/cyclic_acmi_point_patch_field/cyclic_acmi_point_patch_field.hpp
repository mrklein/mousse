// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicACMIPointPatchField
// Description
//   Cyclic ACMI front and back plane patch field
// SourceFiles
//   cyclic_acmi_point_patch_field.cpp
#ifndef cyclic_acmi_point_patch_field_hpp_
#define cyclic_acmi_point_patch_field_hpp_
#include "coupled_point_patch_field.hpp"
#include "cyclic_acmi_point_patch.hpp"
#include "primitive_patch_interpolation.hpp"
namespace mousse
{
template<class Type>
class cyclicACMIPointPatchField
:
  public coupledPointPatchField<Type>
{
  // Private data
    //- Local reference cast into the cyclicACMI patch
    const cyclicACMIPointPatch& cyclicACMIPatch_;
    //- Owner side patch interpolation pointer
    mutable autoPtr<PrimitivePatchInterpolation<primitivePatch> > ppiPtr_;
    //- Neighbour side patch interpolation pointer
    mutable autoPtr<PrimitivePatchInterpolation<primitivePatch> >
      nbrPpiPtr_;
  // Private Member Functions
    //- Owner side patch interpolation
    const PrimitivePatchInterpolation<primitivePatch>& ppi() const
    {
      if (!ppiPtr_.valid())
      {
        ppiPtr_.reset
        (
          new PrimitivePatchInterpolation<primitivePatch>
          (
            cyclicACMIPatch_.cyclicACMIPatch()
          )
        );
      }
      return ppiPtr_();
    }
    //- Neighbour side patch interpolation
    const PrimitivePatchInterpolation<primitivePatch>& nbrPpi() const
    {
      if (!nbrPpiPtr_.valid())
      {
        nbrPpiPtr_.reset
        (
          new PrimitivePatchInterpolation<primitivePatch>
          (
            cyclicACMIPatch_.cyclicACMIPatch().neighbPatch()
          )
        );
      }
      return nbrPpiPtr_();
    }
public:
  //- Runtime type information
  TypeName(cyclicACMIPointPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    cyclicACMIPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    cyclicACMIPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    cyclicACMIPointPatchField
    (
      const cyclicACMIPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new cyclicACMIPointPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    cyclicACMIPointPatchField
    (
      const cyclicACMIPointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type> > clone
    (
      const DimensionedField<Type, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new cyclicACMIPointPatchField<Type>
        (
          *this, iF
        )
      );
    }
  // Member functions
    // Constraint handling
      //- Return the constraint type this pointPatchField implements
      virtual const word& constraintType() const
      {
        return cyclicACMIPointPatch::typeName;
      }
    // Cyclic AMI coupled interface functions
      //- Does the patch field perform the transfromation
      virtual bool doTransform() const
      {
        return
         !(
            cyclicACMIPatch_.parallel()
          || pTraits<Type>::rank == 0
          );
      }
      //- Return face transformation tensor
      virtual const tensorField& forwardT() const
      {
        return cyclicACMIPatch_.forwardT();
      }
      //- Return neighbour-cell transformation tensor
      virtual const tensorField& reverseT() const
      {
        return cyclicACMIPatch_.reverseT();
      }
    // Evaluation functions
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      )
      {}
      //- Complete swap of patch point values and add to local values
      virtual void swapAddSeparated
      (
        const Pstream::commsTypes commsType,
        Field<Type>&
      ) const;
};
}  // namespace mousse
#ifdef NoRepository
#    include "cyclic_acmi_point_patch_field.cpp"
#endif
#endif
