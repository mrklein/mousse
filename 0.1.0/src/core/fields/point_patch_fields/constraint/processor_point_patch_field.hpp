// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorPointPatchField
// Description
//   mousse::processorPointPatchField
// SourceFiles
//   processor_point_patch_field.cpp
#ifndef processor_point_patch_field_hpp_
#define processor_point_patch_field_hpp_
#include "coupled_point_patch_field.hpp"
#include "processor_point_patch.hpp"
namespace mousse
{
template<class Type>
class processorPointPatchField
:
  public coupledPointPatchField<Type>
{
  // Private data
    //- Local reference to processor patch
    const processorPointPatch& procPatch_;
public:
  //- Runtime type information
  TYPE_NAME(processorPointPatch::typeName_());
  // Constructors
    //- Construct from patch and internal field
    processorPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    processorPointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    processorPointPatchField
    (
      const processorPointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new processorPointPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    processorPointPatchField
    (
      const processorPointPatchField<Type>&,
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
        new processorPointPatchField<Type>
        (
          *this,
          iF
        )
      );
    }
  //- Destructor
  virtual ~processorPointPatchField();
  // Member functions
    // Access
      //- Return true if running parallel
      virtual bool coupled() const
      {
        if (Pstream::parRun())
        {
          return true;
        }
        else
        {
          return false;
        }
      }
      //- Does the patch field perform the transformation
      virtual bool doTransform() const
      {
        return
         !(
            procPatch_.procPolyPatch().parallel()
          || pTraits<Type>::rank == 0
          );
      }
    //- Constraint handling
      //- Return the constraint type this pointPatchField implements
      virtual const word& constraintType() const
      {
        return type();
      }
    // Evaluation functions
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes /*commsType*/=Pstream::blocking
      )
      {}
      //- Assume processor patch always collocated
      virtual void swapAddSeparated
      (
        const Pstream::commsTypes,
        Field<Type>&
      ) const
      {}
};
}  // namespace mousse
#ifdef NoRepository
#    include "processor_point_patch_field.cpp"
#endif
#endif
