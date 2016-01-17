// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::jumpCyclicFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition provides a base class for coupled-cyclic
//   conditions with a specified 'jump' (or offset) between the values
// SeeAlso
//   mousse::cyclicFvPatchField
// SourceFiles
//   jump_cyclic_fv_patch_field.cpp
#ifndef jump_cyclic_fv_patch_field_hpp_
#define jump_cyclic_fv_patch_field_hpp_
#include "cyclic_fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class jumpCyclicFvPatchField
:
  public cyclicFvPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("jumpCyclic");
  // Constructors
    //- Construct from patch and internal field
    jumpCyclicFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    jumpCyclicFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given jumpCyclicFvPatchField onto a new patch
    jumpCyclicFvPatchField
    (
      const jumpCyclicFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    jumpCyclicFvPatchField
    (
      const jumpCyclicFvPatchField<Type>&
    );
    //- Construct as copy setting internal field reference
    jumpCyclicFvPatchField
    (
      const jumpCyclicFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
  // Member functions
    // Access
      //- Return the interface type
      virtual const word& interfaceFieldType() const
      {
        return cyclicFvPatchField<Type>::type();
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
void jumpCyclicFvPatchField<scalar>::updateInterfaceMatrix
(
  scalarField& result,
  const scalarField& psiInternal,
  const scalarField& coeffs,
  const direction cmpt,
  const Pstream::commsTypes commsType
) const;
}  // namespace mousse
#ifdef NoRepository
#   include "jump_cyclic_fv_patch_field.cpp"
#endif
#endif
