// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::duplicatePoints
// Description
//   Duplicate points
// SourceFiles
//   duplicate_points.cpp
#ifndef duplicate_points_hpp_
#define duplicate_points_hpp_
#include "label_list.hpp"
#include "type_info.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class polyTopoChange;
class edge;
class face;
class mapPolyMesh;
class localPointRegion;
class duplicatePoints
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
    //- Per regionSide.pointRegion() the points created.
    //  element 0 is the original point.
    //  Can have empty entries if a point only has one region.
    labelListList duplicates_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    duplicatePoints(const duplicatePoints&);
    //- Disallow default bitwise assignment
    void operator=(const duplicatePoints&);
public:
  //- Runtime type information
  ClassName("duplicatePoints");
  // Constructors
    //- Construct from mesh
    duplicatePoints(const polyMesh& mesh);
  // Member Functions
    // Access
      //- Per point in regionSide.pointRegions() the points created.
      const labelListList& duplicates() const
      {
        return duplicates_;
      }
    // Topology changes
      //- Play commands into polyTopoChange to duplicate points. Gets
      //  localPointRegion structure which is per non-manifold point
      //  the regions per point.
      void setRefinement
      (
        const localPointRegion& regionSide,
        polyTopoChange&
      );
      //- Force recalculation of locally stored data on topological change
      void updateMesh(const mapPolyMesh&);
};
}  // namespace mousse
#endif
