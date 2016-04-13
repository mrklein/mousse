// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "probes.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "iomanip.hpp"
#include "interpolation.hpp"


namespace mousse {

//- Comparison operator for probes class
template<class T>
class isNotEqOp
{
public:
  void operator()(T& x, const T& y) const
  {
    const T unsetVal{-VGREAT*pTraits<T>::one};
    if (x != unsetVal) {
      // Keep x.
      // Note:chould check for y != unsetVal but multiple sample cells
      // already handled in read().
    } else {
      // x is not set. y might be.
      x = y;
    }
  }
};

}


// Private Member Functions 
template<class Type>
void mousse::probes::sampleAndWrite
(
  const GeometricField<Type, fvPatchField, volMesh>& vField
)
{
  Field<Type> values{sample(vField)};
  if (Pstream::master()) {
    unsigned int w = IOstream::defaultPrecision() + 7;
    OFstream& os = *probeFilePtrs_[vField.name()];
    os << setw(w) << vField.time().timeToUserTime(vField.time().value());
    FOR_ALL(values, probeI) {
      os << ' ' << setw(w) << values[probeI];
    }
    os << endl;
  }
}


template<class Type>
void mousse::probes::sampleAndWrite
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>& sField
)
{
  Field<Type> values{sample(sField)};
  if (Pstream::master()) {
    unsigned int w = IOstream::defaultPrecision() + 7;
    OFstream& os = *probeFilePtrs_[sField.name()];
    os << setw(w) << sField.time().timeToUserTime(sField.time().value());
    FOR_ALL(values, probeI) {
      os << ' ' << setw(w) << values[probeI];
    }
    os << endl;
  }
}


template<class Type>
void mousse::probes::sampleAndWrite(const fieldGroup<Type>& fields)
{
  FOR_ALL(fields, fieldI) {
    if (loadFromFiles_) {
      sampleAndWrite
      (
        GeometricField<Type, fvPatchField, volMesh>
        {
          {
            fields[fieldI],
            mesh_.time().timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::NO_WRITE,
            false
          },
          mesh_
        }
      );
    } else {
      objectRegistry::const_iterator iter = mesh_.find(fields[fieldI]);
      const word& ft =
        GeometricField<Type, fvPatchField, volMesh>::typeName;
      if (iter != objectRegistry::end() && iter()->type() == ft) {
        sampleAndWrite
        (
          mesh_.lookupObject<GeometricField<Type, fvPatchField, volMesh>>
          (
            fields[fieldI]
          )
        );
      }
    }
  }
}


template<class Type>
void mousse::probes::sampleAndWriteSurfaceFields(const fieldGroup<Type>& fields)
{
  FOR_ALL(fields, fieldI) {
    if (loadFromFiles_) {
      sampleAndWrite
      (
        GeometricField<Type, fvsPatchField, surfaceMesh>
        {
          {
            fields[fieldI],
            mesh_.time().timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::NO_WRITE,
            false
          },
          mesh_
        }
      );
    } else {
      objectRegistry::const_iterator iter = mesh_.find(fields[fieldI]);
      const word& ft
        = GeometricField<Type, fvsPatchField, surfaceMesh>::typeName;
      if (iter != objectRegistry::end() && iter()->type() == ft) {
        sampleAndWrite
        (
          mesh_.lookupObject<GeometricField<Type, fvsPatchField, surfaceMesh>>
          (
            fields[fieldI]
          )
        );
      }
    }
  }
}


// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::probes::sample
(
  const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
  const Type unsetVal{-VGREAT*pTraits<Type>::one};
  tmp<Field<Type>> tValues{new Field<Type>{this->size(), unsetVal}};
  Field<Type>& values = tValues();
  if (fixedLocations_)
  {
    autoPtr<interpolation<Type>> interpolator
    {
      interpolation<Type>::New(interpolationScheme_, vField)
    };
    FOR_ALL(*this, probeI) {
      if (elementList_[probeI] >= 0) {
        const vector& position = operator[](probeI);
        values[probeI] = interpolator().interpolate(position,
                                                    elementList_[probeI],
                                                    -1);
      }
    }
  } else {
    FOR_ALL(*this, probeI) {
      if (elementList_[probeI] >= 0) {
        values[probeI] = vField[elementList_[probeI]];
      }
    }
  }
  Pstream::listCombineGather(values, isNotEqOp<Type>());
  Pstream::listCombineScatter(values);
  return tValues;
}


template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::probes::sample(const word& fieldName) const
{
  return sample
  (
    mesh_.lookupObject<GeometricField<Type, fvPatchField, volMesh>>
    (
      fieldName
    )
  );
}


template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::probes::sample
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>& sField
) const
{
  const Type unsetVal{-VGREAT*pTraits<Type>::one};
  tmp<Field<Type>> tValues
  {
    new Field<Type>(this->size(), unsetVal)
  };
  Field<Type>& values = tValues();
  FOR_ALL(*this, probeI) {
    if (faceList_[probeI] >= 0) {
      values[probeI] = sField[faceList_[probeI]];
    }
  }
  Pstream::listCombineGather(values, isNotEqOp<Type>());
  Pstream::listCombineScatter(values);
  return tValues;
}


template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::probes::sampleSurfaceFields(const word& fieldName) const
{
  return sample
  (
    mesh_.lookupObject<GeometricField<Type, fvsPatchField, surfaceMesh>>
    (
      fieldName
    )
  );
}

