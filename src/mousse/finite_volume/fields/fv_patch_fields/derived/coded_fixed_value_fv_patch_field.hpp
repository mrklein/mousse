#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_CODED_FIXED_VALUE_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_CODED_FIXED_VALUE_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::codedFixedValueFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   Constructs on-the-fly a new boundary condition (derived from
//   fixedValueFvPatchField) which is then used to evaluate.
//   \heading Patch usage
//   Example:
//   \verbatim
//   myPatch
//   {
//     type            codedFixedValue;
//     value           uniform 0;
//     redirectType    rampedFixedValue;   // name of generated BC
//     code
//     #{
//       operator==(min(10, 0.1*this->db().time().value()));
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
//   the code is read from a (runTimeModifiable!) dictionary system/codeDict
//   which would have a corresponding entry:
//   \verbatim
//   myPatch
//   {
//     code
//     #{
//       operator==(min(10, 0.1*this->db().time().value()));
//     #};
//   }
//   \endverbatim
// SeeAlso
//   mousse::dynamicCode
//   mousse::functionEntries::codeStream

#include "fixed_value_fv_patch_fields.hpp"
#include "coded_base.hpp"


namespace mousse {

// Forward declaration of classes
class dynamicCode;
class dynamicCodeContext;
class IOdictionary;


template<class Type>
class codedFixedValueFvPatchField
:
  public fixedValueFvPatchField<Type>,
  public codedBase
{
  // Private data
    //- Dictionary contents for the boundary condition
    const dictionary dict_;
    const word redirectType_;
    mutable autoPtr<fvPatchField<Type> > redirectPatchFieldPtr_;
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
  TYPE_NAME("codedFixedValue");
  // Constructors
    //- Construct from patch and internal field
    codedFixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    codedFixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given codedFixedValueFvPatchField
    //  onto a new patch
    codedFixedValueFvPatchField
    (
      const codedFixedValueFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    codedFixedValueFvPatchField
    (
      const codedFixedValueFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new codedFixedValueFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    codedFixedValueFvPatchField
    (
      const codedFixedValueFvPatchField<Type>&,
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
        new codedFixedValueFvPatchField<Type>{*this, iF}
      };
    }
  // Member functions
    //- Get reference to the underlying patch
    const fvPatchField<Type>& redirectPatchField() const;
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Evaluate the patch field, sets Updated to false
    virtual void evaluate
    (
      const Pstream::commsTypes commsType=Pstream::blocking
    );
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse

#include "coded_fixed_value_fv_patch_field.ipp"

#endif
