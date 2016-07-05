#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_BASIC_SLICED_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_BASIC_SLICED_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::slicedFvPatchField
// Group
//   grpGenericBoundaryConditions
// Description
//   Specialization of fvPatchField which creates the underlying
//   fvPatchField as a slice of the given complete field.
//   The destructor is wrapped to avoid deallocation of the storage of the
//   complete fields when this is destroyed.
//   Should only used as a template argument for SlicedGeometricField.
// SeeAlso
//   mousse::fvPatchField

#include "fv_patch_field.hpp"


namespace mousse {

template<class Type>
class slicedFvPatchField
:
  public fvPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("sliced");
  // Constructors
    //- Construct from patch, internal field and field to slice
    slicedFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const Field<Type>&
    );
    //- Construct from patch and internal field. Assign value later.
    slicedFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    slicedFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping the given slicedFvPatchField<Type>
    //  onto a new patch
    slicedFvPatchField
    (
      const slicedFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    slicedFvPatchField(const slicedFvPatchField<Type>&);
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const;
    //- Construct as copy setting internal field reference
    slicedFvPatchField
    (
      const slicedFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type>> clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const;
  //- Destructor
  virtual ~slicedFvPatchField<Type>();
  // Member functions
    // Access
      //- Return true if this patch field fixes a value.
      //  Needed to check if a level has to be specified while solving
      //  Poissons equations.
      virtual bool fixesValue() const
      {
        return true;
      }
    // Evaluation functions
      //- Return patch-normal gradient
      virtual tmp<Field<Type>> snGrad() const;
      //- Update the coefficients associated with the patch field
      //  Sets Updated to true
      virtual void updateCoeffs();
      //- Return internal field next to patch as patch field
      virtual tmp<Field<Type>> patchInternalField() const;
      //- Return internal field next to patch as patch field
      virtual void patchInternalField(Field<Type>&) const;
      //- Return neighbour coupled given internal cell data
      virtual tmp<Field<Type>> patchNeighbourField
      (
        const Field<Type>& iField
      ) const;
      //- Return patchField of the values on the patch or on the
      //  opposite patch
      virtual tmp<Field<Type>> patchNeighbourField() const;
      //- Initialise the evaluation of the patch field
      virtual void initEvaluate
      (
        const Pstream::commsTypes=Pstream::blocking
      )
      {}
      //- Evaluate the patch field, sets Updated to false
      virtual void evaluate
      (
        const Pstream::commsTypes=Pstream::blocking
      )
      {}
      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type>> valueInternalCoeffs
      (
        const tmp<scalarField>&
      ) const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type>> valueBoundaryCoeffs
      (
        const tmp<scalarField>&
      ) const;
      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type>> gradientInternalCoeffs() const;
      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type>> gradientBoundaryCoeffs() const;
    //- Write
    virtual void write(Ostream&) const;
  // Member operators
    virtual void operator=(const UList<Type>&) {}
    virtual void operator=(const fvPatchField<Type>&) {}
    virtual void operator+=(const fvPatchField<Type>&) {}
    virtual void operator-=(const fvPatchField<Type>&) {}
    virtual void operator*=(const fvPatchField<scalar>&) {}
    virtual void operator/=(const fvPatchField<scalar>&) {}
    virtual void operator+=(const Field<Type>&) {}
    virtual void operator-=(const Field<Type>&) {}
    virtual void operator*=(const Field<scalar>&) {}
    virtual void operator/=(const Field<scalar>&) {}
    virtual void operator=(const Type&) {}
    virtual void operator+=(const Type&) {}
    virtual void operator-=(const Type&) {}
    virtual void operator*=(const scalar) {}
    virtual void operator/=(const scalar) {}
};
}  // namespace mousse

#include "sliced_fv_patch_field.ipp"

#endif
