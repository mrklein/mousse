#ifndef FV_MOTION_SOLVER_FV_PATCH_FIELDS_DERIVED_SURFACE_SLIP_DISPLACEMENT_SURFACE_SLIP_DISPLACEMENT_FV_PATCH_FIELD_HPP_
#define FV_MOTION_SOLVER_FV_PATCH_FIELDS_DERIVED_SURFACE_SLIP_DISPLACEMENT_SURFACE_SLIP_DISPLACEMENT_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceSlipDisplacementFvPatchField
// Description
//   fvPatchField corresponding to surfaceSlipDisplacementPointPatchField.
//   Is just a slip type since all hard work (projection) is done in the
//   pointPatch field.

#include "slip_fv_patch_field.hpp"


namespace mousse {

template<class Type>
class surfaceSlipDisplacementFvPatchField
:
  public slipFvPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("surfaceSlipDisplacement");
  // Constructors
    //- Construct from patch and internal field
    surfaceSlipDisplacementFvPatchField
    (
      const fvPatch& p,
      const DimensionedField<Type, volMesh>& iF
    )
    :
      slipFvPatchField<Type>{p, iF}
    {}
    //- Construct from patch, internal field and dictionary
    surfaceSlipDisplacementFvPatchField
    (
      const fvPatch& p,
      const DimensionedField<Type, volMesh>& iF,
      const dictionary& dict
    )
    :
      slipFvPatchField<Type>{p, iF, dict}
    {}
    //- Construct by mapping given surfaceSlipDisplacementFvPatchField
    //  onto a new patch
    surfaceSlipDisplacementFvPatchField
    (
      const surfaceSlipDisplacementFvPatchField<Type>& ptf,
      const fvPatch& p,
      const DimensionedField<Type, volMesh>& iF,
      const fvPatchFieldMapper& mapper
    )
    :
      slipFvPatchField<Type>{ptf, p, iF, mapper}
    {}
    //- Construct as copy
    surfaceSlipDisplacementFvPatchField
    (
      const surfaceSlipDisplacementFvPatchField<Type>& ptf
    )
    :
      slipFvPatchField<Type>{ptf}
    {}
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return
        tmp<fvPatchField<Type>>
        {
          new surfaceSlipDisplacementFvPatchField<Type>{*this}
        };
    }
    //- Construct as copy setting internal field reference
    surfaceSlipDisplacementFvPatchField
    (
      const surfaceSlipDisplacementFvPatchField<Type>& ptf,
      const DimensionedField<Type, volMesh>& iF
    )
    :
      slipFvPatchField<Type>{ptf, iF}
    {}
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type>> clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return
        tmp<fvPatchField<Type>>
        {
          new surfaceSlipDisplacementFvPatchField<Type>{*this, iF}
        };
    }
    virtual void write(Ostream& os) const
    {
      slipFvPatchField<Type>::write(os);
      // Only so we can easily postprocess.
      this->writeEntry("value", os);
    }
};

}  // namespace mousse

#endif

