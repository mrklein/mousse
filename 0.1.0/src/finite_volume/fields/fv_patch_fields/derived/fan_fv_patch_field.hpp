// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fanFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition provides a jump condition, using the \c cyclic
//   condition as a base.
//   The jump is specified as a \c DataEntry type, to enable the use of, e.g.
//   contant, polynomial, table values.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     patchType    | underlying patch type should be \c cyclic| yes |
//     jumpTable    | jump data, e.g. \c csvFile | yes      |
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | none
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            fan;
//     patchType       cyclic;
//     jumpTable       csvFile;
//     csvFileCoeffs
//     {
//       hasHeaderLine   1;
//       refColumn       0;
//       componentColumns 1(1);
//       separator       ",";
//       fileName        "$MOUSSE_CASE/constant/pressureVsU";
//     }
//     value           uniform 0;
//   }
//   \endverbatim
//   The above example shows the use of a comma separated (CSV) file to specify
//   the jump condition.
// Note
//   The underlying \c patchType should be set to \c cyclic
// SeeAlso
//   mousse::DataEntry
// SourceFiles
//   fan_fv_patch_field.cpp
//   fan_fv_patch_fields.hpp
//   fan_fv_patch_fields.cpp
//   fan_fv_patch_fields_fwd.hpp
#ifndef fan_fv_patch_field_hpp_
#define fan_fv_patch_field_hpp_
#include "uniform_jump_fv_patch_field.hpp"
#include "data_entry.hpp"
namespace mousse
{
template<class Type>
class fanFvPatchField
:
  public uniformJumpFvPatchField<Type>
{
  // Private data
    //- Name of the flux transporting the field
    word phiName_;
    //- Name of the density field used to normalise the mass flux
    //  if neccessary
    word rhoName_;
  // Private Member Functions
    //- Calculate the fan pressure jump
    void calcFanJump();
public:
  //- Runtime type information
  TypeName("fan");
  // Constructors
    //- Construct from patch and internal field
    fanFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fanFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given fanFvPatchField onto a new patch
    fanFvPatchField
    (
      const fanFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fanFvPatchField
    (
      const fanFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new fanFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fanFvPatchField
    (
      const fanFvPatchField<Type>&,
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
        new fanFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
//- Specialisation of the jump-condition for the pressure
template<>
void fanFvPatchField<scalar>::calcFanJump();
template<>
fanFvPatchField<scalar>::fanFvPatchField
(
  const fvPatch&,
  const DimensionedField<scalar, volMesh>&,
  const dictionary&
);
}  // namespace mousse
#ifdef NoRepository
#   include "fan_fv_patch_field.cpp"
#endif
#endif
