// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "outlet_mapped_uniform_inlet_fv_patch_field.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"

// Constructors 
template<class Type>
mousse::outletMappedUniformInletFvPatchField<Type>::
outletMappedUniformInletFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>{p, iF},
  outletPatchName_{},
  phiName_{"phi"}
{}


template<class Type>
mousse::outletMappedUniformInletFvPatchField<Type>::
outletMappedUniformInletFvPatchField
(
  const outletMappedUniformInletFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<Type>{ptf, p, iF, mapper},
  outletPatchName_{ptf.outletPatchName_},
  phiName_{ptf.phiName_}
{}


template<class Type>
mousse::outletMappedUniformInletFvPatchField<Type>::
outletMappedUniformInletFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<Type>{p, iF, dict},
  outletPatchName_{dict.lookup("outletPatchName")},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")}
{}


template<class Type>
mousse::outletMappedUniformInletFvPatchField<Type>::
outletMappedUniformInletFvPatchField
(
  const outletMappedUniformInletFvPatchField<Type>& ptf
)
:
  fixedValueFvPatchField<Type>{ptf},
  outletPatchName_{ptf.outletPatchName_},
  phiName_{ptf.phiName_}
{}


template<class Type>
mousse::outletMappedUniformInletFvPatchField<Type>::
outletMappedUniformInletFvPatchField
(
  const outletMappedUniformInletFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>{ptf, iF},
  outletPatchName_{ptf.outletPatchName_},
  phiName_{ptf.phiName_}
{}


// Member Functions 
template<class Type>
void mousse::outletMappedUniformInletFvPatchField<Type>::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  const GeometricField<Type, fvPatchField, volMesh>& f
  (
    dynamic_cast<const GeometricField<Type, fvPatchField, volMesh>&>
    (
      this->dimensionedInternalField()
    )
  );
  const fvPatch& p = this->patch();
  label outletPatchID =
    p.patch().boundaryMesh().findPatchID(outletPatchName_);
  if (outletPatchID < 0)
  {
    FATAL_ERROR_IN
    (
      "void outletMappedUniformInletFvPatchField<Type>::updateCoeffs()"
    )
    << "Unable to find outlet patch " << outletPatchName_
    << abort(FatalError);
  }
  const fvPatch& outletPatch = p.boundaryMesh()[outletPatchID];
  const fvPatchField<Type>& outletPatchField =
    f.boundaryField()[outletPatchID];
  const surfaceScalarField& phi =
    this->db().objectRegistry::template lookupObject<surfaceScalarField>
    (phiName_);
  const scalarField& outletPatchPhi = phi.boundaryField()[outletPatchID];
  scalar sumOutletPatchPhi = gSum(outletPatchPhi);
  if (sumOutletPatchPhi > SMALL)
  {
    Type averageOutletField =
      gSum(outletPatchPhi*outletPatchField)
     /sumOutletPatchPhi;
    this->operator==(averageOutletField);
  }
  else
  {
    Type averageOutletField =
      gSum(outletPatch.magSf()*outletPatchField)
     /gSum(outletPatch.magSf());
    this->operator==(averageOutletField);
  }
  fixedValueFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void mousse::outletMappedUniformInletFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  os.writeKeyword("outletPatchName")
    << outletPatchName_ << token::END_STATEMENT << nl;
  if (phiName_ != "phi")
  {
    os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
  }
  this->writeEntry("value", os);
}
