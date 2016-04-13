// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "meshed_surface.hpp"
#include "unsorted_meshed_surface.hpp"


// Member Functions 
template<class Face>
mousse::autoPtr<mousse::MeshedSurface<Face>>
mousse::MeshedSurface<Face>::New(const fileName& name, const word& ext)
{
  if (debug) {
    Info << "MeshedSurface::New(const fileName&, const word&) : "
      "constructing MeshedSurface"
      << endl;
  }
  typename fileExtensionConstructorTable::iterator cstrIter =
    fileExtensionConstructorTablePtr_->find(ext);
  if (cstrIter == fileExtensionConstructorTablePtr_->end()) {
    // no direct reader, delegate if possible
    wordHashSet supported = FriendType::readTypes();
    if (supported.found(ext)) {
      // create indirectly
      autoPtr<MeshedSurface<Face>> surf{new MeshedSurface<Face>};
      surf().transfer(FriendType::New(name, ext)());
      return surf;
    }
    // nothing left to try, issue error
    supported += readTypes();
    FATAL_ERROR_IN
    (
      "MeshedSurface<Face>::New(const fileName&, const word&) : "
      "constructing MeshedSurface"
    )
    << "Unknown file extension " << ext << nl << nl
    << "Valid types are :" << nl
    << supported
    << exit(FatalError);
  }
  return autoPtr<MeshedSurface<Face>>{cstrIter()(name)};
}


template<class Face>
mousse::autoPtr< mousse::MeshedSurface<Face> >
mousse::MeshedSurface<Face>::New(const fileName& name)
{
  word ext = name.ext();
  if (ext == "gz") {
    ext = name.lessExt().ext();
  }
  return New(name, ext);
}
