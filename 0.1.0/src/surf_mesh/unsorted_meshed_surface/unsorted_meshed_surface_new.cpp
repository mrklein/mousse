// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "unsorted_meshed_surface.hpp"
// Member Functions 
template<class Face>
mousse::autoPtr< mousse::UnsortedMeshedSurface<Face> >
mousse::UnsortedMeshedSurface<Face>::New(const fileName& name, const word& ext)
{
  if (debug)
  {
    Info<< "UnsortedMeshedSurface::New(const fileName&, const word&) : "
      "constructing UnsortedMeshedSurface"
      << endl;
  }
  typename fileExtensionConstructorTable::iterator cstrIter =
    fileExtensionConstructorTablePtr_->find(ext);
  if (cstrIter == fileExtensionConstructorTablePtr_->end())
  {
    // no direct reader, use the parent if possible
    wordHashSet supported = ParentType::readTypes();
    if (supported.found(ext))
    {
      // create indirectly
      autoPtr<UnsortedMeshedSurface<Face> > surf
      (
        new UnsortedMeshedSurface<Face>
      );
      surf().transfer(ParentType::New(name, ext)());
      return surf;
    }
    // nothing left but to issue an error
    supported += readTypes();
    FatalErrorIn
    (
      "UnsortedMeshedSurface<Face>::New"
      "(const fileName&, const word&) : "
      "constructing UnsortedMeshedSurface"
    )   << "Unknown file extension " << ext << nl << nl
      << "Valid types are:" << nl
      << supported
      << exit(FatalError);
  }
  return autoPtr<UnsortedMeshedSurface<Face> >(cstrIter()(name));
}
template<class Face>
mousse::autoPtr< mousse::UnsortedMeshedSurface<Face> >
mousse::UnsortedMeshedSurface<Face>::New(const fileName& name)
{
  word ext = name.ext();
  if (ext == "gz")
  {
    ext = name.lessExt().ext();
  }
  return New(name, ext);
}
