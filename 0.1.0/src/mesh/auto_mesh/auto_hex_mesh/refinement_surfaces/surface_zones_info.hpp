// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceZonesInfo
// Description
// SourceFiles
//   surface_zones_info.cpp
#ifndef surface_zones_info_hpp_
#define surface_zones_info_hpp_
#include "named_enum.hpp"
#include "point.hpp"
#include "word.hpp"
#include "ptr_list.hpp"
#include "label_list.hpp"
namespace mousse
{
class searchableSurface;
class searchableSurfaces;
class polyMesh;
class dictionary;
class surfaceZonesInfo
{
public:
  //- Types of selection of area
  enum areaSelectionAlgo
  {
    INSIDE,
    OUTSIDE,
    INSIDEPOINT,
    NONE
  };
  static const NamedEnum<areaSelectionAlgo, 4> areaSelectionAlgoNames;
  //- What to do with faceZone faces
  enum faceZoneType
  {
    INTERNAL,
    BAFFLE,
    BOUNDARY
  };
  static const NamedEnum<faceZoneType, 3> faceZoneTypeNames;
private:
  // Private data
    //- Per 'interface' surface : name of faceZone to put faces into
    word faceZoneName_;
    //- Per 'interface' surface : name of cellZone to put cells into
    word cellZoneName_;
    //- Per 'interface' surface : (only used if surface is closed)
    //  How to select zone cells : surface inside or outside or given
    //  inside location.
    areaSelectionAlgo zoneInside_;
    //- If zoneInside=location gives the corresponding inside point
    point zoneInsidePoint_;
    //- Per 'interface' surface :
    //  What to do with outside
    faceZoneType faceType_;
public:
  // Constructors
    //- Construct from surfaces and dictionary
    surfaceZonesInfo
    (
      const searchableSurface& surface,
      const dictionary& surfacesDict
    );
    //- Construct from components
    surfaceZonesInfo
    (
      const word& faceZoneNames,
      const word& cellZoneNames,
      const areaSelectionAlgo& zoneInside,
      const point& zoneInsidePoints,
      const faceZoneType& faceType
    );
    //- Copy constructor
    surfaceZonesInfo(const surfaceZonesInfo&);
    //- Return clone
    autoPtr<surfaceZonesInfo> clone() const
    {
      return autoPtr<surfaceZonesInfo>{new surfaceZonesInfo{*this}};
    }
    //- Disallow default bitwise assignment
    surfaceZonesInfo& operator=(const surfaceZonesInfo&) = delete;
  // Member Functions
    // Access
      //- Per 'interface' surface : empty or name of faceZone to put
      //  faces into
      const word& faceZoneName() const
      {
        return faceZoneName_;
      }
      //- Per 'interface' surface : empty or name of cellZone to put
      //  cells into
      const word& cellZoneName() const
      {
        return cellZoneName_;
      }
      const areaSelectionAlgo& zoneInside() const
      {
        return zoneInside_;
      }
      //- Get specified inside locations for surfaces with a cellZone
      const point& zoneInsidePoint() const
      {
        return zoneInsidePoint_;
      }
      //- How to handle face of surfaces with a faceZone
      const faceZoneType& faceType() const
      {
        return faceType_;
      }
    // Query
      //- Get indices of unnamed surfaces (surfaces without faceZoneName)
      static labelList getUnnamedSurfaces
      (
        const PtrList<surfaceZonesInfo>& surfList
      );
      //- Get indices of named surfaces (surfaces with faceZoneName)
      static labelList getNamedSurfaces
      (
        const PtrList<surfaceZonesInfo>& surfList
      );
      //- Get indices of surfaces with a cellZone that are closed and
      //  have 'inside' or 'outside' selection.
      static labelList getClosedNamedSurfaces
      (
        const PtrList<surfaceZonesInfo>& surfList,
        const searchableSurfaces& allGeometry,
        const labelList& surfaces
      );
      //- Get indices of surfaces with a cellZone that are unclosed
      static labelList getUnclosedNamedSurfaces
      (
        const PtrList<surfaceZonesInfo>& surfList,
        const searchableSurfaces& allGeometry,
        const labelList& surfaces
      );
      //- Get indices of surfaces with a cellZone that are closed.
      static labelList getAllClosedNamedSurfaces
      (
        const PtrList<surfaceZonesInfo>& surfList,
        const searchableSurfaces& allGeometry,
        const labelList& surfaces
      );
      //- Get indices of surfaces with a cellZone that have 'insidePoint'
      //  section.
      static labelList getInsidePointNamedSurfaces
      (
        const PtrList<surfaceZonesInfo>& surfList
      );
      static labelList addCellZonesToMesh
      (
        const PtrList<surfaceZonesInfo>& surfList,
        const labelList& namedSurfaces,
        polyMesh& mesh
      );
      static labelList addFaceZonesToMesh
      (
        const PtrList<surfaceZonesInfo>& surfList,
        const labelList& namedSurfaces,
        polyMesh& mesh
      );
};
}  // namespace mousse
#endif
