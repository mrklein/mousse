#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_CONSTRAINT_JUMP_CYCLIC_AMI_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_CONSTRAINT_JUMP_CYCLIC_AMI_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::jumpCyclicAMIFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition provides a base class that enforces a cyclic
//   condition with a specified 'jump' (or offset) between a pair of boundaries,
//   whereby communication between the patches is performed using an arbitrary
//   mesh interface (AMI) interpolation.
// SeeAlso
//   mousse::cyclicAMIFvPatchField
// SourceFiles
//   jump_cyclic_ami_fv_patch_field.cpp
#include "cyclic_ami_fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class jumpCyclicAMIFvPatchField
:
  public cyclicAMIFvPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("jumpCyclicAMI");
  // Constructors
    //- Construct from patch and internal field
    jumpCyclicAMIFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    jumpCyclicAMIFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given jumpCyclicAMIFvPatchField onto a
    //  new patch
    jumpCyclicAMIFvPatchField
    (
      const jumpCyclicAMIFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    jumpCyclicAMIFvPatchField
    (
      const jumpCyclicAMIFvPatchField<Type>&
    );
    //- Construct as copy setting internal field reference
    jumpCyclicAMIFvPatchField
    (
      const jumpCyclicAMIFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
  // Member functions
    // Access
      //- Return the interface type
      virtual const word& interfaceFieldType() const
      {
        return cyclicAMIFvPatchField<Type>::type();
      }
      //- Return the "jump" across the patch as a "half" field
      virtual tmp<Field<Type> > jump() const = 0;
    // Evaluation functions
      //- Return neighbour coupled given internal cell data
      tmp<Field<Type> > patchNeighbourField() const;
      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        scalarField& result,
        const scalarField& psiInternal,
        const scalarField& coeffs,
        const direction cmpt,
        const Pstream::commsTypes commsType
      ) const;
      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        Field<Type>&,
        const Field<Type>&,
        const scalarField&,
        const Pstream::commsTypes commsType
      ) const;
};
//- Update result field based on interface functionality
template<>
void jumpCyclicAMIFvPatchField<scalar>::updateInterfaceMatrix
(
  scalarField& result,
  const scalarField& psiInternal,
  const scalarField& coeffs,
  const direction cmpt,
  const Pstream::commsTypes commsType
) const;
}  // namespace mousse
#ifdef NoRepository
#   include "jump_cyclic_ami_fv_patch_field.cpp"
#endif
#endif
