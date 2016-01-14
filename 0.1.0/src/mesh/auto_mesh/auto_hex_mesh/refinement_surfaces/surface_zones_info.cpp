// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_zones_info.hpp"
#include "searchable_surface.hpp"
#include "searchable_surfaces.hpp"
#include "poly_mesh.hpp"
#include "dictionary.hpp"
// Static Data Members
namespace mousse
{
  template<>
  const char* mousse::NamedEnum
  <
    mousse::surfaceZonesInfo::areaSelectionAlgo,
    4
  >::names[] =
  {
    "inside",
    "outside",
    "insidePoint",
    "none"
  };
}
const mousse::NamedEnum<mousse::surfaceZonesInfo::areaSelectionAlgo, 4>
  mousse::surfaceZonesInfo::areaSelectionAlgoNames;
namespace mousse
{
  template<>
  const char* mousse::NamedEnum
  <
    mousse::surfaceZonesInfo::faceZoneType,
    3
  >::names[] =
  {
    "internal",
    "baffle",
    "boundary"
  };
}
const mousse::NamedEnum<mousse::surfaceZonesInfo::faceZoneType, 3>
  mousse::surfaceZonesInfo::faceZoneTypeNames;
// Constructors 
mousse::surfaceZonesInfo::surfaceZonesInfo
(
  const searchableSurface& surface,
  const dictionary& surfacesDict
)
:
  faceZoneName_{},
  cellZoneName_{},
  zoneInside_{NONE},
  zoneInsidePoint_{point::min},
  faceType_{INTERNAL}
{
  // Global zone names per surface
  if (surfacesDict.readIfPresent("faceZone", faceZoneName_))
  {
    // Read optional entry to determine inside of faceZone
    word method;
    bool hasSide = surfacesDict.readIfPresent("cellZoneInside", method);
    if (hasSide)
    {
      zoneInside_ = areaSelectionAlgoNames[method];
      if (zoneInside_ == INSIDEPOINT)
      {
        surfacesDict.lookup("insidePoint") >> zoneInsidePoint_;
      }
    }
    else
    {
      // Check old syntax
      bool inside;
      if (surfacesDict.readIfPresent("zoneInside", inside))
      {
        hasSide = true;
        zoneInside_ = (inside ? INSIDE : OUTSIDE);
      }
    }
    // Read optional cellZone name
    if (surfacesDict.readIfPresent("cellZone", cellZoneName_))
    {
      if((zoneInside_ == INSIDE || zoneInside_ == OUTSIDE)
         && !surface.hasVolumeType())
      {
        IO_WARNING_IN
        (
          "surfaceZonesInfo::surfaceZonesInfo(..)",
          surfacesDict
        )
        << "Illegal entry zoneInside "
        << areaSelectionAlgoNames[zoneInside_]
        << " for faceZone "
        << faceZoneName_
        << " since surface is not closed." << endl;
      }
    }
    else if (hasSide)
    {
      IO_WARNING_IN
      (
        "surfaceZonesInfo::surfaceZonesInfo(..)",
        surfacesDict
      )
      << "Unused entry zoneInside for faceZone "
      << faceZoneName_
      << " since no cellZone specified."
      << endl;
    }
    // How to handle faces on faceZone
    word faceTypeMethod;
    if (surfacesDict.readIfPresent("faceType", faceTypeMethod))
    {
      faceType_ = faceZoneTypeNames[faceTypeMethod];
    }
  }
}
mousse::surfaceZonesInfo::surfaceZonesInfo
(
  const word& faceZoneName,
  const word& cellZoneName,
  const areaSelectionAlgo& zoneInside,
  const point& zoneInsidePoint,
  const faceZoneType& faceType
)
:
  faceZoneName_{faceZoneName},
  cellZoneName_{cellZoneName},
  zoneInside_{zoneInside},
  zoneInsidePoint_{zoneInsidePoint},
  faceType_{faceType}
{}
mousse::surfaceZonesInfo::surfaceZonesInfo(const surfaceZonesInfo& surfZone)
:
  faceZoneName_{surfZone.faceZoneName()},
  cellZoneName_{surfZone.cellZoneName()},
  zoneInside_{surfZone.zoneInside()},
  zoneInsidePoint_{surfZone.zoneInsidePoint()},
  faceType_{surfZone.faceType()}
{}
mousse::labelList mousse::surfaceZonesInfo::getUnnamedSurfaces
(
  const PtrList<surfaceZonesInfo>& surfList
)
{
  labelList anonymousSurfaces(surfList.size());
  label i = 0;
  FOR_ALL(surfList, surfI)
  {
    if (surfList[surfI].faceZoneName().empty())
    {
      anonymousSurfaces[i++] = surfI;
    }
  }
  anonymousSurfaces.setSize(i);
  return anonymousSurfaces;
}
mousse::labelList mousse::surfaceZonesInfo::getNamedSurfaces
(
  const PtrList<surfaceZonesInfo>& surfList
)
{
 labelList namedSurfaces(surfList.size());
  label namedI = 0;
  FOR_ALL(surfList, surfI)
  {
    if (surfList.set(surfI) && surfList[surfI].faceZoneName().size())
    {
      namedSurfaces[namedI++] = surfI;
    }
  }
  namedSurfaces.setSize(namedI);
  return namedSurfaces;
}
mousse::labelList mousse::surfaceZonesInfo::getClosedNamedSurfaces
(
  const PtrList<surfaceZonesInfo>& surfList,
  const searchableSurfaces& allGeometry,
  const labelList& surfaces
)
{
  labelList closed(surfList.size());
  label closedI = 0;
  FOR_ALL(surfList, surfI)
  {
    if (surfList.set(surfI) && surfList[surfI].cellZoneName().size()
        && (surfList[surfI].zoneInside() == surfaceZonesInfo::INSIDE
            || surfList[surfI].zoneInside() == surfaceZonesInfo::OUTSIDE)
        && allGeometry[surfaces[surfI]].hasVolumeType())
    {
      closed[closedI++] = surfI;
    }
  }
  closed.setSize(closedI);
  return closed;
}
mousse::labelList mousse::surfaceZonesInfo::getUnclosedNamedSurfaces
(
  const PtrList<surfaceZonesInfo>& surfList,
  const searchableSurfaces& allGeometry,
  const labelList& surfaces
)
{
  labelList unclosed(surfList.size());
  label unclosedI = 0;
  FOR_ALL(surfList, surfI)
  {
    if (surfList.set(surfI) && !allGeometry[surfaces[surfI]].hasVolumeType())
    {
      unclosed[unclosedI++] = surfI;
    }
  }
  unclosed.setSize(unclosedI);
  return unclosed;
}
mousse::labelList mousse::surfaceZonesInfo::getAllClosedNamedSurfaces
(
  const PtrList<surfaceZonesInfo>& surfList,
  const searchableSurfaces& allGeometry,
  const labelList& surfaces
)
{
  labelList closed(surfList.size());
  label closedI = 0;
  FOR_ALL(surfList, surfI)
  {
    if (surfList.set(surfI) && surfList[surfI].cellZoneName().size()
        && allGeometry[surfaces[surfI]].hasVolumeType())
    {
      closed[closedI++] = surfI;
    }
  }
  closed.setSize(closedI);
  return closed;
}
mousse::labelList mousse::surfaceZonesInfo::getInsidePointNamedSurfaces
(
  const PtrList<surfaceZonesInfo>& surfList
)
{
  labelList closed(surfList.size());
  label closedI = 0;
  FOR_ALL(surfList, surfI)
  {
    if (surfList.set(surfI) && surfList[surfI].cellZoneName().size()
        && surfList[surfI].zoneInside() == surfaceZonesInfo::INSIDEPOINT)
    {
      closed[closedI++] = surfI;
    }
  }
  closed.setSize(closedI);
  return closed;
}
mousse::labelList mousse::surfaceZonesInfo::addCellZonesToMesh
(
  const PtrList<surfaceZonesInfo>& surfList,
  const labelList& namedSurfaces,
  polyMesh& mesh
)
{
  labelList surfaceToCellZone(surfList.size(), -1);
  cellZoneMesh& cellZones = mesh.cellZones();
  FOR_ALL(namedSurfaces, i)
  {
    label surfI = namedSurfaces[i];
    const word& cellZoneName = surfList[surfI].cellZoneName();
    if (cellZoneName != word::null)
    {
      label zoneI = cellZones.findZoneID(cellZoneName);
      if (zoneI == -1)
      {
        zoneI = cellZones.size();
        cellZones.setSize(zoneI+1);
        cellZones.set
        (
          zoneI,
          new cellZone
          {
            cellZoneName,   //name
            labelList(0),   //addressing
            zoneI,          //index
            cellZones       //cellZoneMesh
          }
        );
      }
      surfaceToCellZone[surfI] = zoneI;
    }
  }
  // Check they are synced
  List<wordList> allCellZones{Pstream::nProcs()};
  allCellZones[Pstream::myProcNo()] = cellZones.names();
  Pstream::gatherList(allCellZones);
  Pstream::scatterList(allCellZones);
  for (label procI = 1; procI < allCellZones.size(); procI++)
  {
    if (allCellZones[procI] != allCellZones[0])
    {
      FATAL_ERROR_IN
      (
        "meshRefinement::zonify"
        "(const label, const point&)"
      )
      << "Zones not synchronised among processors." << nl
      << " Processor0 has cellZones:" << allCellZones[0]
      << " , processor" << procI
      << " has cellZones:" << allCellZones[procI]
      << exit(FatalError);
    }
  }
  return surfaceToCellZone;
}
mousse::labelList mousse::surfaceZonesInfo::addFaceZonesToMesh
(
  const PtrList<surfaceZonesInfo>& surfList,
  const labelList& namedSurfaces,
  polyMesh& mesh
)
{
  labelList surfaceToFaceZone(surfList.size(), -1);
  faceZoneMesh& faceZones = mesh.faceZones();
  FOR_ALL(namedSurfaces, i)
  {
    label surfI = namedSurfaces[i];
    const word& faceZoneName = surfList[surfI].faceZoneName();
    label zoneI = faceZones.findZoneID(faceZoneName);
    if (zoneI == -1)
    {
      zoneI = faceZones.size();
      faceZones.setSize(zoneI+1);
      faceZones.set
      (
        zoneI,
        new faceZone
        {
          faceZoneName,   //name
          labelList(0),   //addressing
          boolList(0),    //flipmap
          zoneI,          //index
          faceZones       //faceZoneMesh
        }
      );
    }
    surfaceToFaceZone[surfI] = zoneI;
  }
  // Check they are synced
  List<wordList> allFaceZones{Pstream::nProcs()};
  allFaceZones[Pstream::myProcNo()] = faceZones.names();
  Pstream::gatherList(allFaceZones);
  Pstream::scatterList(allFaceZones);
  for (label procI = 1; procI < allFaceZones.size(); procI++)
  {
    if (allFaceZones[procI] != allFaceZones[0])
    {
      FATAL_ERROR_IN
      (
        "meshRefinement::zonify"
        "(const label, const point&)"
      )
      << "Zones not synchronised among processors." << nl
      << " Processor0 has faceZones:" << allFaceZones[0]
      << " , processor" << procI
      << " has faceZones:" << allFaceZones[procI]
      << exit(FatalError);
    }
  }
  return surfaceToFaceZone;
}
