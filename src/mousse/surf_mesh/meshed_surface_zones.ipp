// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "meshed_surface.hpp"


// Protected Member Functions 
template<class Face>
void mousse::MeshedSurface<Face>::checkZones()
{
  // extra safety, ensure we have at some zones
  // and they cover all the faces - fix start silently
  surfZoneList& zones = this->storedZones();
  if (zones.size()) {
    label count = 0;
    FOR_ALL(zones, zoneI) {
      zones[zoneI].start() = count;
      count += zones[zoneI].size();
    }
    if (count < this->size()) {
      WARNING_IN
      (
        "MeshedSurface::checkZones()\n"
      )
      << "more faces " << this->size() << " than zones " << count
      << " ... extending final zone"
      << endl;
      zones.last().size() += count - this->size();
    } else if (count > this->size()) {
      FATAL_ERROR_IN
      (
        "MeshedSurface::checkZones()\n"
      )
      << "more zones " << count << " than faces " << this->size()
      << exit(FatalError);
    }
  }
}


template<class Face>
void mousse::MeshedSurface<Face>::sortFacesAndStore
(
  const Xfer<List<Face> >& unsortedFaces,
  const Xfer<List<label> >& zoneIds,
  const bool sorted
)
{
  List<Face> oldFaces{unsortedFaces};
  List<label> zones{zoneIds};
  if (sorted) {
    // already sorted - simply transfer faces
    this->storedFaces().transfer(oldFaces);
  } else {
    // unsorted - determine the sorted order:
    // avoid SortableList since we discard the main list anyhow
    List<label> faceMap;
    sortedOrder(zones, faceMap);
    zones.clear();
    // sorted faces
    List<Face> newFaces{faceMap.size()};
    FOR_ALL(faceMap, faceI) {
      // use transfer to recover memory where possible
      newFaces[faceI].transfer(oldFaces[faceMap[faceI]]);
    }
    this->storedFaces().transfer(newFaces);
  }
  zones.clear();
}


// Member Functions 
template<class Face>
void mousse::MeshedSurface<Face>::addZones
(
  const UList<surfZone>& srfZones,
  const bool cullEmpty
)
{
  label nZone = 0;
  surfZoneList& zones = this->storedZones();
  zones.setSize(zones.size());
  FOR_ALL(zones, zoneI) {
    if (srfZones[zoneI].size() || !cullEmpty) {
      zones[nZone] = surfZone(srfZones[zoneI], nZone);
      nZone++;
    }
  }
  zones.setSize(nZone);
}


template<class Face>
void mousse::MeshedSurface<Face>::addZones
(
  const labelUList& sizes,
  const UList<word>& names,
  const bool cullEmpty
)
{
  label start   = 0;
  label nZone = 0;
  surfZoneList& zones = this->storedZones();
  zones.setSize(sizes.size());
  FOR_ALL(zones, zoneI) {
    if (sizes[zoneI] || !cullEmpty) {
      zones[nZone] = surfZone
      {
        names[zoneI],
        sizes[zoneI],
        start,
        nZone
      };
      start += sizes[zoneI];
      nZone++;
    }
  }
  zones.setSize(nZone);
}


template<class Face>
void mousse::MeshedSurface<Face>::addZones
(
  const labelUList& sizes,
  const bool cullEmpty
)
{
  label start   = 0;
  label nZone = 0;
  surfZoneList& zones = this->storedZones();
  zones.setSize(sizes.size());
  FOR_ALL(zones, zoneI) {
    if (sizes[zoneI] || !cullEmpty) {
      zones[nZone] = surfZone
      (
        word("zone") + ::mousse::name(nZone),
        sizes[zoneI],
        start,
        nZone
      );
      start += sizes[zoneI];
      nZone++;
    }
  }
  zones.setSize(nZone);
}


template<class Face>
void mousse::MeshedSurface<Face>::removeZones()
{
  this->storedZones().clear();
}
