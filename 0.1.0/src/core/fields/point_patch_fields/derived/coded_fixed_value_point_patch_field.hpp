#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_DERIVED_CODED_FIXED_VALUE_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_DERIVED_CODED_FIXED_VALUE_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::codedFixedValuePointPatchField
// Description
//   Constructs on-the-fly a new boundary condition (derived from
//   fixedValuePointPatchField) which is then used to evaluate.
//   Example:
//   \verbatim
//     movingWall
//     {
//       type            codedFixedValue;
//       value           uniform 0;
//       redirectType    rampedFixedValue;   // name of generated bc
//       code
//       #{
//         operator==
//         (
//           vector(0,0,1)
//          *min(10, 0.1*this->db().time().value())
//         );
//       #};
//       //codeInclude
//       //#{
//       //    #include "fv_cfd.hpp"
//       //#};
//       //codeOptions
//       //#{
//       //    -I$(LIB_SRC)/finiteVolume/lnInclude
//       //#};
//     }
//   \endverbatim
//   A special form is if the 'code' section is not supplied. In this case
//   the code gets read from a (runTimeModifiable!) dictionary system/codeDict
//   which would have a corresponding entry
//   \verbatim
//   rampedFixedValue
//   {
//     code
//     #{
//       operator==(min(10, 0.1*this->db().time().value()));
//     #};
//   }
//   \endverbatim
// SeeAlso
//   codedFixedValueFvPatchField
// SourceFiles
//   coded_fixed_value_point_patch_field.cpp
#include "fixed_value_point_patch_fields.hpp"
#include "coded_base.hpp"
namespace mousse
{
// Forward declaration of classes
class dynamicCode;
class dynamicCodeContext;
class IOdictionary;
template<class Type>
class codedFixedValuePointPatchField
:
  public fixedValuePointPatchField<Type>,
  public codedBase
{
  // Private data
    //- Dictionary contents for the boundary condition
    mutable dictionary dict_;
    const word redirectType_;
    mutable autoPtr<pointPatchField<Type> > redirectPatchFieldPtr_;
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
    codedFixedValuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    codedFixedValuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&,
      const bool valueRequired=true
    );
    //- Construct by mapping given codedFixedValuePointPatchField
    //  onto a new patch
    codedFixedValuePointPatchField
    (
      const codedFixedValuePointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct as copy
    codedFixedValuePointPatchField
    (
      const codedFixedValuePointPatchField<Type>&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new codedFixedValuePointPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    codedFixedValuePointPatchField
    (
      const codedFixedValuePointPatchField<Type>&,
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
        new codedFixedValuePointPatchField<Type>
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    //- Get reference to the underlying patch
    const pointPatchField<Type>& redirectPatchField() const;
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
#ifdef NoRepository
#   include "coded_fixed_value_point_patch_field.cpp"
#endif
#endif
