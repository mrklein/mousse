// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::outletMappedUniformInletFvPatchField
// Group
//   grpInletBoundaryConditions
// Description
//   This boundary conditon averages the field over the "outlet" patch specified
//   by name "outletPatchName" and applies this as the uniform value of the
//   field over this patch.
//   \heading Patch usage
//   \table
//     Property        | Description             | Required    | Default value
//     outletPatchName | name of outlet patch    | yes         |
//     phi             | flux field name         | no          | phi
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            outletMappedUniformInlet;
//     outletPatchName aPatch;
//     phi             phi;
//     value           uniform 0;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   outlet_mapped_uniform_inlet_fv_patch_field.cpp
#ifndef outlet_mapped_uniform_inlet_fv_patch_field_hpp_
#define outlet_mapped_uniform_inlet_fv_patch_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
template<class Type>
class outletMappedUniformInletFvPatchField
:
  public fixedValueFvPatchField<Type>
{
  // Private data
    //- Name of the outlet patch to be mapped
    word outletPatchName_;
   //- Name of the flux transporting the field
    word phiName_;
public:
  //- Runtime type information
  TYPE_NAME("outletMappedUniformInlet");
  // Constructors
    //- Construct from patch and internal field
    outletMappedUniformInletFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    outletMappedUniformInletFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given outletMappedUniformInletFvPatchField
    //  onto a new patch
    outletMappedUniformInletFvPatchField
    (
      const outletMappedUniformInletFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    outletMappedUniformInletFvPatchField
    (
      const outletMappedUniformInletFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new outletMappedUniformInletFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    outletMappedUniformInletFvPatchField
    (
      const outletMappedUniformInletFvPatchField<Type>&,
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
        new outletMappedUniformInletFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Name of the outlet patch to be mapped
      const word& outletPatchName() const
      {
        return outletPatchName_;
      }
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "outlet_mapped_uniform_inlet_fv_patch_field.cpp"
#endif
#endif
