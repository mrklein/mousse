// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_patch_field.hpp"
#include "point_mesh.hpp"
#include "dictionary.hpp"


namespace mousse {

// Constructors 
template<class Type>
pointPatchField<Type>::pointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  patch_{p},
  internalField_{iF},
  updated_{false},
  patchType_{word::null}
{}


template<class Type>
pointPatchField<Type>::pointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  patch_{p},
  internalField_{iF},
  updated_{false},
  patchType_{dict.lookupOrDefault<word>("patchType", word::null)}
{}


template<class Type>
mousse::pointPatchField<Type>::pointPatchField
(
  const pointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper&
)
:
  patch_{p},
  internalField_{iF},
  updated_{false},
  patchType_{ptf.patchType_}
{}


template<class Type>
pointPatchField<Type>::pointPatchField
(
  const pointPatchField<Type>& ptf
)
:
  patch_{ptf.patch_},
  internalField_{ptf.internalField_},
  updated_{false},
  patchType_{ptf.patchType_}
{}


template<class Type>
pointPatchField<Type>::pointPatchField
(
  const pointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  patch_{ptf.patch_},
  internalField_{iF},
  updated_{false},
  patchType_{ptf.patchType_}
{}


// Member Functions 
template<class Type>
const objectRegistry& pointPatchField<Type>::db() const
{
  return patch_.boundaryMesh().mesh()();
}


template<class Type>
void pointPatchField<Type>::write(Ostream& os) const
{
  os.writeKeyword("type") << type() << token::END_STATEMENT << nl;
  if (patchType_.size()) {
    os.writeKeyword("patchType") << patchType_ << token::END_STATEMENT << nl;
  }
}


template<class Type>
tmp<Field<Type> > pointPatchField<Type>::patchInternalField() const
{
  return patchInternalField(internalField());
}


template<class Type>
template<class Type1>
tmp<Field<Type1> > pointPatchField<Type>::patchInternalField
(
  const Field<Type1>& iF,
  const labelList& meshPoints
) const
{
  // Check size
  if (iF.size() != internalField().size()) {
    FATAL_ERROR_IN
    (
      "tmp<Field<Type1> > pointPatchField<"
      "Type>::"
      "patchInternalField(const Field<Type1>& iF) const"
    )
    << "given internal field does not correspond to the mesh. "
    << "Field size: " << iF.size()
    << " mesh size: " << internalField().size()
    << abort(FatalError);
  }
  return tmp<Field<Type1>>{new Field<Type1>{iF, meshPoints}};
}


template<class Type>
template<class Type1>
tmp<Field<Type1> > pointPatchField<Type>::patchInternalField
(
  const Field<Type1>& iF
) const
{
  return patchInternalField(iF, patch().meshPoints());
}


template<class Type>
template<class Type1>
void pointPatchField<Type>::addToInternalField
(
  Field<Type1>& iF,
  const Field<Type1>& pF
) const
{
  // Check size
  if (iF.size() != internalField().size()) {
    FATAL_ERROR_IN
    (
      "void pointPatchField<Type>::"
      "addToInternalField("
      "Field<Type1>& iF, const Field<Type1>& iF) const"
    )
    << "given internal field does not correspond to the mesh. "
    << "Field size: " << iF.size()
    << " mesh size: " << internalField().size()
    << abort(FatalError);
  }
  if (pF.size() != size()) {
    FATAL_ERROR_IN
    (
      "void pointPatchField<Type>::"
      "addToInternalField("
      "Field<Type1>& iF, const Field<Type1>& iF) const"
    )
    << "given patch field does not correspond to the mesh. "
    << "Field size: " << pF.size()
    << " mesh size: " << size()
    << abort(FatalError);
  }
  // Get the addressing
  const labelList& mp = patch().meshPoints();
  FOR_ALL(mp, pointI) {
    iF[mp[pointI]] += pF[pointI];
  }
}


template<class Type>
template<class Type1>
void pointPatchField<Type>::addToInternalField
(
  Field<Type1>& iF,
  const Field<Type1>& pF,
  const labelList& points
) const
{
  // Check size
  if (iF.size() != internalField().size()) {
    FATAL_ERROR_IN
    (
      "void pointPatchField<Type>::"
      "addToInternalField("
      "Field<Type1>& iF, const Field<Type1>& iF, const labelList&) const"
    )
    << "given internal field does not correspond to the mesh. "
    << "Field size: " << iF.size()
    << " mesh size: " << internalField().size()
    << abort(FatalError);
  }
  if (pF.size() != size()) {
    FATAL_ERROR_IN
    (
      "void pointPatchField<Type>::"
      "addToInternalField("
      "Field<Type1>& iF, const Field<Type1>& iF, const labelList&) const"
    )
    << "given patch field does not correspond to the mesh. "
    << "Field size: " << pF.size()
    << " mesh size: " << size()
    << abort(FatalError);
  }
  // Get the addressing
  const labelList& mp = patch().meshPoints();
  FOR_ALL(points, i) {
    label pointI = points[i];
    iF[mp[pointI]] += pF[pointI];
  }
}


template<class Type>
template<class Type1>
void pointPatchField<Type>::setInInternalField
(
  Field<Type1>& iF,
  const Field<Type1>& pF,
  const labelList& meshPoints
) const
{
  // Check size
  if (iF.size() != internalField().size()) {
    FATAL_ERROR_IN
    (
      "void pointPatchField<Type>::"
      "setInInternalField("
      "Field<Type1>& iF, const Field<Type1>& iF) const"
    )
    << "given internal field does not correspond to the mesh. "
    << "Field size: " << iF.size()
    << " mesh size: " << internalField().size()
    << abort(FatalError);
  }
  if (pF.size() != meshPoints.size()) {
    FATAL_ERROR_IN
    (
      "void pointPatchField<Type>::"
      "setInInternalField("
      "Field<Type1>& iF, const Field<Type1>& iF) const"
    )
    << "given patch field does not correspond to the meshPoints. "
    << "Field size: " << pF.size()
    << " meshPoints size: " << size()
    << abort(FatalError);
  }
  FOR_ALL(meshPoints, pointI) {
    iF[meshPoints[pointI]] = pF[pointI];
  }
}


template<class Type>
template<class Type1>
void pointPatchField<Type>::setInInternalField
(
  Field<Type1>& iF,
  const Field<Type1>& pF
) const
{
  setInInternalField(iF, pF, patch().meshPoints());
}


template<class Type>
void pointPatchField<Type>::evaluate(const Pstream::commsTypes)
{
  if (!updated_) {
    updateCoeffs();
  }
  updated_ = false;
}


// IOstream Operators 
template<class Type>
Ostream& operator<<
(
  Ostream& os,
  const pointPatchField<Type>& ptf
)
{
  ptf.write(os);
  os.check("Ostream& operator<<(Ostream&, const pointPatchField<Type>&)");
  return os;
}

}  // namespace mousse

#include "point_patch_field_new.ipp"
