// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicSlipFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition is a light wrapper around the cyclicFvPatchField
//   condition, providing no new functionality.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            cyclicSlip;
//   }
//   \endverbatim
// SeeAlso
//   mousse::cyclicFvPatchField
// SourceFiles
//   cyclic_slip_fv_patch_field.cpp
#ifndef cyclic_slip_fv_patch_field_hpp_
#define cyclic_slip_fv_patch_field_hpp_
#include "cyclic_fv_patch_field.hpp"
#include "cyclic_slip_fv_patch.hpp"
namespace mousse
{
template<class Type>
class cyclicSlipFvPatchField
:
  public cyclicFvPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME(cyclicSlipFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    cyclicSlipFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    cyclicSlipFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given cyclicSlipFvPatchField onto a new patch
    cyclicSlipFvPatchField
    (
      const cyclicSlipFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    cyclicSlipFvPatchField
    (
      const cyclicSlipFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new cyclicSlipFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    cyclicSlipFvPatchField
    (
      const cyclicSlipFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type> > clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type> >
      (
        new cyclicSlipFvPatchField<Type>(*this, iF)
      );
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "cyclic_slip_fv_patch_field.cpp"
#endif
#endif
