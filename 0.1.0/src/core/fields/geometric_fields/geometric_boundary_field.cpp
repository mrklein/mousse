// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "empty_poly_patch.hpp"
#include "comm_schedule.hpp"
#include "global_mesh_data.hpp"
#include "cyclic_poly_patch.hpp"

template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
readField
(
  const DimensionedField<Type, GeoMesh>& field,
  const dictionary& dict
)
{
  // Clear the boundary field if already initialised
  this->clear();
  this->setSize(bmesh_.size());
  if (debug)
  {
    Info<< "GeometricField<Type, PatchField, GeoMesh>::"
      "GeometricBoundaryField::readField"
      "("
      "const DimensionedField<Type, GeoMesh>&,"
      "const dictionary&"
      ")"
      << endl;
  }
  label nUnset = this->size();
  // 1. Handle explicit patch names. Note that there can be only one explicit
  //    patch name since is key of dictionary.
  FOR_ALL_CONST_ITER(dictionary, dict, iter)
  {
    if (iter().isDict() && !iter().keyword().isPattern())
    {
      label patchi = bmesh_.findPatchID(iter().keyword());
      if (patchi != -1)
      {
        this->set
        (
          patchi,
          PatchField<Type>::New
          (
            bmesh_[patchi],
            field,
            iter().dict()
          )
        );
        nUnset--;
      }
    }
  }
  if (nUnset == 0)
  {
    return;
  }
  // 2. Patch-groups. (using non-wild card entries of dictionaries)
  // (patchnames already matched above)
  // Note: in reverse order of entries in the dictionary (last
  // patchGroups wins). This is so is consistent with dictionary wildcard
  // behaviour
  if (dict.size())
  {
    for
    (
      IDLList<entry>::const_reverse_iterator iter = dict.rbegin();
      iter != dict.rend();
      ++iter
    )
    {
      const entry& e = iter();
      if (e.isDict() && !e.keyword().isPattern())
      {
        const labelList patchIDs = bmesh_.findIndices
        (
          e.keyword(),
          true                    // use patchGroups
        );
        FOR_ALL(patchIDs, i)
        {
          label patchi = patchIDs[i];
          if (!this->set(patchi))
          {
            this->set
            (
              patchi,
              PatchField<Type>::New
              (
                bmesh_[patchi],
                field,
                e.dict()
              )
            );
          }
        }
      }
    }
  }
  // 3. Wildcard patch overrides
  FOR_ALL(bmesh_, patchi)
  {
    if (!this->set(patchi))
    {
      if (bmesh_[patchi].type() == emptyPolyPatch::typeName)
      {
        this->set
        (
          patchi,
          PatchField<Type>::New
          (
            emptyPolyPatch::typeName,
            bmesh_[patchi],
            field
          )
        );
      }
      else
      {
        bool found = dict.found(bmesh_[patchi].name());
        if (found)
        {
          this->set
          (
            patchi,
            PatchField<Type>::New
            (
              bmesh_[patchi],
              field,
              dict.subDict(bmesh_[patchi].name())
            )
          );
        }
      }
    }
  }
  // Check for any unset patches
  FOR_ALL(bmesh_, patchi)
  {
    if (!this->set(patchi))
    {
      if (bmesh_[patchi].type() == cyclicPolyPatch::typeName)
      {
        FATAL_IO_ERROR_IN
        (
          "GeometricField<Type, PatchField, GeoMesh>::"
          "GeometricBoundaryField::readField"
          "("
            "const DimensionedField<Type, GeoMesh>&, "
            "const dictionary&"
          ")",
          dict
        )
        << "Cannot find patchField entry for cyclic "
        << bmesh_[patchi].name() << endl
        << "Is your field uptodate with split cyclics?" << endl
        << "Run mousse-ugrade-cyclics to convert mesh and fields"
        << " to split cyclics." << exit(FatalIOError);
      }
      else
      {
        FATAL_IO_ERROR_IN
        (
          "GeometricField<Type, PatchField, GeoMesh>::"
          "GeometricBoundaryField::readField"
          "("
            "const DimensionedField<Type, GeoMesh>&, "
            "const dictionary&"
          ")",
          dict
        )
        << "Cannot find patchField entry for "
        << bmesh_[patchi].name() << exit(FatalIOError);
      }
    }
  }
}


// Constructors 
template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
  const BoundaryMesh& bmesh
)
:
  FieldField<PatchField, Type>{bmesh.size()},
  bmesh_{bmesh}
{}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
  const BoundaryMesh& bmesh,
  const DimensionedField<Type, GeoMesh>& field,
  const word& patchFieldType
)
:
  FieldField<PatchField, Type>{bmesh.size()},
  bmesh_{bmesh}
{
  if (debug)
  {
    Info<< "GeometricField<Type, PatchField, GeoMesh>::"
       "GeometricBoundaryField::"
       "GeometricBoundaryField(const BoundaryMesh&, "
       "const DimensionedField<Type>&, const word&)"
      << endl;
  }
  FOR_ALL(bmesh_, patchi)
  {
    this->set
    (
      patchi,
      PatchField<Type>::New
      (
        patchFieldType,
        bmesh_[patchi],
        field
      )
    );
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
  const BoundaryMesh& bmesh,
  const DimensionedField<Type, GeoMesh>& field,
  const wordList& patchFieldTypes,
  const wordList& constraintTypes
)
:
  FieldField<PatchField, Type>{bmesh.size()},
  bmesh_{bmesh}
{
  if (debug)
  {
    Info<< "GeometricField<Type, PatchField, GeoMesh>::"
       "GeometricBoundaryField::"
       "GeometricBoundaryField"
       "("
          "const BoundaryMesh&, "
          "const DimensionedField<Type>&, "
          "const wordList&, "
          "const wordList&"
        ")"
      << endl;
  }
  if
  (
    patchFieldTypes.size() != this->size()
  || (constraintTypes.size() && (constraintTypes.size() != this->size()))
  )
  {
    FATAL_ERROR_IN
    (
      "GeometricField<Type, PatchField, GeoMesh>::"
      "GeometricBoundaryField::"
      "GeometricBoundaryField"
      "("
        "const BoundaryMesh&, "
        "const DimensionedField<Type>&, "
        "const wordList&, "
        "const wordList&"
      ")"
    )   << "Incorrect number of patch type specifications given" << nl
      << "    Number of patches in mesh = " << bmesh.size()
      << " number of patch type specifications = "
      << patchFieldTypes.size()
      << abort(FatalError);
  }
  if (constraintTypes.size())
  {
    FOR_ALL(bmesh_, patchi)
    {
      this->set
      (
        patchi,
        PatchField<Type>::New
        (
          patchFieldTypes[patchi],
          constraintTypes[patchi],
          bmesh_[patchi],
          field
        )
      );
    }
  }
  else
  {
    FOR_ALL(bmesh_, patchi)
    {
      this->set
      (
        patchi,
        PatchField<Type>::New
        (
          patchFieldTypes[patchi],
          bmesh_[patchi],
          field
        )
      );
    }
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
  const BoundaryMesh& bmesh,
  const DimensionedField<Type, GeoMesh>& field,
  const PtrList<PatchField<Type> >& ptfl
)
:
  FieldField<PatchField, Type>{bmesh.size()},
  bmesh_{bmesh}
{
  if (debug)
  {
    Info<< "GeometricField<Type, PatchField, GeoMesh>::"
       "GeometricBoundaryField::"
       "GeometricBoundaryField"
       "("
          "const BoundaryMesh&, "
          "const DimensionedField<Type, GeoMesh>&, "
          "const PtrLIst<PatchField<Type> >&"
       ")"
      << endl;
  }
  FOR_ALL(bmesh_, patchi)
  {
    this->set(patchi, ptfl[patchi].clone(field));
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
  const DimensionedField<Type, GeoMesh>& field,
  const typename GeometricField<Type, PatchField, GeoMesh>::
  GeometricBoundaryField& btf
)
:
  FieldField<PatchField, Type>{btf.size()},
  bmesh_{btf.bmesh_}
{
  if (debug)
  {
    Info<< "GeometricField<Type, PatchField, GeoMesh>::"
       "GeometricBoundaryField::"
       "GeometricBoundaryField"
       "("
         "const DimensionedField<Type, GeoMesh>&, "
         "const typename GeometricField<Type, PatchField, GeoMesh>::"
         "GeometricBoundaryField&"
       ")"
      << endl;
  }
  FOR_ALL(bmesh_, patchi)
  {
    this->set(patchi, btf[patchi].clone(field));
  }
}


// Construct as copy
// Dangerous because Field may be set to a field which gets deleted.
// Need new type of GeometricBoundaryField, one which IS part of a geometric
// field for which snGrad etc. may be called and a free standing
// GeometricBoundaryField for which such operations are unavailable.
template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
  const typename GeometricField<Type, PatchField, GeoMesh>::
  GeometricBoundaryField& btf
)
:
  FieldField<PatchField, Type>{btf},
  bmesh_{btf.bmesh_}
{
  if (debug)
  {
    Info<< "GeometricField<Type, PatchField, GeoMesh>::"
       "GeometricBoundaryField::"
       "GeometricBoundaryField"
       "("
         "const GeometricField<Type, PatchField, GeoMesh>::"
         "GeometricBoundaryField&"
       ")"
      << endl;
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
  const BoundaryMesh& bmesh,
  const DimensionedField<Type, GeoMesh>& field,
  const dictionary& dict
)
:
  FieldField<PatchField, Type>{bmesh.size()},
  bmesh_{bmesh}
{
  readField(field, dict);
}


// Member Functions 
template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
updateCoeffs()
{
  if (debug)
  {
    Info<< "GeometricField<Type, PatchField, GeoMesh>::"
       "GeometricBoundaryField::"
       "updateCoeffs()" << endl;
  }
  FOR_ALL(*this, patchi)
  {
    this->operator[](patchi).updateCoeffs();
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
evaluate()
{
  if (debug)
  {
    Info<< "GeometricField<Type, PatchField, GeoMesh>::"
       "GeometricBoundaryField::"
       "evaluate()" << endl;
  }
  if
  (
    Pstream::defaultCommsType == Pstream::blocking
  || Pstream::defaultCommsType == Pstream::nonBlocking
  )
  {
    label nReq = Pstream::nRequests();
    FOR_ALL(*this, patchi)
    {
      this->operator[](patchi).initEvaluate(Pstream::defaultCommsType);
    }
    // Block for any outstanding requests
    if (Pstream::parRun()
        && Pstream::defaultCommsType == Pstream::nonBlocking)
    {
      Pstream::waitRequests(nReq);
    }
    FOR_ALL(*this, patchi)
    {
      this->operator[](patchi).evaluate(Pstream::defaultCommsType);
    }
  }
  else if (Pstream::defaultCommsType == Pstream::scheduled)
  {
    const lduSchedule& patchSchedule =
      bmesh_.mesh().globalData().patchSchedule();
    FOR_ALL(patchSchedule, patchEvali)
    {
      if (patchSchedule[patchEvali].init)
      {
        this->operator[](patchSchedule[patchEvali].patch)
          .initEvaluate(Pstream::scheduled);
      }
      else
      {
        this->operator[](patchSchedule[patchEvali].patch)
          .evaluate(Pstream::scheduled);
      }
    }
  }
  else
  {
    FATAL_ERROR_IN("GeometricBoundaryField::evaluate()")
      << "Unsuported communications type "
      << Pstream::commsTypeNames[Pstream::defaultCommsType]
      << exit(FatalError);
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::wordList
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
types() const
{
  const FieldField<PatchField, Type>& pff = *this;
  wordList Types(pff.size());
  FOR_ALL(pff, patchi)
  {
    Types[patchi] = pff[patchi].type();
  }
  return Types;
}


template<class Type, template<class> class PatchField, class GeoMesh>
typename mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
boundaryInternalField() const
{
  typename GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField
    BoundaryInternalField(*this);
  FOR_ALL(BoundaryInternalField, patchi)
  {
    BoundaryInternalField[patchi] ==
      this->operator[](patchi).patchInternalField();
  }
  return BoundaryInternalField;
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::LduInterfaceFieldPtrsList<Type>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
interfaces() const
{
  LduInterfaceFieldPtrsList<Type> interfaces(this->size());
  FOR_ALL(interfaces, patchi)
  {
    if (isA<LduInterfaceField<Type> >(this->operator[](patchi)))
    {
      interfaces.set
      (
        patchi,
        &refCast<const LduInterfaceField<Type>>
        (
          this->operator[](patchi)
        )
      );
    }
  }
  return interfaces;
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::lduInterfaceFieldPtrsList
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
scalarInterfaces() const
{
  lduInterfaceFieldPtrsList interfaces{this->size()};
  FOR_ALL(interfaces, patchi)
  {
    if (isA<lduInterfaceField>(this->operator[](patchi)))
    {
      interfaces.set
      (
        patchi,
        &refCast<const lduInterfaceField>
        (
          this->operator[](patchi)
        )
      );
    }
  }
  return interfaces;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
writeEntry(const word& keyword, Ostream& os) const
{
  os  << keyword << nl << token::BEGIN_BLOCK << incrIndent << nl;
  FOR_ALL(*this, patchi)
  {
    os  << indent << this->operator[](patchi).patch().name() << nl
      << indent << token::BEGIN_BLOCK << nl
      << incrIndent << this->operator[](patchi) << decrIndent
      << indent << token::END_BLOCK << endl;
  }
  os  << decrIndent << token::END_BLOCK << endl;
  // Check state of IOstream
  os.check
  (
    "GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::"
    "writeEntry(const word& keyword, Ostream& os) const"
  );
}


// Member Operators 
template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator=
(
  const typename GeometricField<Type, PatchField, GeoMesh>::
  GeometricBoundaryField& bf
)
{
  FieldField<PatchField, Type>::operator=(bf);
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator=
(
  const FieldField<PatchField, Type>& ptff
)
{
  FieldField<PatchField, Type>::operator=(ptff);
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator=
(
  const Type& t
)
{
  FieldField<PatchField, Type>::operator=(t);
}


// Forced assignments
template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator==
(
  const typename GeometricField<Type, PatchField, GeoMesh>::
  GeometricBoundaryField& bf
)
{
  FOR_ALL((*this), patchI)
  {
    this->operator[](patchI) == bf[patchI];
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator==
(
  const FieldField<PatchField, Type>& ptff
)
{
  FOR_ALL((*this), patchI)
  {
    this->operator[](patchI) == ptff[patchI];
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator==
(
  const Type& t
)
{
  FOR_ALL((*this), patchI)
  {
    this->operator[](patchI) == t;
  }
}


// Friend Operators 
template<class Type, template<class> class PatchField, class GeoMesh>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const typename GeometricField<Type, PatchField, GeoMesh>::
  GeometricBoundaryField& bf
)
{
  os << static_cast<const FieldField<PatchField, Type>&>(bf);
  return os;
}
