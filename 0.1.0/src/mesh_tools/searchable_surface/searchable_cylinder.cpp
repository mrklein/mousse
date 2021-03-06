// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_cylinder.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(searchableCylinder, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(searchableSurface, searchableCylinder, dict);

}


// Private Member Functions 
mousse::tmp<mousse::pointField> mousse::searchableCylinder::coordinates() const
{
  tmp<pointField> tCtrs{new pointField{1, 0.5*(point1_ + point2_)}};
  return tCtrs;
}


void mousse::searchableCylinder::boundingSpheres
(
  pointField& centres,
  scalarField& radiusSqr
) const
{
  centres.setSize(1);
  centres[0] = 0.5*(point1_ + point2_);
  radiusSqr.setSize(1);
  radiusSqr[0] = mousse::magSqr(point1_-centres[0]) + mousse::sqr(radius_);
  // Add a bit to make sure all points are tested inside
  radiusSqr += mousse::sqr(SMALL);
}


mousse::tmp<mousse::pointField> mousse::searchableCylinder::points() const
{
  tmp<pointField> tPts{new pointField{2}};
  pointField& pts = tPts();
  pts[0] = point1_;
  pts[1] = point2_;
  return tPts;
}


mousse::pointIndexHit mousse::searchableCylinder::findNearest
(
  const point& sample,
  const scalar nearestDistSqr
) const
{
  pointIndexHit info{false, sample, -1};
  vector v{sample - point1_};
  // Decompose sample-point1 into normal and parallel component
  scalar parallel = (v & unitDir_);
  // Remove the parallel component and normalise
  v -= parallel*unitDir_;
  scalar magV = mag(v);
  if (magV < ROOTVSMALL) {
    v = vector::zero;
  } else {
    v /= magV;
  }
  if (parallel <= 0) {
    // nearest is at point1 end cap. Clip to radius.
    info.setPoint(point1_ + min(magV, radius_)*v);
  } else if (parallel >= magDir_) {
    // nearest is at point2 end cap. Clip to radius.
    info.setPoint(point2_ + min(magV, radius_)*v);
  } else {
    // inbetween endcaps. Might either be nearer endcaps or cylinder wall
    // distance to endpoint: parallel or parallel-magDir
    // distance to cylinder wall: magV-radius_
    // Nearest cylinder point
    point cylPt;
    if (magV < ROOTVSMALL) {
      // Point exactly on centre line. Take any point on wall.
      vector e1 = point(1,0,0) ^ unitDir_;
      scalar magE1 = mag(e1);
      if (magE1 < SMALL) {
        e1 = point{0,1,0} ^ unitDir_;
        magE1 = mag(e1);
      }
      e1 /= magE1;
      cylPt = sample + radius_*e1;
    } else {
      cylPt = sample + (radius_-magV)*v;
    }
    if (parallel < 0.5*magDir_) {
      // Project onto p1 endcap
      point end1Pt = point1_ + min(magV, radius_)*v;
      if (magSqr(sample-cylPt) < magSqr(sample-end1Pt)) {
        info.setPoint(cylPt);
      } else {
        info.setPoint(end1Pt);
      }
    } else {
      // Project onto p2 endcap
      point end2Pt = point2_ + min(magV, radius_)*v;
      if (magSqr(sample-cylPt) < magSqr(sample-end2Pt)) {
        info.setPoint(cylPt);
      } else {
        info.setPoint(end2Pt);
      }
    }
  }
  if (magSqr(sample - info.rawPoint()) < nearestDistSqr) {
    info.setHit();
    info.setIndex(0);
  }
  return info;
}


mousse::scalar mousse::searchableCylinder::radius2(const point& pt) const
{
  const vector x = (pt-point1_) ^ unitDir_;
  return x&x;
}


// From http://www.gamedev.net/community/forums/topic.asp?topic_id=467789 -
// intersection of cylinder with ray
void mousse::searchableCylinder::findLineAll
(
  const point& start,
  const point& end,
  pointIndexHit& near,
  pointIndexHit& far
) const
{
  near.setMiss();
  far.setMiss();
  vector point1Start{start-point1_};
  vector point2Start{start-point2_};
  vector point1End{end-point1_};
  // Quick rejection of complete vector outside endcaps
  scalar s1 = point1Start&unitDir_;
  scalar s2 = point1End&unitDir_;
  if ((s1 < 0 && s2 < 0) || (s1 > magDir_ && s2 > magDir_)) {
    return;
  }
  // Line as P = start+t*V  where V is unit vector and t=[0..mag(end-start)]
  vector V{end - start};
  scalar magV = mag(V);
  if (magV < ROOTVSMALL) {
    return;
  }
  V /= magV;
  // We now get the nearest intersections to start. This can either be
  // the intersection with the end plane or with the cylinder side.
  // Get the two points (expressed in t) on the end planes. This is to
  // clip any cylinder intersection against.
  scalar tPoint1;
  scalar tPoint2;
  // Maintain the two intersections with the endcaps
  scalar tNear = VGREAT;
  scalar tFar = VGREAT;

  {
    scalar s = (V&unitDir_);
    if (mag(s) > VSMALL) {
      tPoint1 = -s1/s;
      tPoint2 = -(point2Start&unitDir_)/s;
      if (tPoint2 < tPoint1) {
        Swap(tPoint1, tPoint2);
      }
      if (tPoint1 > magV || tPoint2 < 0) {
        return;
      }
      // See if the points on the endcaps are actually inside the cylinder
      if (tPoint1 >= 0 && tPoint1 <= magV) {
        if (radius2(start+tPoint1*V) <= sqr(radius_)) {
          tNear = tPoint1;
        }
      }
      if (tPoint2 >= 0 && tPoint2 <= magV) {
        if (radius2(start+tPoint2*V) <= sqr(radius_)) {
          // Check if already have a near hit from point1
          if (tNear <= 1) {
            tFar = tPoint2;
          } else {
            tNear = tPoint2;
          }
        }
      }
    } else {
      // Vector perpendicular to cylinder. Check for outside already done
      // above so just set tpoint to allow all.
      tPoint1 = -VGREAT;
      tPoint2 = VGREAT;
    }
  }

  const vector x = point1Start ^ unitDir_;
  const vector y = V ^ unitDir_;
  const scalar d = sqr(radius_);
  // Second order equation of the form a*t^2 + b*t + c
  const scalar a = (y&y);
  const scalar b = 2*(x&y);
  const scalar c = (x&x)-d;
  const scalar disc = b*b-4*a*c;
  scalar t1 = -VGREAT;
  scalar t2 = VGREAT;
  if (disc < 0) {
    // Fully outside
    return;
  } else if (disc < ROOTVSMALL) {
    // Single solution
    if (mag(a) > ROOTVSMALL) {
      t1 = -b/(2*a);
      if (t1 >= 0 && t1 <= magV && t1 >= tPoint1 && t1 <= tPoint2) {
        // valid. Insert sorted.
        if (t1 < tNear) {
          tFar = tNear;
          tNear = t1;
        } else if (t1 < tFar) {
          tFar = t1;
        }
      } else {
        return;
      }
    } else {
      // Aligned with axis. Check if outside radius
      if (c > 0) {
        return;
      }
    }
  } else {
    if (mag(a) > ROOTVSMALL) {
      scalar sqrtDisc = sqrt(disc);
      t1 = (-b - sqrtDisc)/(2*a);
      t2 = (-b + sqrtDisc)/(2*a);
      if (t2 < t1) {
        Swap(t1, t2);
      }
      if (t1 >= 0 && t1 <= magV && t1 >= tPoint1 && t1 <= tPoint2) {
        // valid. Insert sorted.
        if (t1 < tNear) {
          tFar = tNear;
          tNear = t1;
        } else if (t1 < tFar) {
          tFar = t1;
        }
      }
      if (t2 >= 0 && t2 <= magV && t2 >= tPoint1 && t2 <= tPoint2) {
        // valid. Insert sorted.
        if (t2 < tNear) {
          tFar = tNear;
          tNear = t2;
        } else if (t2 < tFar) {
          tFar = t2;
        }
      }
    } else {
      if (c > 0) {
        return;
      }
    }
  }

  // Check tNear, tFar
  if (tNear >= 0 && tNear <= magV) {
    near.setPoint(start+tNear*V);
    near.setHit();
    near.setIndex(0);
    if (tFar <= magV) {
      far.setPoint(start+tFar*V);
      far.setHit();
      far.setIndex(0);
    }
  } else if (tFar >= 0 && tFar <= magV) {
    near.setPoint(start+tFar*V);
    near.setHit();
    near.setIndex(0);
  }
}


mousse::boundBox mousse::searchableCylinder::calcBounds() const
{
  // Adapted from
  // http://www.gamedev.net/community/forums
  //       /topic.asp?topic_id=338522&forum_id=20&gforum_id=0
  // Let cylinder have end points A,B and radius r,
  // Bounds in direction X (same for Y and Z) can be found as:
  // Let A.X<B.X (otherwise swap points)
  // Good approximate lowest bound is A.X-r and highest is B.X+r (precise for
  // capsule). At worst, in one direction it can be larger than needed by 2*r.
  // Accurate bounds for cylinder is
  // A.X-kx*r, B.X+kx*r
  // where
  // kx=sqrt(((A.Y-B.Y)^2+(A.Z-B.Z)^2)/((A.X-B.X)^2+(A.Y-B.Y)^2+(A.Z-B.Z)^2))
  // similar thing for Y and Z
  // (i.e.
  // ky=sqrt(((A.X-B.X)^2+(A.Z-B.Z)^2)/((A.X-B.X)^2+(A.Y-B.Y)^2+(A.Z-B.Z)^2))
  // kz=sqrt(((A.X-B.X)^2+(A.Y-B.Y)^2)/((A.X-B.X)^2+(A.Y-B.Y)^2+(A.Z-B.Z)^2))
  // )
  // How derived: geometric reasoning. Bounds of cylinder is same as for 2
  // circles centered on A and B. This sqrt thingy gives sine of angle between
  // axis and direction, used to find projection of radius.
  vector kr
  {
    sqrt(sqr(unitDir_.y()) + sqr(unitDir_.z())),
    sqrt(sqr(unitDir_.x()) + sqr(unitDir_.z())),
    sqrt(sqr(unitDir_.x()) + sqr(unitDir_.y()))
  };
  kr *= radius_;
  point min = point1_ - kr;
  point max = point1_ + kr;
  min = ::mousse::min(min, point2_ - kr);
  max = ::mousse::max(max, point2_ + kr);
  return boundBox(min, max);
}


// Constructors 
mousse::searchableCylinder::searchableCylinder
(
  const IOobject& io,
  const point& point1,
  const point& point2,
  const scalar radius
)
:
  searchableSurface{io},
  point1_{point1},
  point2_{point2},
  magDir_{mag(point2_-point1_)},
  unitDir_{(point2_-point1_)/magDir_},
  radius_{radius}
{
  bounds() = calcBounds();
}


mousse::searchableCylinder::searchableCylinder
(
  const IOobject& io,
  const dictionary& dict
)
:
  searchableSurface{io},
  point1_{dict.lookup("point1")},
  point2_{dict.lookup("point2")},
  magDir_{mag(point2_-point1_)},
  unitDir_{(point2_-point1_)/magDir_},
  radius_{readScalar(dict.lookup("radius"))}
{
  bounds() = calcBounds();
}


// Destructor 
mousse::searchableCylinder::~searchableCylinder()
{}


// Member Functions 
const mousse::wordList& mousse::searchableCylinder::regions() const
{
  if (regions_.empty()) {
    regions_.setSize(1);
    regions_[0] = "region0";
  }
  return regions_;
}


void mousse::searchableCylinder::findNearest
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  List<pointIndexHit>& info
) const
{
  info.setSize(samples.size());
  FOR_ALL(samples, i) {
    info[i] = findNearest(samples[i], nearestDistSqr[i]);
  }
}


void mousse::searchableCylinder::findLine
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  info.setSize(start.size());
  FOR_ALL(start, i) {
    // Pick nearest intersection. If none intersected take second one.
    pointIndexHit b;
    findLineAll(start[i], end[i], info[i], b);
    if (!info[i].hit() && b.hit()) {
      info[i] = b;
    }
  }
}


void mousse::searchableCylinder::findLineAny
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  info.setSize(start.size());
  FOR_ALL(start, i) {
    // Discard far intersection
    pointIndexHit b;
    findLineAll(start[i], end[i], info[i], b);
    if (!info[i].hit() && b.hit()) {
      info[i] = b;
    }
  }
}


void mousse::searchableCylinder::findLineAll
(
  const pointField& start,
  const pointField& end,
  List<List<pointIndexHit> >& info
) const
{
  info.setSize(start.size());
  FOR_ALL(start, i) {
    pointIndexHit near, far;
    findLineAll(start[i], end[i], near, far);
    if (near.hit()) {
      if (far.hit()) {
        info[i].setSize(2);
        info[i][0] = near;
        info[i][1] = far;
      } else {
        info[i].setSize(1);
        info[i][0] = near;
      }
    } else {
      if (far.hit()) {
        info[i].setSize(1);
        info[i][0] = far;
      } else {
        info[i].clear();
      }
    }
  }
}


void mousse::searchableCylinder::getRegion
(
  const List<pointIndexHit>& info,
  labelList& region
) const
{
  region.setSize(info.size());
  region = 0;
}


void mousse::searchableCylinder::getNormal
(
  const List<pointIndexHit>& info,
  vectorField& normal
) const
{
  normal.setSize(info.size());
  normal = vector::zero;
  FOR_ALL(info, i) {
    if (info[i].hit()) {
      vector v{info[i].hitPoint() - point1_};
      // Decompose sample-point1 into normal and parallel component
      scalar parallel = (v & unitDir_);
      // Remove the parallel component and normalise
      v -= parallel*unitDir_;
      scalar magV = mag(v);
      if (parallel <= 0) {
        if ((magV-radius_) < mag(parallel)) {
          // either above endcap (magV<radius) or outside but closer
          normal[i] = -unitDir_;
        } else {
          normal[i] = v/magV;
        }
      } else if (parallel <= 0.5*magDir_) {
        // See if endcap closer or sidewall
        if (magV >= radius_ || (radius_-magV) < parallel) {
          normal[i] = v/magV;
        } else {
          // closer to endcap
          normal[i] = -unitDir_;
        }
      } else if (parallel <= magDir_) {
        // See if endcap closer or sidewall
        if (magV >= radius_ || (radius_-magV) < (magDir_-parallel)) {
          normal[i] = v/magV;
        } else {
          // closer to endcap
          normal[i] = unitDir_;
        }
      } else {   // beyond cylinder
        if ((magV-radius_) < (parallel-magDir_)) {
          // above endcap
          normal[i] = unitDir_;
        } else {
          normal[i] = v/magV;
        }
      }
    }
  }
}


void mousse::searchableCylinder::getVolumeType
(
  const pointField& points,
  List<volumeType>& volType
) const
{
  volType.setSize(points.size());
  volType = volumeType::INSIDE;
  FOR_ALL(points, pointI) {
    const point& pt = points[pointI];
    vector v{pt - point1_};
    // Decompose sample-point1 into normal and parallel component
    scalar parallel = v & unitDir_;
    if (parallel < 0) {
      // left of point1 endcap
      volType[pointI] = volumeType::OUTSIDE;
    } else if (parallel > magDir_) {
      // right of point2 endcap
      volType[pointI] = volumeType::OUTSIDE;
    } else {
      // Remove the parallel component
      v -= parallel*unitDir_;
      if (mag(v) > radius_) {
        volType[pointI] = volumeType::OUTSIDE;
      } else {
        volType[pointI] = volumeType::INSIDE;
      }
    }
  }
}

