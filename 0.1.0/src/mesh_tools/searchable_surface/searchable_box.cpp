// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_box.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "sortable_list.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(searchableBox, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(searchableSurface, searchableBox, dict);
}
// Private Member Functions 
void mousse::searchableBox::projectOntoCoordPlane
(
  const direction dir,
  const point& planePt,
  pointIndexHit& info
) const
{
  // Set point
  info.rawPoint()[dir] = planePt[dir];
  // Set face
  if (planePt[dir] == min()[dir])
  {
    info.setIndex(dir*2);
  }
  else if (planePt[dir] == max()[dir])
  {
    info.setIndex(dir*2+1);
  }
  else
  {
    FATAL_ERROR_IN("searchableBox::projectOntoCoordPlane(..)")
      << "Point on plane " << planePt
      << " is not on coordinate " << min()[dir]
      << " nor " << max()[dir] << abort(FatalError);
  }
}
// Returns miss or hit with face (0..5) and region(always 0)
mousse::pointIndexHit mousse::searchableBox::findNearest
(
  const point& bbMid,
  const point& sample,
  const scalar nearestDistSqr
) const
{
  // Point can be inside or outside. For every component direction can be
  // left of min, right of max or inbetween.
  // - outside points: project first one x plane (either min().x()
  // or max().x()), then onto y plane and finally z. You should be left
  // with intersection point
  // - inside point: find nearest side (compare to mid point). Project onto
  //   that.
  // The face is set to the last projected face.
  // Outside point projected onto cube. Assume faces 0..5.
  pointIndexHit info(true, sample, -1);
  bool outside = false;
  // (for internal points) per direction what nearest cube side is
  point near;
  for (direction dir = 0; dir < vector::nComponents; dir++)
  {
    if (info.rawPoint()[dir] < min()[dir])
    {
      projectOntoCoordPlane(dir, min(), info);
      outside = true;
    }
    else if (info.rawPoint()[dir] > max()[dir])
    {
      projectOntoCoordPlane(dir, max(), info);
      outside = true;
    }
    else if (info.rawPoint()[dir] > bbMid[dir])
    {
      near[dir] = max()[dir];
    }
    else
    {
      near[dir] = min()[dir];
    }
  }
  // For outside points the info will be correct now. Handle inside points
  // using the three near distances. Project onto the nearest plane.
  if (!outside)
  {
    vector dist(cmptMag(info.rawPoint() - near));
    if (dist.x() < dist.y())
    {
      if (dist.x() < dist.z())
      {
        // Project onto x plane
        projectOntoCoordPlane(vector::X, near, info);
      }
      else
      {
        projectOntoCoordPlane(vector::Z, near, info);
      }
    }
    else
    {
      if (dist.y() < dist.z())
      {
        projectOntoCoordPlane(vector::Y, near, info);
      }
      else
      {
        projectOntoCoordPlane(vector::Z, near, info);
      }
    }
  }
  // Check if outside. Optimisation: could do some checks on distance already
  // on components above
  if (magSqr(info.rawPoint() - sample) > nearestDistSqr)
  {
    info.setMiss();
    info.setIndex(-1);
  }
  return info;
}
// Constructors 
mousse::searchableBox::searchableBox
(
  const IOobject& io,
  const treeBoundBox& bb
)
:
  searchableSurface(io),
  treeBoundBox(bb)
{
  if (!contains(midpoint()))
  {
    FATAL_ERROR_IN
    (
      "mousse::searchableBox::searchableBox\n"
      "(\n"
      "    const IOobject& io,\n"
      "    const treeBoundBox& bb\n"
      ")\n"
    )   << "Illegal bounding box specification : "
      << static_cast<const treeBoundBox>(*this) << exit(FatalError);
  }
  bounds() = static_cast<boundBox>(*this);
}
mousse::searchableBox::searchableBox
(
  const IOobject& io,
  const dictionary& dict
)
:
  searchableSurface(io),
  treeBoundBox(dict.lookup("min"), dict.lookup("max"))
{
  if (!contains(midpoint()))
  {
    FATAL_ERROR_IN
    (
      "mousse::searchableBox::searchableBox\n"
      "(\n"
      "    const IOobject& io,\n"
      "    const treeBoundBox& bb\n"
      ")\n"
    )   << "Illegal bounding box specification : "
      << static_cast<const treeBoundBox>(*this) << exit(FatalError);
  }
  bounds() = static_cast<boundBox>(*this);
}
// Destructor 
mousse::searchableBox::~searchableBox()
{}
// Member Functions 
const mousse::wordList& mousse::searchableBox::regions() const
{
  if (regions_.empty())
  {
    regions_.setSize(1);
    regions_[0] = "region0";
  }
  return regions_;
}
mousse::tmp<mousse::pointField> mousse::searchableBox::coordinates() const
{
  tmp<pointField> tCtrs = tmp<pointField>(new pointField(6));
  pointField& ctrs = tCtrs();
  const pointField pts(treeBoundBox::points());
  const faceList& fcs = treeBoundBox::faces;
  FOR_ALL(fcs, i)
  {
    ctrs[i] = fcs[i].centre(pts);
  }
  return tCtrs;
}
void mousse::searchableBox::boundingSpheres
(
  pointField& centres,
  scalarField& radiusSqr
) const
{
  centres.setSize(size());
  radiusSqr.setSize(size());
  radiusSqr = 0.0;
  const pointField pts(treeBoundBox::points());
  const faceList& fcs = treeBoundBox::faces;
  FOR_ALL(fcs, i)
  {
    const face& f = fcs[i];
    centres[i] = f.centre(pts);
    FOR_ALL(f, fp)
    {
      const point& pt = pts[f[fp]];
      radiusSqr[i] = mousse::max
      (
        radiusSqr[i],
        mousse::magSqr(pt-centres[i])
      );
    }
  }
  // Add a bit to make sure all points are tested inside
  radiusSqr += mousse::sqr(SMALL);
}
mousse::tmp<mousse::pointField> mousse::searchableBox::points() const
{
  return treeBoundBox::points();
}
mousse::pointIndexHit mousse::searchableBox::findNearest
(
  const point& sample,
  const scalar nearestDistSqr
) const
{
  return findNearest(midpoint(), sample, nearestDistSqr);
}
mousse::pointIndexHit mousse::searchableBox::findNearestOnEdge
(
  const point& sample,
  const scalar nearestDistSqr
) const
{
  const point bbMid(midpoint());
  // Outside point projected onto cube. Assume faces 0..5.
  pointIndexHit info(true, sample, -1);
  bool outside = false;
  // (for internal points) per direction what nearest cube side is
  point near;
  for (direction dir = 0; dir < vector::nComponents; dir++)
  {
    if (info.rawPoint()[dir] < min()[dir])
    {
      projectOntoCoordPlane(dir, min(), info);
      outside = true;
    }
    else if (info.rawPoint()[dir] > max()[dir])
    {
      projectOntoCoordPlane(dir, max(), info);
      outside = true;
    }
    else if (info.rawPoint()[dir] > bbMid[dir])
    {
      near[dir] = max()[dir];
    }
    else
    {
      near[dir] = min()[dir];
    }
  }
  // For outside points the info will be correct now. Handle inside points
  // using the three near distances. Project onto the nearest two planes.
  if (!outside)
  {
    // Get the per-component distance to nearest wall
    vector dist(cmptMag(info.rawPoint() - near));
    SortableList<scalar> sortedDist(3);
    sortedDist[0] = dist[0];
    sortedDist[1] = dist[1];
    sortedDist[2] = dist[2];
    sortedDist.sort();
    // Project onto nearest
    projectOntoCoordPlane(sortedDist.indices()[0], near, info);
    // Project onto second nearest
    projectOntoCoordPlane(sortedDist.indices()[1], near, info);
  }
  // Check if outside. Optimisation: could do some checks on distance already
  // on components above
  if (magSqr(info.rawPoint() - sample) > nearestDistSqr)
  {
    info.setMiss();
    info.setIndex(-1);
  }
  return info;
}
mousse::pointIndexHit mousse::searchableBox::findNearest
(
  const linePointRef&,
  treeBoundBox& /*tightest*/,
  point& /*linePoint*/
) const
{
  NOT_IMPLEMENTED
  (
    "searchableBox::findNearest"
    "(const linePointRef&, treeBoundBox&, point&)"
  );
  return pointIndexHit();
}
mousse::pointIndexHit mousse::searchableBox::findLine
(
  const point& start,
  const point& end
) const
{
  pointIndexHit info(false, start, -1);
  bool foundInter;
  if (posBits(start) == 0)
  {
    if (posBits(end) == 0)
    {
      // Both start and end inside.
      foundInter = false;
    }
    else
    {
      // end is outside. Clip to bounding box.
      foundInter = intersects(end, start, info.rawPoint());
    }
  }
  else
  {
    // start is outside. Clip to bounding box.
    foundInter = intersects(start, end, info.rawPoint());
  }
  // Classify point
  if (foundInter)
  {
    info.setHit();
    for (direction dir = 0; dir < vector::nComponents; dir++)
    {
      if (info.rawPoint()[dir] == min()[dir])
      {
        info.setIndex(2*dir);
        break;
      }
      else if (info.rawPoint()[dir] == max()[dir])
      {
        info.setIndex(2*dir+1);
        break;
      }
    }
    if (info.index() == -1)
    {
      FATAL_ERROR_IN("searchableBox::findLine(const point&, const point&)")
        << "point " << info.rawPoint()
        << " on segment " << start << end
        << " should be on face of " << *this
        << " but it isn't." << abort(FatalError);
    }
  }
  return info;
}
mousse::pointIndexHit mousse::searchableBox::findLineAny
(
  const point& start,
  const point& end
) const
{
  return findLine(start, end);
}
void mousse::searchableBox::findNearest
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  List<pointIndexHit>& info
) const
{
  info.setSize(samples.size());
  const point bbMid(midpoint());
  FOR_ALL(samples, i)
  {
    info[i] = findNearest(bbMid, samples[i], nearestDistSqr[i]);
  }
}
void mousse::searchableBox::findLine
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  info.setSize(start.size());
  FOR_ALL(start, i)
  {
    info[i] = findLine(start[i], end[i]);
  }
}
void mousse::searchableBox::findLineAny
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  info.setSize(start.size());
  FOR_ALL(start, i)
  {
    info[i] = findLineAny(start[i], end[i]);
  }
}
void mousse::searchableBox::findLineAll
(
  const pointField& start,
  const pointField& end,
  List<List<pointIndexHit> >& info
) const
{
  info.setSize(start.size());
  // Work array
  DynamicList<pointIndexHit, 1, 1> hits;
  // Tolerances:
  // To find all intersections we add a small vector to the last intersection
  // This is chosen such that
  // - it is significant (SMALL is smallest representative relative tolerance;
  //   we need something bigger since we're doing calculations)
  // - if the start-end vector is zero we still progress
  const vectorField dirVec(end-start);
  const scalarField magSqrDirVec(magSqr(dirVec));
  const vectorField smallVec
  (
    ROOTSMALL*dirVec
   + vector(ROOTVSMALL,ROOTVSMALL,ROOTVSMALL)
  );
  FOR_ALL(start, pointI)
  {
    // See if any intersection between pt and end
    pointIndexHit inter = findLine(start[pointI], end[pointI]);
    if (inter.hit())
    {
      hits.clear();
      hits.append(inter);
      point pt = inter.hitPoint() + smallVec[pointI];
      while (((pt-start[pointI])&dirVec[pointI]) <= magSqrDirVec[pointI])
      {
        // See if any intersection between pt and end
        pointIndexHit inter = findLine(pt, end[pointI]);
        // Check for not hit or hit same face as before (can happen
        // if vector along surface of face)
        if
        (
          !inter.hit()
        || (inter.index() == hits.last().index())
        )
        {
          break;
        }
        hits.append(inter);
        pt = inter.hitPoint() + smallVec[pointI];
      }
      info[pointI].transfer(hits);
    }
    else
    {
      info[pointI].clear();
    }
  }
}
void mousse::searchableBox::getRegion
(
  const List<pointIndexHit>& info,
  labelList& region
) const
{
  region.setSize(info.size());
  region = 0;
}
void mousse::searchableBox::getNormal
(
  const List<pointIndexHit>& info,
  vectorField& normal
) const
{
  normal.setSize(info.size());
  normal = vector::zero;
  FOR_ALL(info, i)
  {
    if (info[i].hit())
    {
      normal[i] = treeBoundBox::faceNormals[info[i].index()];
    }
    else
    {
      // Set to what?
    }
  }
}
void mousse::searchableBox::getVolumeType
(
  const pointField& points,
  List<volumeType>& volType
) const
{
  volType.setSize(points.size());
  volType = volumeType::INSIDE;
  FOR_ALL(points, pointI)
  {
    const point& pt = points[pointI];
    for (direction dir = 0; dir < vector::nComponents; dir++)
    {
      if (pt[dir] < min()[dir] || pt[dir] > max()[dir])
      {
        volType[pointI] = volumeType::OUTSIDE;
        break;
      }
    }
  }
}
