// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "meshed_surface.hpp"
// Private Member Functions 
namespace mousse
{
  // specialization: from face -> triFace
  template<>
  void mousse::MeshedSurface<triFace>::transcribe(MeshedSurface<face>& surf)
  {
    // first triangulate
    surf.triangulate();
    this->storedPoints().transfer(surf.storedPoints());
    this->storedZones().transfer(surf.storedZones());
    // transcribe from face -> triFace
    List<face>&    origFaces = surf.storedFaces();
    List<triFace>  newFaces(origFaces.size());
    FOR_ALL(origFaces, faceI)
    {
      newFaces[faceI] = triFace
      (
        static_cast<const labelUList&>(origFaces[faceI])
      );
    }
    surf.clear();
    this->storedFaces().transfer(newFaces);
  }
  // specialization: from face -> face
  template<>
  void mousse::MeshedSurface<face>::transcribe(MeshedSurface<face>& surf)
  {
    this->transfer(surf);
  }
}  // end of namespace mousse
// Protected Member Functions 
// Member Functions
