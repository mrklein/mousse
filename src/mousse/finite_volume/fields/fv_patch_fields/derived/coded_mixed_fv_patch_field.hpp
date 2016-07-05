#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_CODED_MIXED_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_CODED_MIXED_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::codedMixedFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   Constructs on-the-fly a new boundary condition (derived from
//   mixedFvPatchField) which is then used to evaluate.
//   \heading Patch usage
//   Example:
//   \verbatim
//   myPatch
//   {
//     type            codedMixed;
//     refValue        uniform (0 0 0);
//     refGradient     uniform (0 0 0);
//     valueFraction   uniform 1;
//     redirectType    rampedMixed;   // name of generated BC
//     code
//     #{
//       this->refValue() =
//         vector(1, 0, 0)
//        *min(10, 0.1*this->db().time().value());
//       this->refGrad() = vector::zero;
//       this->valueFraction() = 1.0;
//     #};
//     //codeInclude
//     //#{
//     //    #include "fv_cfd.hpp"
//     //#};
//     //codeOptions
//     //#{
//     //    -I$(LIB_SRC)/finiteVolume/lnInclude
//     //#};
//   }
//   \endverbatim
//   A special form is if the 'code' section is not supplied. In this case
//   the code gets read from a (runTimeModifiable!) dictionary system/codeDict
//   which would have a corresponding entry
//   \verbatim
//   myPatch
//   {
//     code
//     #{
//       this->refValue() = min(10, 0.1*this->db().time().value());
//       this->refGrad() = vector::zero;
//       this->valueFraction() = 1.0;
//     #};
//   }
//   \endverbatim
// SeeAlso
//   mousse::dynamicCode
//   mousse::functionEntries::codeStream

#include "mixed_fv_patch_fields.hpp"
#include "coded_base.hpp"


namespace mousse {

// Forward declaration of classes
class dynamicCode;
class dynamicCodeContext;
class IOdictionary;


template<class Type>
class codedMixedFvPatchField
:
  public mixedFvPatchField<Type>,
  public codedBase
{
  // Private data
    //- Dictionary contents for the boundary condition
    mutable dictionary dict_;
    const word redirectType_;
    mutable autoPtr<mixedFvPatchField<Type>> redirectPatchFieldPtr_;
  // Private Member Functions
    const IOdictionary& dict() const;
    //- Set the rewrite vars controlling the Type
    static void setFieldTemplates(dynamicCode& dynCode);
    //- Get the loaded dynamic libraries
    virtual dlLibraryTable& libs() const;
    //- Adapt the context for the current object
    virtual void prepare(dynamicCode&, const dynamicCodeContext&) const;
    // Return a description (type + name) for the output
    virtual string description() const;
    // Clear the ptr to the redirected object
    virtual void clearRedirect() const;
    // Get the dictionary to initialize the codeContext
    virtual const dictionary& codeDict() const;
public:
  // Static data members
    //- Name of the C code template to be used
    static const word codeTemplateC;
    //- Name of the H code template to be used
    static const word codeTemplateH;
  //- Runtime type information
  TYPE_NAME("codedMixed");
  // Constructors
    //- Construct from patch and internal field
    codedMixedFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    codedMixedFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given codedMixedFvPatchField
    //  onto a new patch
    codedMixedFvPatchField
    (
      const codedMixedFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    codedMixedFvPatchField
    (
      const codedMixedFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new codedMixedFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    codedMixedFvPatchField
    (
      const codedMixedFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type>> clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type>>
      {
        new codedMixedFvPatchField<Type>{*this, iF}
      };
    }
  // Member functions
    //- Get reference to the underlying patchField
    const mixedFvPatchField<Type>& redirectPatchField() const;
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Evaluate the patch field
    //  This is only needed to set the updated() flag of the redirectType
    //  to false.
    virtual void evaluate
    (
      const Pstream::commsTypes commsType=Pstream::blocking
    );
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse

#include "coded_mixed_fv_patch_field.ipp"

#endif
