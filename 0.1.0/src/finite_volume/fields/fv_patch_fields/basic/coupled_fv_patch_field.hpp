// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coupledFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   Abstract base class for coupled patches.
// SourceFiles
//   coupled_fv_patch_field.cpp
#ifndef coupled_fv_patch_field_hpp_
#define coupled_fv_patch_field_hpp_
#include "_ldu_interface_field.hpp"
#include "fv_patch_field.hpp"
#include "coupled_fv_patch.hpp"
namespace mousse
{
template<class Type>
class coupledFvPatchField
:
  public LduInterfaceField<Type>,
  public fvPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME(coupledFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    coupledFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch and internal field and patch field
    coupledFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const Field<Type>&
    );
    //- Construct from patch, internal field and dictionary
    coupledFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping the given coupledFvPatchField onto a new patch
    coupledFvPatchField
    (
      const coupledFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    coupledFvPatchField
    (
      const coupledFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const = 0;
    //- Construct as copy setting internal field reference
    coupledFvPatchField
    (
      const coupledFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone
    (
      const DimensionedField<Type, volMesh>&
    ) const = 0;
  // Member functions
    // Access
      //- Return true if this patch field is derived from
      //  coupledFvPatchField<Type>.
      virtual bool coupled() const
      {
        return true;
      }
      //- Return neighbour field of internal field
      virtual tmp<Field<Type> > patchNeighbourField() const = 0;
    // Evaluation functions
      //- Return patch-normal gradient
      virtual tmp<Field<Type> > snGrad
      (
        const scalarField& deltaCoeffs
      ) const;
      //- Return patch-normal gradient
      virtual tmp<Field<Type> > snGrad() const
      {
        NOT_IMPLEMENTED
        (
          type() + "::coupledFvPatchField<Type>::snGrad()"
        );
        return *this;
      }
      //- Initialise the evaluation of the patch field
      virtual void initEvaluate
      (
        const Pstream::commsTypes commsType
      );
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType
      );
      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type> > valueInternalCoeffs
      (
        const tmp<scalarField>&
      ) const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type> > valueBoundaryCoeffs
      (
        const tmp<scalarField>&
      ) const;
      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type> > gradientInternalCoeffs
      (
        const scalarField& deltaCoeffs
      ) const;
      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type> > gradientInternalCoeffs() const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type> > gradientBoundaryCoeffs
      (
        const scalarField& deltaCoeffs
      ) const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type> > gradientBoundaryCoeffs() const;
    // Coupled interface functionality
      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        scalarField& result,
        const scalarField& psiInternal,
        const scalarField& coeffs,
        const direction,
        const Pstream::commsTypes commsType
      ) const = 0;
      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        Field<Type>&,
        const Field<Type>&,
        const scalarField&,
        const Pstream::commsTypes commsType
      ) const = 0;
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "coupled_fv_patch_field.cpp"
#endif
#endif
