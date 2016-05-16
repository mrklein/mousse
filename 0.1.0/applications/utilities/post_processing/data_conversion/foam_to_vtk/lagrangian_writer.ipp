// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lagrangian_writer.hpp"
#include "write_funs.hpp"
#include "iofield.hpp"


template<class Type>
void mousse::lagrangianWriter::writeIOField(const wordList& objects)
{
  FOR_ALL(objects, i) {
    const word& object = objects[i];
    IOobject header
    {
      object,
      vMesh_.mesh().time().timeName(),
      cloud::prefix/cloudName_,
      vMesh_.mesh(),
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    };
    IOField<Type> fld{header};
    os_ << object << ' ' << pTraits<Type>::nComponents << ' '
      << fld.size() << " float" << std::endl;
    DynamicList<floatScalar> fField{pTraits<Type>::nComponents*fld.size()};
    writeFuns::insert(fld, fField);
    writeFuns::write(os_, binary_, fField);
  }
}

