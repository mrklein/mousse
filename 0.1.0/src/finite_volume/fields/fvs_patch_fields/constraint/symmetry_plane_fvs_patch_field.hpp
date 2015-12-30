// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryPlaneFvsPatchField
// Description
//   mousse::symmetryPlaneFvsPatchField
// SourceFiles
//   symmetry_plane_fvs_patch_field.cpp
#ifndef symmetry_plane_fvs_patch_field_hpp_
#define symmetry_plane_fvs_patch_field_hpp_
#include "fvs_patch_field.hpp"
#include "symmetry_plane_fv_patch.hpp"
namespace mousse
{
template<class Type>
class symmetryPlaneFvsPatchField
:
  public fvsPatchField<Type>
{
public:
  //- Runtime type information
  TypeName(symmetryPlaneFvPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    symmetryPlaneFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct from patch, internal field and dictionary
    symmetryPlaneFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );
    //- Construct by mapping given symmetryPlaneFvsPatchField
    //  onto a new patch
    symmetryPlaneFvsPatchField
    (
      const symmetryPlaneFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    symmetryPlaneFvsPatchField
    (
      const symmetryPlaneFvsPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type> > clone() const
    {
      return tmp<fvsPatchField<Type> >
      (
        new symmetryPlaneFvsPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    symmetryPlaneFvsPatchField
    (
      const symmetryPlaneFvsPatchField<Type>&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvsPatchField<Type> > clone
    (
      const DimensionedField<Type, surfaceMesh>& iF
    ) const
    {
      return tmp<fvsPatchField<Type> >
      (
        new symmetryPlaneFvsPatchField<Type>(*this, iF)
      );
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "symmetry_plane_fvs_patch_field.cpp"
#endif
#endif
