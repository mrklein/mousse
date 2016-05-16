#ifndef UTILITIES_POST_PROCESSING_LAGRANGIAN_STEADY_PARTICLE_TRACKS_STEADY_PARTICLE_TRACKS_TEMPLATES_HPP_
#define UTILITIES_POST_PROCESSING_LAGRANGIAN_STEADY_PARTICLE_TRACKS_STEADY_PARTICLE_TRACKS_TEMPLATES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ofstream.hpp"
#include "sortable_list.hpp"
#include "ioobject_list.hpp"
#include "ptr_list.hpp"
#include "field.hpp"


namespace mousse {

bool fieldOk(const IOobjectList& cloudObjs, const word& name);

template<class Type>
tmp<Field<Type>> readParticleField
(
  const word& name,
  const IOobjectList cloudObjs
);

template<class Type>
void readFields
(
  PtrList<List<Type>>& values,
  const List<word>& fields,
  const IOobjectList& cloudObjs
);

template<class Type>
void writeVTK(OFstream& os, const Type& value);
template<class Type>
void writeVTKFields
(
  OFstream& os,
  const PtrList<List<Type> >& values,
  const List<SortableList<scalar> >& agePerTrack,
  const List<word>& fieldNames
);

void processFields
(
  OFstream& os,
  const List<SortableList<scalar> >& agePerTrack,
  const List<word>& userFieldNames,
  const IOobjectList& cloudObjs
);

}  // namespace mousse


// Implementation
namespace mousse {

template<class Type>
bool fieldOk(const IOobjectList& cloudObjs, const word& name)
{
  IOobjectList objects{cloudObjs.lookupClass(IOField<Type>::typeName)};
  return (objects.lookup(name) != nullptr);
}

template<class Type>
tmp<Field<Type> > readParticleField
(
  const word& name,
  const IOobjectList cloudObjs
)
{
  IOobjectList objects{cloudObjs.lookupClass(IOField<Type>::typeName)};
  const IOobject* obj = objects.lookup(name);
  if (obj != nullptr) {
    IOField<Type> newField{*obj};
    return tmp<Field<Type>>{new Field<Type>{newField.xfer()}};
  }
  FATAL_ERROR_IN
  (
    "template<class Type>"
    "void readParticleField"
    "("
    "  const word&,"
    "  const IOobjectList"
    ")"
  )
  << "error: cloud field name " << name << " not found"
  << abort(FatalError);
  return Field<Type>::null();
}


template<class Type>
void readFields
(
  PtrList<List<Type> >& values,
  const List<word>& fieldNames,
  const IOobjectList& cloudObjs
)
{
  IOobjectList objects{cloudObjs.lookupClass(IOField<Type>::typeName)};
  FOR_ALL(fieldNames, j) {
    const IOobject* obj = objects.lookup(fieldNames[j]);
    if (obj != nullptr) {
      Info << "        reading field " << fieldNames[j] << endl;
      IOField<Type> newField{*obj};
      values.set(j, new List<Type>{newField.xfer()});
    } else {
      FATAL_ERROR_IN
      (
        "template<class Type>"
        "void readFields"
        "("
        "  PtrList<List<Type> >&, "
        "  const List<word>&, "
        "  const IOobjectList&"
        ")"
      )
      << "Unable to read field " << fieldNames[j]
      << abort(FatalError);
    }
  }
}


template<class Type>
void writeVTK(OFstream& os, const Type& value)
{
  os << value.component(0);
  for (label i=1; i<pTraits<Type>::nComponents; i++) {
    os << ' ' << value.component(i);
  }
}


template<class Type>
void writeVTKFields
(
  OFstream& os,
  const PtrList<List<Type> >& values,
  const List<List<label> >& addr,
  const List<word>& fieldNames
)
{
  label step = max(floor(8/pTraits<Type>::nComponents), 1);
  FOR_ALL(values, fieldI) {
    Info << "        writing field " << fieldNames[fieldI] << endl;
    os << nl << fieldNames[fieldI] << ' ' << pTraits<Type>::nComponents
      << ' ' << values[fieldI].size() << " float" << nl;
    label offset = 0;
    FOR_ALL(addr, trackI) {
      const List<label> ids{addr[trackI]};
      List<Type> data{UIndirectList<Type>{values[fieldI], ids}};
      label nData = data.size() - 1;
      FOR_ALL(data, i) {
        writeVTK<Type>(os, data[i]);
        if (((i + 1) % step == 0) || (i == nData)) {
          os << nl;
        } else {
          os << ' ';
        }
      }
      offset += ids.size();
    }
  }
}


template<class Type>
void processFields
(
  OFstream& os,
  const List<List<label> >& addr,
  const List<word>& userFieldNames,
  const IOobjectList& cloudObjs
)
{
  IOobjectList objects{cloudObjs.lookupClass(IOField<Type>::typeName)};
  if (!objects.size())
    return;
  DynamicList<word> fieldNames{objects.size()};
  FOR_ALL(userFieldNames, i) {
    IOobject* obj = objects.lookup(userFieldNames[i]);
    if (obj != NULL) {
      fieldNames.append(obj->name());
    }
  }
  fieldNames.shrink();
  PtrList<List<Type>> values{fieldNames.size()};
  readFields<Type>(values, fieldNames, cloudObjs);
  writeVTKFields<Type>(os, values, addr, fieldNames);
}

}  // namespace mousse

#endif

