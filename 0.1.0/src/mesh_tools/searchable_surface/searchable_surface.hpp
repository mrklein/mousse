#ifndef MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_SURFACE_HPP_
#define MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_SURFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableSurface
// Description
//   Base class of (analytical or triangulated) surface.
//   Encapsulates all the search routines. WIP.
//   Information returned is usually a pointIndexHit:
//   - bool  : was intersection/nearest found?
//   - point : intersection point or nearest point
//   - index : unique index on surface (e.g. triangle for triSurfaceMesh)

#include "point_field.hpp"
#include "bound_box.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "point_index_hit.hpp"
#include "line_point_ref.hpp"
#include "object_registry.hpp"
#include "volume_type.hpp"
#include "dictionary.hpp"


namespace mousse {

// Forward declaration of classes
class objectRegistry;
class mapDistribute;
class treeBoundBox;


class searchableSurface
:
  public regIOobject
{
  // Private data
    const word name_;
    boundBox bounds_;
public:
  //- Runtime type information
  TYPE_NAME("searchableSurface");
  // Declare run-time constructor selection table
    // For the dictionary constructor
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      searchableSurface,
      dict,
      (
        const IOobject& io,
        const dictionary& dict
      ),
      (io, dict)
    );
    //- Class used for the read-construction of
    //  PtrLists of searchableSurface.
    class iNew
    {
      IOobject& io_;
    public:
      iNew(IOobject& io)
      :
        io_{io}
      {}
      autoPtr<searchableSurface> operator()(Istream& is) const
      {
        word surfaceType{is};
        word readName{is};
        dictionary dict{is};
        autoPtr<IOobject> namedIO{io_.clone()};
        namedIO().rename(readName);
        return searchableSurface::New(surfaceType, namedIO(), dict);
      }
    };
  // Constructors
    searchableSurface(const IOobject& io);
    //- Clone
    virtual autoPtr<searchableSurface> clone() const
    {
      NOT_IMPLEMENTED("autoPtr<searchableSurface> clone() const");
      return autoPtr<searchableSurface>{NULL};
    }
    //- Disallow default bitwise copy construct
    searchableSurface(const searchableSurface&) = delete;
    //- Disallow default bitwise assignment
    searchableSurface& operator=(const searchableSurface&) = delete;
  // Selectors
    //- Return a reference to the selected searchableSurface
    static autoPtr<searchableSurface> New
    (
      const word& surfaceType,
      const IOobject& io,
      const dictionary& dict
    );
  //- Destructor
  virtual ~searchableSurface();
  // Member Functions
    //- Return const reference to boundBox
    const boundBox& bounds() const
    {
      return bounds_;
    }
    //- Return non-const access to the boundBox to allow it to be set.
    boundBox& bounds()
    {
      return bounds_;
    }
    //- Names of regions
    virtual const wordList& regions() const = 0;
    //- Whether supports volume type below
    virtual bool hasVolumeType() const = 0;
    //- Range of local indices that can be returned
    virtual label size() const = 0;
    //- Range of global indices that can be returned
    virtual label globalSize() const
    {
      return size();
    }
    //- Get representative set of element coordinates
    //  Usually the element centres (should be of length size()).
    virtual tmp<pointField> coordinates() const = 0;
    //- Get bounding spheres (centre and radius squared), one per element.
    //  Any point on element is guaranteed to be inside.
    virtual void boundingSpheres
    (
      pointField& centres,
      scalarField& radiusSqr
    ) const = 0;
    //- Get the points that define the surface.
    virtual tmp<pointField> points() const = 0;
    //- Does any part of the surface overlap the supplied bound box?
    virtual bool overlaps(const boundBox& bb) const = 0;
    // Multiple point queries. When surface is distributed the index
    // should be a global index. Not done yet.
      virtual void findNearest
      (
        const pointField& sample,
        const scalarField& nearestDistSqr,
        List<pointIndexHit>&
      ) const = 0;
      //- Find the nearest locations for the supplied points to a
      //  particular region in the searchable surface.
      virtual void findNearest
      (
        const pointField& samples,
        const scalarField& nearestDistSqr,
        const labelList& /*regionIndices*/,
        List<pointIndexHit>& info
      ) const
      {
        findNearest(samples, nearestDistSqr, info);
      }
      //- Find first intersection on segment from start to end.
      //  Note: searchableSurfacesQueries expects no
      //  intersection to be found if start==end. Is problem?
      virtual void findLine
      (
        const pointField& start,
        const pointField& end,
        List<pointIndexHit>&
      ) const = 0;
      //- Return any intersection on segment from start to end.
      virtual void findLineAny
      (
        const pointField& start,
        const pointField& end,
        List<pointIndexHit>&
      ) const = 0;
      //- Get all intersections in order from start to end.
      virtual void findLineAll
      (
        const pointField& start,
        const pointField& end,
        List<List<pointIndexHit> >&
      ) const = 0;
      //- From a set of points and indices get the region
      virtual void getRegion
      (
        const List<pointIndexHit>&,
        labelList& region
      ) const = 0;
      //- From a set of points and indices get the normal
      virtual void getNormal
      (
        const List<pointIndexHit>&,
        vectorField& normal
      ) const = 0;
      //- Determine type (inside/outside) for point. unknown if
      //  cannot be determined (e.g. non-manifold surface)
      virtual void getVolumeType
      (
        const pointField&,
        List<volumeType>&
      ) const = 0;
      //- Find nearest, normal and region. Can be overridden with
      //  optimised implementation
      virtual void findNearest
      (
        const pointField& sample,
        const scalarField& nearestDistSqr,
        List<pointIndexHit>&,
        vectorField& normal,
        labelList& region
      ) const;
    // Other
      //- Set bounds of surface. Bounds currently set as list of
      //  bounding boxes. The bounds are hints to the surface as for
      //  the range of queries it can expect. faceMap/pointMap can be
      //  set if the surface has done any redistribution.
      virtual void distribute
      (
        const List<treeBoundBox>&,
        const bool /*keepNonLocal*/,
        autoPtr<mapDistribute>& /*faceMap*/,
        autoPtr<mapDistribute>& /*pointMap*/
      )
      {}
      //- WIP. Store element-wise field.
      virtual void setField(const labelList& /*values*/)
      {}
      //- WIP. From a set of hits (points and
      //  indices) get the specified field. Misses do not get set. Return
      //  empty field if not supported.
      virtual void getField(const List<pointIndexHit>&, labelList& values)
      const
      {
        values.clear();
      }
};
}  // namespace mousse
#endif
