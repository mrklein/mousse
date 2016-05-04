// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "geometric_field.hpp"
#include "time.hpp"
#include "demand_driven_data.hpp"
#include "dictionary.hpp"
#include "data.hpp"


#define CHECK_FIELD(gf1, gf2, op)                                             \
if ((gf1).mesh() != (gf2).mesh())                                             \
{                                                                             \
  FATAL_ERROR_IN("CHECK_FIELD(gf1, gf2, op)")                                 \
    << "different mesh for fields "                                           \
    << (gf1).name() << " and " << (gf2).name()                                \
    << " during operatrion " <<  op                                           \
    << abort(FatalError);                                                     \
}

// Private Member Functions
template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::readFields
(
  const dictionary& dict
)
{
  DimensionedField<Type, GeoMesh>::readField(dict, "internalField");
  boundaryField_.readField(*this, dict.subDict("boundaryField"));
  if (dict.found("referenceLevel")) {
    Type fieldAverage{pTraits<Type>(dict.lookup("referenceLevel"))};
    Field<Type>::operator+=(fieldAverage);
    FOR_ALL(boundaryField_, patchi) {
      boundaryField_[patchi] == boundaryField_[patchi] + fieldAverage;
    }
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::readFields()
{
  const IOdictionary dict
  {
    {
      this->name(),
      this->time().timeName(),
      this->db(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    this->readStream(typeName)
  };
  this->close();
  readFields(dict);
}


template<class Type, template<class> class PatchField, class GeoMesh>
bool mousse::GeometricField<Type, PatchField, GeoMesh>::readIfPresent()
{
  if (this->readOpt() == IOobject::MUST_READ
      || this->readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN
    (
      "GeometricField<Type, PatchField, GeoMesh>::readIfPresent()"
    ) << "read option IOobject::MUST_READ or MUST_READ_IF_MODIFIED"
      << " suggests that a read constructor for field " << this->name()
      << " would be more appropriate." << endl;
  } else if (this->readOpt() == IOobject::READ_IF_PRESENT && this->headerOk()) {
    readFields();
    // Check compatibility between field and mesh
    if (this->size() != GeoMesh::size(this->mesh())) {
      FATAL_IO_ERROR_IN
      (
        "GeometricField<Type, PatchField, GeoMesh>::"
        "readIfPresent()",
        this->readStream(typeName)
      )
      << "   number of field elements = " << this->size()
      << " number of mesh elements = "
      << GeoMesh::size(this->mesh())
      << exit(FatalIOError);
    }
    readOldTimeIfPresent();
    return true;
  }
  return false;
}


template<class Type, template<class> class PatchField, class GeoMesh>
bool mousse::GeometricField<Type, PatchField, GeoMesh>::readOldTimeIfPresent()
{
  // Read the old time field if present
  IOobject field0
  {
    this->name()  + "_0",
    this->time().timeName(),
    this->db(),
    IOobject::READ_IF_PRESENT,
    IOobject::AUTO_WRITE,
    this->registerObject()
  };
  if (field0.headerOk()) {
    if (debug) {
      Info << "Reading old time level for field"
          << endl << this->info() << endl;
    }
    field0Ptr_ = new GeometricField<Type, PatchField, GeoMesh>
    {
      field0,
      this->mesh()
    };
    field0Ptr_->timeIndex_ = timeIndex_ - 1;
    if (!field0Ptr_->readOldTimeIfPresent()) {
      field0Ptr_->oldTime();
    }
    return true;
  }
  return false;
}


// Constructors
template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const IOobject& io,
  const Mesh& mesh,
  const dimensionSet& ds,
  const word& patchFieldType
)
:
  DimensionedField<Type, GeoMesh>{io, mesh, ds, false},
  timeIndex_{this->time().timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{mesh.boundary(), *this, patchFieldType}
{
  if (debug) {
    Info << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
            "creating temporary"
        << endl << this->info() << endl;
  }
  readIfPresent();
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const IOobject& io,
  const Mesh& mesh,
  const dimensionSet& ds,
  const wordList& patchFieldTypes,
  const wordList& actualPatchTypes
)
:
  DimensionedField<Type, GeoMesh>{io, mesh, ds, false},
  timeIndex_{this->time().timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{mesh.boundary(), *this, patchFieldTypes, actualPatchTypes}
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
         "creating temporary"
      << endl << this->info() << endl;
  }
  readIfPresent();
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const IOobject& io,
  const Mesh& mesh,
  const dimensioned<Type>& dt,
  const word& patchFieldType
)
:
  DimensionedField<Type, GeoMesh>{io, mesh, dt, false},
  timeIndex_{this->time().timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{mesh.boundary(), *this, patchFieldType}
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
         "creating temporary"
      << endl << this->info() << endl;
  }
  boundaryField_ == dt.value();
  readIfPresent();
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const IOobject& io,
  const Mesh& mesh,
  const dimensioned<Type>& dt,
  const wordList& patchFieldTypes,
  const wordList& actualPatchTypes
)
:
  DimensionedField<Type, GeoMesh>{io, mesh, dt, false},
  timeIndex_{this->time().timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{mesh.boundary(), *this, patchFieldTypes, actualPatchTypes}
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
         "creating temporary"
      << endl << this->info() << endl;
  }
  boundaryField_ == dt.value();
  readIfPresent();
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const IOobject& io,
  const Mesh& mesh,
  const dimensionSet& ds,
  const Field<Type>& iField,
  const PtrList<PatchField<Type>>& ptfl
)
:
  DimensionedField<Type, GeoMesh>{io, mesh, ds, iField},
  timeIndex_{this->time().timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{mesh.boundary(), *this, ptfl}
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
      "constructing from components"
      << endl << this->info() << endl;
  }
  readIfPresent();
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const IOobject& io,
  const Mesh& mesh
)
:
  DimensionedField<Type, GeoMesh>{io, mesh, dimless, false},
  timeIndex_{this->time().timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{mesh.boundary()}
{
  readFields();
  // Check compatibility between field and mesh
  if (this->size() != GeoMesh::size(this->mesh())) {
    FATAL_IO_ERROR_IN
    (
      "GeometricField<Type, PatchField, GeoMesh>::GeometricField"
      "(const IOobject&, const Mesh&)",
      this->readStream(typeName)
    )
    << "   number of field elements = " << this->size()
    << " number of mesh elements = " << GeoMesh::size(this->mesh())
    << exit(FatalIOError);
  }
  readOldTimeIfPresent();
  if (debug) {
    Info
      << "Finishing read-construct of "
      "GeometricField<Type, PatchField, GeoMesh>"
      << endl << this->info() << endl;
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const IOobject& io,
  const Mesh& mesh,
  const dictionary& dict
)
:
  DimensionedField<Type, GeoMesh>{io, mesh, dimless, false},
  timeIndex_{this->time().timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{mesh.boundary()}
{
  readFields(dict);
  // Check compatibility between field and mesh
  if (this->size() != GeoMesh::size(this->mesh())) {
    FATAL_ERROR_IN
    (
      "GeometricField<Type, PatchField, GeoMesh>::GeometricField"
      "(const IOobject&, const Mesh&, const dictionary&)"
    )
    << "   number of field elements = " << this->size()
    << " number of mesh elements = " << GeoMesh::size(this->mesh())
    << exit(FatalIOError);
  }
  readOldTimeIfPresent();
  if (debug) {
    Info
      << "Finishing dictionary-construct of "
      "GeometricField<Type, PatchField, GeoMesh>"
      << endl << this->info() << endl;
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const GeometricField<Type, PatchField, GeoMesh>& gf
)
:
  DimensionedField<Type, GeoMesh>{gf},
  timeIndex_{gf.timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{*this, gf.boundaryField_}
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
      "constructing as copy"
      << endl << this->info() << endl;
  }
  if (gf.field0Ptr_ != nullptr) {
    field0Ptr_ = new GeometricField<Type, PatchField, GeoMesh>{*gf.field0Ptr_};
  }
  this->writeOpt() = IOobject::NO_WRITE;
}


#ifndef NoConstructFromTmp

template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const tmp<GeometricField<Type, PatchField, GeoMesh>>& tgf
)
:
  DimensionedField<Type, GeoMesh>
  {
    const_cast<GeometricField<Type, PatchField, GeoMesh>&>(tgf()),
    tgf.isTmp()
  },
  timeIndex_{tgf().timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{*this, tgf().boundaryField_}
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
      "constructing as copy"
      << endl << this->info() << endl;
  }
  this->writeOpt() = IOobject::NO_WRITE;
  tgf.clear();
}

#endif


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const IOobject& io,
  const GeometricField<Type, PatchField, GeoMesh>& gf
)
:
  DimensionedField<Type, GeoMesh>{io, gf},
  timeIndex_{gf.timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{*this, gf.boundaryField_}
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
         "constructing as copy resetting IO params"
      << endl << this->info() << endl;
  }
  if (!readIfPresent() && gf.field0Ptr_ != nullptr) {
    field0Ptr_ =
      new GeometricField<Type, PatchField, GeoMesh>
      {
        io.name() + "_0",
        *gf.field0Ptr_
      };
  }
}


#ifndef NoConstructFromTmp
template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const IOobject& io,
  const tmp<GeometricField<Type, PatchField, GeoMesh>>& tgf
)
:
  DimensionedField<Type, GeoMesh>
  {
    io,
    const_cast<GeometricField<Type, PatchField, GeoMesh>&>(tgf()),
    tgf.isTmp()
  },
  timeIndex_{tgf().timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{*this, tgf().boundaryField_}
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
      "constructing from tmp resetting IO params"
      << endl << this->info() << endl;
  }
  tgf.clear();
  readIfPresent();
}
#endif


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const word& newName,
  const GeometricField<Type, PatchField, GeoMesh>& gf
)
:
  DimensionedField<Type, GeoMesh>{newName, gf},
  timeIndex_{gf.timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{*this, gf.boundaryField_}
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
      "constructing as copy resetting name"
      << endl << this->info() << endl;
  }
  if (!readIfPresent() && gf.field0Ptr_ != nullptr) {
    field0Ptr_ =
      new GeometricField<Type, PatchField, GeoMesh>
      {
        newName + "_0",
        *gf.field0Ptr_
      };
  }
}


#ifndef NoConstructFromTmp
template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const word& newName,
  const tmp<GeometricField<Type, PatchField, GeoMesh>>& tgf
)
:
  DimensionedField<Type, GeoMesh>
  {
    newName,
    const_cast<GeometricField<Type, PatchField, GeoMesh>&>(tgf()),
    tgf.isTmp()
  },
  timeIndex_{tgf().timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{*this, tgf().boundaryField_}
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
      "constructing from tmp resetting name"
      << endl << this->info() << endl;
  }
  tgf.clear();
}
#endif


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const IOobject& io,
  const GeometricField<Type, PatchField, GeoMesh>& gf,
  const word& patchFieldType
)
:
  DimensionedField<Type, GeoMesh>{io, gf},
  timeIndex_{gf.timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_{this->mesh().boundary(), *this, patchFieldType}
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
      "constructing as copy resetting IO params"
      << endl << this->info() << endl;
  }
  boundaryField_ == gf.boundaryField_;
  if (!readIfPresent() && gf.field0Ptr_ != nullptr) {
    field0Ptr_ =
      new GeometricField<Type, PatchField, GeoMesh>
      {
        io.name() + "_0",
        *gf.field0Ptr_
      };
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
  const IOobject& io,
  const GeometricField<Type, PatchField, GeoMesh>& gf,
  const wordList& patchFieldTypes,
  const wordList& actualPatchTypes
)
:
  DimensionedField<Type, GeoMesh>{io, gf},
  timeIndex_{gf.timeIndex()},
  field0Ptr_{nullptr},
  fieldPrevIterPtr_{nullptr},
  boundaryField_
  {
    this->mesh().boundary(),
    *this,
    patchFieldTypes,
    actualPatchTypes
  }
{
  if (debug) {
    Info
      << "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
         "constructing as copy resetting IO params and patch types"
      << endl << this->info() << endl;
  }
  boundaryField_ == gf.boundaryField_;
  if (!readIfPresent() && gf.field0Ptr_ != nullptr) {
    field0Ptr_ =
      new GeometricField<Type, PatchField, GeoMesh>
      {
        io.name() + "_0",
        *gf.field0Ptr_
      };
  }
}


// Destructor
template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>::~GeometricField()
{
  deleteDemandDrivenData(field0Ptr_);
  deleteDemandDrivenData(fieldPrevIterPtr_);
}


// Member Functions
template<class Type, template<class> class PatchField, class GeoMesh>
typename
mousse::GeometricField<Type, PatchField, GeoMesh>::DimensionedInternalField&
mousse::GeometricField<Type, PatchField, GeoMesh>::dimensionedInternalField()
{
  this->setUpToDate();
  storeOldTimes();
  return *this;
}


template<class Type, template<class> class PatchField, class GeoMesh>
typename
mousse::GeometricField<Type, PatchField, GeoMesh>::InternalField&
mousse::GeometricField<Type, PatchField, GeoMesh>::internalField()
{
  this->setUpToDate();
  storeOldTimes();
  return *this;
}


template<class Type, template<class> class PatchField, class GeoMesh>
typename
mousse::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField&
mousse::GeometricField<Type, PatchField, GeoMesh>::boundaryField()
{
  this->setUpToDate();
  storeOldTimes();
  return boundaryField_;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::storeOldTimes() const
{
  if (field0Ptr_ != nullptr && timeIndex_ != this->time().timeIndex()
      && !(this->name().size() > 2
           && this->name()(this->name().size()-2, 2) == "_0")) {
    storeOldTime();
  }
  // Correct time index
  timeIndex_ = this->time().timeIndex();
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::storeOldTime() const
{
  if (field0Ptr_ != nullptr) {
    field0Ptr_->storeOldTime();
    if (debug) {
      Info
        << "Storing old time field for field" << endl
        << this->info() << endl;
    }
    *field0Ptr_ == *this;
    field0Ptr_->timeIndex_ = timeIndex_;
    if (field0Ptr_->field0Ptr_ != nullptr) {
      field0Ptr_->writeOpt() = this->writeOpt();
    }
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::label mousse::GeometricField<Type, PatchField, GeoMesh>::nOldTimes() const
{
  if (field0Ptr_ != nullptr) {
    return field0Ptr_->nOldTimes() + 1;
  } else {
    return 0;
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
const mousse::GeometricField<Type, PatchField, GeoMesh>&
mousse::GeometricField<Type, PatchField, GeoMesh>::oldTime() const
{
  if (field0Ptr_ == nullptr) {
    field0Ptr_ =
      new GeometricField<Type, PatchField, GeoMesh>
      {
        IOobject
        {
          this->name() + "_0",
          this->time().timeName(),
          this->db(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          this->registerObject()
        },
        *this
      };
  } else {
    storeOldTimes();
  }
  return *field0Ptr_;
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::GeometricField<Type, PatchField, GeoMesh>&
mousse::GeometricField<Type, PatchField, GeoMesh>::oldTime()
{
  static_cast<const GeometricField<Type, PatchField, GeoMesh>&>(*this).oldTime();
  return *field0Ptr_;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::storePrevIter() const
{
  if (fieldPrevIterPtr_ == nullptr) {
    if (debug) {
      Info
        << "Allocating previous iteration field" << endl
        << this->info() << endl;
    }
    fieldPrevIterPtr_ =
      new GeometricField<Type, PatchField, GeoMesh>
      {
        this->name() + "PrevIter",
        *this
      };
  } else {
    *fieldPrevIterPtr_ == *this;
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
const mousse::GeometricField<Type, PatchField, GeoMesh>&
mousse::GeometricField<Type, PatchField, GeoMesh>::prevIter() const
{
  if (fieldPrevIterPtr_ == nullptr) {
    FATAL_ERROR_IN
    (
      "GeometricField<Type, PatchField, GeoMesh>::prevIter() const"
    )
    << "previous iteration field" << endl << this->info() << endl
    << "  not stored."
    << "  Use field.storePrevIter() at start of iteration."
    << abort(FatalError);
  }
  return *fieldPrevIterPtr_;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::
correctBoundaryConditions()
{
  this->setUpToDate();
  storeOldTimes();
  boundaryField_.evaluate();
}


template<class Type, template<class> class PatchField, class GeoMesh>
bool mousse::GeometricField<Type, PatchField, GeoMesh>::needReference() const
{
  // Search all boundary conditions, if any are
  // fixed-value or mixed (Robin) do not set reference level for solution.
  bool needRef = true;
  FOR_ALL(boundaryField_, patchi) {
    if (boundaryField_[patchi].fixesValue()) {
      needRef = false;
      break;
    }
  }
  reduce(needRef, andOp<bool>());
  return needRef;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::relax(const scalar alpha)
{
  if (debug) {
    INFO_IN
    (
      "GeometricField<Type, PatchField, GeoMesh>::relax"
      "(const scalar alpha)"
    )
    << "Relaxing" << endl << this->info() << " by " << alpha << endl;
  }
  operator==(prevIter() + alpha*(*this - prevIter()));
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::relax()
{
  word name = this->name();
  if (this->mesh().data::template lookupOrDefault<bool>
      (
        "finalIteration",
        false
      )) {
    name += "Final";
  }
  if (this->mesh().relaxField(name)) {
    relax(this->mesh().fieldRelaxationFactor(name));
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::word mousse::GeometricField<Type, PatchField, GeoMesh>::select
(
  bool final
) const
{
  if (final) {
    return this->name() + "Final";
  } else {
    return this->name();
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::writeMinMax
(
  Ostream& os
) const
{
  os << "min/max(" << this->name() << ") = "
    << mousse::min(*this).value() << ", "
    << mousse::max(*this).value()
    << endl;
}


template<class Type, template<class> class PatchField, class GeoMesh>
bool mousse::GeometricField<Type, PatchField, GeoMesh>::
writeData(Ostream& os) const
{
  os << *this;
  return os.good();
}


// Member Functions
template<class Type, template<class> class PatchField, class GeoMesh>
mousse::tmp<mousse::GeometricField<Type, PatchField, GeoMesh>>
mousse::GeometricField<Type, PatchField, GeoMesh>::T() const
{
  tmp<GeometricField<Type, PatchField, GeoMesh>> result
  {
    new GeometricField<Type, PatchField, GeoMesh>
    {
      {
        this->name() + ".T()",
        this->instance(),
        this->db()
      },
      this->mesh(),
      this->dimensions()
    }
  };
  mousse::T(result().internalField(), internalField());
  mousse::T(result().boundaryField(), boundaryField());
  return result;
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::tmp
<
  mousse::GeometricField
  <
    typename mousse::GeometricField<Type, PatchField, GeoMesh>::cmptType,
    PatchField,
    GeoMesh
  >
>
mousse::GeometricField<Type, PatchField, GeoMesh>::component
(
  const direction d
) const
{
  tmp<GeometricField<cmptType, PatchField, GeoMesh>> Component
  {
    new GeometricField<cmptType, PatchField, GeoMesh>
    {
      {
        this->name() + ".component(" + mousse::name(d) + ')',
        this->instance(),
        this->db()
      },
      this->mesh(),
      this->dimensions()
    }
  };
  mousse::component(Component().internalField(), internalField(), d);
  mousse::component(Component().boundaryField(), boundaryField(), d);
  return Component;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::replace
(
  const direction d,
  const GeometricField
  <
    typename GeometricField<Type, PatchField, GeoMesh>::cmptType,
    PatchField,
    GeoMesh
  >& gcf
)
{
  internalField().replace(d, gcf.internalField());
  boundaryField().replace(d, gcf.boundaryField());
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::replace
(
  const direction d,
  const dimensioned<cmptType>& ds
)
{
  internalField().replace(d, ds.value());
  boundaryField().replace(d, ds.value());
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::max
(
  const dimensioned<Type>& dt
)
{
  mousse::max(internalField(), internalField(), dt.value());
  mousse::max(boundaryField(), boundaryField(), dt.value());
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::min
(
  const dimensioned<Type>& dt
)
{
  mousse::min(internalField(), internalField(), dt.value());
  mousse::min(boundaryField(), boundaryField(), dt.value());
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::negate()
{
  internalField().negate();
  boundaryField().negate();
}


// Member Operators
template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::operator=
(
  const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
  if (this == &gf) {
    FATAL_ERROR_IN
    (
      "GeometricField<Type, PatchField, GeoMesh>::operator="
      "(const GeometricField<Type, PatchField, GeoMesh>&)"
    )
    << "attempted assignment to self"
    << abort(FatalError);
  }
  CHECK_FIELD(*this, gf, "=");
  // only equate field contents not ID
  dimensionedInternalField() = gf.dimensionedInternalField();
  boundaryField() = gf.boundaryField();
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::operator=
(
  const tmp<GeometricField<Type, PatchField, GeoMesh>>& tgf
)
{
  if (this == &(tgf())) {
    FATAL_ERROR_IN
    (
      "GeometricField<Type, PatchField, GeoMesh>::operator="
      "(const tmp<GeometricField<Type, PatchField, GeoMesh>>&)"
    )
    << "attempted assignment to self"
    << abort(FatalError);
  }
  const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();
  CHECK_FIELD(*this, gf, "=");
  // only equate field contents not ID
  this->dimensions() = gf.dimensions();
  // This is dodgy stuff, don't try it at home.
  internalField().transfer
  (
    const_cast<Field<Type>&>(gf.internalField())
  );
  boundaryField() = gf.boundaryField();
  tgf.clear();
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::operator=
(
  const dimensioned<Type>& dt
)
{
  dimensionedInternalField() = dt;
  boundaryField() = dt.value();
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::operator==
(
  const tmp<GeometricField<Type, PatchField, GeoMesh>>& tgf
)
{
  const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();
  CHECK_FIELD(*this, gf, "==");
  // only equate field contents not ID
  dimensionedInternalField() = gf.dimensionedInternalField();
  boundaryField() == gf.boundaryField();
  tgf.clear();
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::GeometricField<Type, PatchField, GeoMesh>::operator==
(
  const dimensioned<Type>& dt
)
{
  dimensionedInternalField() = dt;
  boundaryField() == dt.value();
}


#define COMPUTED_ASSIGNMENT(TYPE, op)                                         \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void mousse::GeometricField<Type, PatchField, GeoMesh>::operator op           \
(                                                                             \
  const GeometricField<TYPE, PatchField, GeoMesh>& gf                         \
)                                                                             \
{                                                                             \
  CHECK_FIELD(*this, gf, #op);                                                \
                                                                              \
  dimensionedInternalField() op gf.dimensionedInternalField();                \
  boundaryField() op gf.boundaryField();                                      \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void mousse::GeometricField<Type, PatchField, GeoMesh>::operator op           \
(                                                                             \
  const tmp<GeometricField<TYPE, PatchField, GeoMesh>>& tgf                   \
)                                                                             \
{                                                                             \
  operator op(tgf());                                                         \
  tgf.clear();                                                                \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void mousse::GeometricField<Type, PatchField, GeoMesh>::operator op           \
(                                                                             \
  const dimensioned<TYPE>& dt                                                 \
)                                                                             \
{                                                                             \
  dimensionedInternalField() op dt;                                           \
  boundaryField() op dt.value();                                              \
}

COMPUTED_ASSIGNMENT(Type, +=)
COMPUTED_ASSIGNMENT(Type, -=)
COMPUTED_ASSIGNMENT(scalar, *=)
COMPUTED_ASSIGNMENT(scalar, /=)

#undef COMPUTED_ASSIGNMENT


// IOstream Operators
template<class Type, template<class> class PatchField, class GeoMesh>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
  gf.dimensionedInternalField().writeData(os, "internalField");
  os << nl;
  gf.boundaryField().writeEntry("boundaryField", os);
  // Check state of IOstream
  os.check
  (
    "Ostream& operator<<(Ostream&, "
    "const GeometricField<Type, PatchField, GeoMesh>&)"
  );
  return (os);
}


template<class Type, template<class> class PatchField, class GeoMesh>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const tmp<GeometricField<Type, PatchField, GeoMesh>>& tgf
)
{
  os << tgf();
  tgf.clear();
  return os;
}

#undef CHECK_FIELD

#include "geometric_boundary_field.ipp"
#include "geometric_field_functions.ipp"
