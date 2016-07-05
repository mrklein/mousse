// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ftr_surface_format.hpp"
#include "keyed.hpp"
#include "ifstream.hpp"


// Constructors 
template<class Face>
mousse::fileFormats::FTRsurfaceFormat<Face>::FTRsurfaceFormat
(
  const fileName& filename
)
{
  read(filename);
}


// Member Functions 
template<class Face>
bool mousse::fileFormats::FTRsurfaceFormat<Face>::read
(
  const fileName& filename
)
{
  this->clear();
  IFstream is{filename};
  if (!is.good()) {
    FATAL_ERROR_IN
    (
      "fileFormats::FTRsurfaceFormat::read(const fileName&)"
    )
    << "Cannot read file " << filename
    << exit(FatalError);
  }
  List<ftrPatch> ftrPatches{is};
  // points read directly
  is >> this->storedPoints();
  // triFaces read with attached keys
  List< Keyed<triFace>> facesRead{is};
  List<Face> faceLst{facesRead.size()};
  List<label> zoneIds{facesRead.size()};
  // disentangle faces/keys - already triangulated
  FOR_ALL(facesRead, faceI) {
    // unfortunately cannot transfer to save memory
    faceLst[faceI] = facesRead[faceI];
    zoneIds[faceI] = facesRead[faceI].key();
  }
  this->storedFaces().transfer(faceLst);
  this->storedZoneIds().transfer(zoneIds);
  facesRead.clear();
  // change ftrPatch into surfZoneIdentifier
  List<surfZoneIdentifier> newZones{ftrPatches.size()};
  FOR_ALL(newZones, zoneI) {
    newZones[zoneI] = surfZoneIdentifier
    {
      ftrPatches[zoneI].name(),
      zoneI
    };
  }
  this->storedZoneToc().transfer(newZones);
  return true;
}

