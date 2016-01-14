// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "refinement_features.hpp"
#include "time.hpp"
#include "tuple2.hpp"
#include "dynamic_field.hpp"
// Private Member Functions 
void mousse::refinementFeatures::read
(
  const objectRegistry& io,
  const PtrList<dictionary>& featDicts
)
{
  FOR_ALL(featDicts, featI)
  {
    const dictionary& dict = featDicts[featI];
    fileName featFileName(dict.lookup("file"));
    // Try reading extendedEdgeMesh first
    IOobject extFeatObj
    (
      featFileName,                       // name
      io.time().constant(),               // instance
      "extendedFeatureEdgeMesh",          // local
      io.time(),                          // registry
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    );
    const fileName fName(extFeatObj.filePath());
    if (!fName.empty() && extendedEdgeMesh::canRead(fName))
    {
      autoPtr<extendedEdgeMesh> eMeshPtr = extendedEdgeMesh::New
      (
        fName
      );
      Info<< "Read extendedFeatureEdgeMesh " << extFeatObj.name()
        << nl << incrIndent;
      eMeshPtr().writeStats(Info);
      Info<< decrIndent << endl;
      set(featI, new extendedFeatureEdgeMesh(extFeatObj, eMeshPtr()));
    }
    else
    {
      // Try reading edgeMesh
      IOobject featObj
      (
        featFileName,                       // name
        io.time().constant(),               // instance
        "triSurface",                       // local
        io.time(),                          // registry
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
      );
      const fileName fName(featObj.filePath());
      if (fName.empty())
      {
        FATAL_IO_ERROR_IN
        (
          "refinementFeatures::read"
          "(const objectRegistry&"
          ", const PtrList<dictionary>&)",
          dict
        )   << "Could not open " << featObj.objectPath()
          << exit(FatalIOError);
      }
      // Read as edgeMesh
      autoPtr<edgeMesh> eMeshPtr = edgeMesh::New(fName);
      const edgeMesh& eMesh = eMeshPtr();
      Info<< "Read edgeMesh " << featObj.name() << nl
        << incrIndent;
      eMesh.writeStats(Info);
      Info<< decrIndent << endl;
      // Analyse for feature points. These are all classified as mixed
      // points for lack of anything better
      const labelListList& pointEdges = eMesh.pointEdges();
      labelList oldToNew(eMesh.points().size(), -1);
      DynamicField<point> newPoints(eMesh.points().size());
      FOR_ALL(pointEdges, pointI)
      {
        if (pointEdges[pointI].size() > 2)
        {
          oldToNew[pointI] = newPoints.size();
          newPoints.append(eMesh.points()[pointI]);
        }
        //else if (pointEdges[pointI].size() == 2)
        //MEJ: do something based on a feature angle?
      }
      label nFeatures = newPoints.size();
      FOR_ALL(oldToNew, pointI)
      {
        if (oldToNew[pointI] == -1)
        {
          oldToNew[pointI] = newPoints.size();
          newPoints.append(eMesh.points()[pointI]);
        }
      }
      const edgeList& edges = eMesh.edges();
      edgeList newEdges(edges.size());
      FOR_ALL(edges, edgeI)
      {
        const edge& e = edges[edgeI];
        newEdges[edgeI] = edge
        (
          oldToNew[e[0]],
          oldToNew[e[1]]
        );
      }
      // Construct an extendedEdgeMesh with
      // - all points on more than 2 edges : mixed feature points
      // - all edges : external edges
      extendedEdgeMesh eeMesh
      (
        newPoints,          // pts
        newEdges,           // eds
        0,                  // (point) concaveStart
        0,                  // (point) mixedStart
        nFeatures,          // (point) nonFeatureStart
        edges.size(),       // (edge) internalStart
        edges.size(),       // (edge) flatStart
        edges.size(),       // (edge) openStart
        edges.size(),       // (edge) multipleStart
        vectorField(0),     // normals
        List<extendedEdgeMesh::sideVolumeType>(0),// normalVolumeTypes
        vectorField(0),     // edgeDirections
        labelListList(0),   // normalDirections
        labelListList(0),   // edgeNormals
        labelListList(0),   // featurePointNormals
        labelListList(0),   // featurePointEdges
        identity(newEdges.size())   // regionEdges
      );
      //Info<< "Constructed extendedFeatureEdgeMesh " << featObj.name()
      //    << nl << incrIndent;
      //eeMesh.writeStats(Info);
      //Info<< decrIndent << endl;
      set(featI, new extendedFeatureEdgeMesh(featObj, eeMesh));
    }
    const extendedEdgeMesh& eMesh = operator[](featI);
    if (dict.found("levels"))
    {
      List<Tuple2<scalar, label> > distLevels(dict["levels"]);
      if (dict.size() < 1)
      {
        FATAL_ERROR_IN
        (
          "refinementFeatures::read"
          "(const objectRegistry&"
          ", const PtrList<dictionary>&)"
        )   << " : levels should be at least size 1" << endl
          << "levels : "  << dict["levels"]
          << exit(FatalError);
      }
      distances_[featI].setSize(distLevels.size());
      levels_[featI].setSize(distLevels.size());
      FOR_ALL(distLevels, j)
      {
        distances_[featI][j] = distLevels[j].first();
        levels_[featI][j] = distLevels[j].second();
        // Check in incremental order
        if (j > 0)
        {
          if
          (
            (distances_[featI][j] <= distances_[featI][j-1])
          || (levels_[featI][j] > levels_[featI][j-1])
          )
          {
            FATAL_ERROR_IN
            (
              "refinementFeatures::read"
              "(const objectRegistry&"
              ", const PtrList<dictionary>&)"
            )   << " : Refinement should be specified in order"
              << " of increasing distance"
              << " (and decreasing refinement level)." << endl
              << "Distance:" << distances_[featI][j]
              << " refinementLevel:" << levels_[featI][j]
              << exit(FatalError);
          }
        }
      }
    }
    else
    {
      // Look up 'level' for single level
      levels_[featI] = labelList(1, readLabel(dict.lookup("level")));
      distances_[featI] = scalarField(1, 0.0);
    }
    Info<< "Refinement level according to distance to "
      << featFileName << " (" << eMesh.points().size() << " points, "
      << eMesh.edges().size() << " edges)." << endl;
    FOR_ALL(levels_[featI], j)
    {
      Info<< "    level " << levels_[featI][j]
        << " for all cells within " << distances_[featI][j]
        << " metre." << endl;
    }
  }
}
void mousse::refinementFeatures::buildTrees(const label featI)
{
  const extendedEdgeMesh& eMesh = operator[](featI);
  const pointField& points = eMesh.points();
  const edgeList& edges = eMesh.edges();
  // Calculate bb of all points
  treeBoundBox bb(points);
  // Random number generator. Bit dodgy since not exactly random ;-)
  Random rndGen(65431);
  // Slightly extended bb. Slightly off-centred just so on symmetric
  // geometry there are less face/edge aligned items.
  bb = bb.extend(rndGen, 1e-4);
  bb.min() -= point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
  bb.max() += point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
  edgeTrees_.set
  (
    featI,
    new indexedOctree<treeDataEdge>
    (
      treeDataEdge
      (
        false,                  // do not cache bb
        edges,
        points,
        identity(edges.size())
      ),
      bb,     // overall search domain
      8,      // maxLevel
      10,     // leafsize
      3.0     // duplicity
    )
  );
  labelList featurePoints(identity(eMesh.nonFeatureStart()));
  pointTrees_.set
  (
    featI,
    new indexedOctree<treeDataPoint>
    (
      treeDataPoint(points, featurePoints),
      bb,     // overall search domain
      8,      // maxLevel
      10,     // leafsize
      3.0     // duplicity
    )
  );
}
const mousse::PtrList<mousse::indexedOctree<mousse::treeDataEdge> >&
mousse::refinementFeatures::regionEdgeTrees() const
{
  if (!regionEdgeTreesPtr_.valid())
  {
    regionEdgeTreesPtr_.reset
    (
      new PtrList<indexedOctree<treeDataEdge> >(size())
    );
    PtrList<indexedOctree<treeDataEdge> >& trees = regionEdgeTreesPtr_();
    FOR_ALL(*this, featI)
    {
      const extendedEdgeMesh& eMesh = operator[](featI);
      const pointField& points = eMesh.points();
      const edgeList& edges = eMesh.edges();
      // Calculate bb of all points
      treeBoundBox bb(points);
      // Random number generator. Bit dodgy since not exactly random ;-)
      Random rndGen(65431);
      // Slightly extended bb. Slightly off-centred just so on symmetric
      // geometry there are less face/edge aligned items.
      bb = bb.extend(rndGen, 1e-4);
      bb.min() -= point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
      bb.max() += point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
      trees.set
      (
        featI,
        new indexedOctree<treeDataEdge>
        (
          treeDataEdge
          (
            false,                  // do not cache bb
            edges,
            points,
            eMesh.regionEdges()
          ),
          bb,     // overall search domain
          8,      // maxLevel
          10,     // leafsize
          3.0     // duplicity
        )
      );
    }
  }
  return regionEdgeTreesPtr_();
}
// Find maximum level of a shell.
void mousse::refinementFeatures::findHigherLevel
(
  const pointField& pt,
  const label featI,
  labelList& maxLevel
) const
{
  const labelList& levels = levels_[featI];
  const scalarField& distances = distances_[featI];
  // Collect all those points that have a current maxLevel less than
  // (any of) the shell. Also collect the furthest distance allowable
  // to any shell with a higher level.
  pointField candidates(pt.size());
  labelList candidateMap(pt.size());
  scalarField candidateDistSqr(pt.size());
  label candidateI = 0;
  FOR_ALL(maxLevel, pointI)
  {
    FOR_ALL_REVERSE(levels, levelI)
    {
      if (levels[levelI] > maxLevel[pointI])
      {
        candidates[candidateI] = pt[pointI];
        candidateMap[candidateI] = pointI;
        candidateDistSqr[candidateI] = sqr(distances[levelI]);
        candidateI++;
        break;
      }
    }
  }
  candidates.setSize(candidateI);
  candidateMap.setSize(candidateI);
  candidateDistSqr.setSize(candidateI);
  // Do the expensive nearest test only for the candidate points.
  const indexedOctree<treeDataEdge>& tree = edgeTrees_[featI];
  List<pointIndexHit> nearInfo(candidates.size());
  FOR_ALL(candidates, candidateI)
  {
    nearInfo[candidateI] = tree.findNearest
    (
      candidates[candidateI],
      candidateDistSqr[candidateI]
    );
  }
  // Update maxLevel
  FOR_ALL(nearInfo, candidateI)
  {
    if (nearInfo[candidateI].hit())
    {
      // Check which level it actually is in.
      label minDistI = findLower
      (
        distances,
        mag(nearInfo[candidateI].hitPoint()-candidates[candidateI])
      );
      label pointI = candidateMap[candidateI];
      // pt is inbetween shell[minDistI] and shell[minDistI+1]
      maxLevel[pointI] = levels[minDistI+1];
    }
  }
}
// Constructors 
mousse::refinementFeatures::refinementFeatures
(
  const objectRegistry& io,
  const PtrList<dictionary>& featDicts
)
:
  PtrList<extendedFeatureEdgeMesh>(featDicts.size()),
  distances_(featDicts.size()),
  levels_(featDicts.size()),
  edgeTrees_(featDicts.size()),
  pointTrees_(featDicts.size())
{
  // Read features
  read(io, featDicts);
  // Search engines
  FOR_ALL(*this, i)
  {
    buildTrees(i);
  }
}
//mousse::refinementFeatures::refinementFeatures
//(
//    const objectRegistry& io,
//    const PtrList<dictionary>& featDicts,
//    const scalar minCos
//)
//:
//    PtrList<extendedFeatureEdgeMesh>(featDicts.size()),
//    distances_(featDicts.size()),
//    levels_(featDicts.size()),
//    edgeTrees_(featDicts.size()),
//    pointTrees_(featDicts.size())
//{
//    // Read features
//    read(io, featDicts);
//
//    // Search engines
//    FOR_ALL(*this, i)
//    {
//        const edgeMesh& eMesh = operator[](i);
//        const pointField& points = eMesh.points();
//        const edgeList& edges = eMesh.edges();
//        const labelListList& pointEdges = eMesh.pointEdges();
//
//        DynamicList<label> featurePoints;
//        FOR_ALL(pointEdges, pointI)
//        {
//            const labelList& pEdges = pointEdges[pointI];
//            if (pEdges.size() > 2)
//            {
//                featurePoints.append(pointI);
//            }
//            else if (pEdges.size() == 2)
//            {
//                // Check the angle
//                const edge& e0 = edges[pEdges[0]];
//                const edge& e1 = edges[pEdges[1]];
//
//                const point& p = points[pointI];
//                const point& p0 = points[e0.otherVertex(pointI)];
//                const point& p1 = points[e1.otherVertex(pointI)];
//
//                vector v0 = p-p0;
//                scalar v0Mag = mag(v0);
//
//                vector v1 = p1-p;
//                scalar v1Mag = mag(v1);
//
//                if
//                (
//                    v0Mag > SMALL
//                 && v1Mag > SMALL
//                 && ((v0/v0Mag & v1/v1Mag) < minCos)
//                )
//                {
//                    featurePoints.append(pointI);
//                }
//            }
//        }
//
//        Info<< "Detected " << featurePoints.size()
//            << " featurePoints out of " << points.size()
//            << " points on feature " << i   //eMesh.name()
//            << " when using feature cos " << minCos << endl;
//
//        buildTrees(i, featurePoints);
//    }
//}
// Member Functions 
void mousse::refinementFeatures::findNearestEdge
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  labelList& nearFeature,
  List<pointIndexHit>& nearInfo,
  vectorField& nearNormal
) const
{
  nearFeature.setSize(samples.size());
  nearFeature = -1;
  nearInfo.setSize(samples.size());
  nearInfo = pointIndexHit();
  nearNormal.setSize(samples.size());
  nearNormal = vector::zero;
  FOR_ALL(edgeTrees_, featI)
  {
    const indexedOctree<treeDataEdge>& tree = edgeTrees_[featI];
    if (tree.shapes().size() > 0)
    {
      FOR_ALL(samples, sampleI)
      {
        const point& sample = samples[sampleI];
        scalar distSqr;
        if (nearInfo[sampleI].hit())
        {
          distSqr = magSqr(nearInfo[sampleI].hitPoint()-sample);
        }
        else
        {
          distSqr = nearestDistSqr[sampleI];
        }
        pointIndexHit info = tree.findNearest(sample, distSqr);
        if (info.hit())
        {
          nearFeature[sampleI] = featI;
          nearInfo[sampleI] = pointIndexHit
          (
            info.hit(),
            info.hitPoint(),
            tree.shapes().edgeLabels()[info.index()]
          );
          const treeDataEdge& td = tree.shapes();
          const edge& e = td.edges()[nearInfo[sampleI].index()];
          nearNormal[sampleI] =  e.vec(td.points());
          nearNormal[sampleI] /= mag(nearNormal[sampleI])+VSMALL;
        }
      }
    }
  }
}
void mousse::refinementFeatures::findNearestRegionEdge
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  labelList& nearFeature,
  List<pointIndexHit>& nearInfo,
  vectorField& nearNormal
) const
{
  nearFeature.setSize(samples.size());
  nearFeature = -1;
  nearInfo.setSize(samples.size());
  nearInfo = pointIndexHit();
  nearNormal.setSize(samples.size());
  nearNormal = vector::zero;
  const PtrList<indexedOctree<treeDataEdge> >& regionTrees =
    regionEdgeTrees();
  FOR_ALL(regionTrees, featI)
  {
    const indexedOctree<treeDataEdge>& regionTree = regionTrees[featI];
    FOR_ALL(samples, sampleI)
    {
      const point& sample = samples[sampleI];
      scalar distSqr;
      if (nearInfo[sampleI].hit())
      {
        distSqr = magSqr(nearInfo[sampleI].hitPoint()-sample);
      }
      else
      {
        distSqr = nearestDistSqr[sampleI];
      }
      // Find anything closer than current best
      pointIndexHit info = regionTree.findNearest(sample, distSqr);
      if (info.hit())
      {
        const treeDataEdge& td = regionTree.shapes();
        nearFeature[sampleI] = featI;
        nearInfo[sampleI] = pointIndexHit
        (
          info.hit(),
          info.hitPoint(),
          regionTree.shapes().edgeLabels()[info.index()]
        );
        const edge& e = td.edges()[nearInfo[sampleI].index()];
        nearNormal[sampleI] =  e.vec(td.points());
        nearNormal[sampleI] /= mag(nearNormal[sampleI])+VSMALL;
      }
    }
  }
}
//void mousse::refinementFeatures::findNearestPoint
//(
//    const pointField& samples,
//    const scalarField& nearestDistSqr,
//    labelList& nearFeature,
//    labelList& nearIndex
//) const
//{
//    nearFeature.setSize(samples.size());
//    nearFeature = -1;
//    nearIndex.setSize(samples.size());
//    nearIndex = -1;
//
//    FOR_ALL(pointTrees_, featI)
//    {
//        const indexedOctree<treeDataPoint>& tree = pointTrees_[featI];
//
//        if (tree.shapes().pointLabels().size() > 0)
//        {
//            FOR_ALL(samples, sampleI)
//            {
//                const point& sample = samples[sampleI];
//
//                scalar distSqr;
//                if (nearFeature[sampleI] != -1)
//                {
//                    label nearFeatI = nearFeature[sampleI];
//                    const indexedOctree<treeDataPoint>& nearTree =
//                        pointTrees_[nearFeatI];
//                    label featPointI =
//                        nearTree.shapes().pointLabels()[nearIndex[sampleI]];
//                    const point& featPt =
//                        operator[](nearFeatI).points()[featPointI];
//                    distSqr = magSqr(featPt-sample);
//                }
//                else
//                {
//                    distSqr = nearestDistSqr[sampleI];
//                }
//
//                pointIndexHit info = tree.findNearest(sample, distSqr);
//
//                if (info.hit())
//                {
//                    nearFeature[sampleI] = featI;
//                    nearIndex[sampleI] = info.index();
//                }
//            }
//        }
//    }
//}
void mousse::refinementFeatures::findNearestPoint
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  labelList& nearFeature,
  List<pointIndexHit>& nearInfo
) const
{
  nearFeature.setSize(samples.size());
  nearFeature = -1;
  nearInfo.setSize(samples.size());
  nearInfo = pointIndexHit();
  FOR_ALL(pointTrees_, featI)
  {
    const indexedOctree<treeDataPoint>& tree = pointTrees_[featI];
    if (tree.shapes().pointLabels().size() > 0)
    {
      FOR_ALL(samples, sampleI)
      {
        const point& sample = samples[sampleI];
        scalar distSqr;
        if (nearFeature[sampleI] != -1)
        {
          distSqr = magSqr(nearInfo[sampleI].hitPoint()-sample);
        }
        else
        {
          distSqr = nearestDistSqr[sampleI];
        }
        pointIndexHit info = tree.findNearest(sample, distSqr);
        if (info.hit())
        {
          nearFeature[sampleI] = featI;
          nearInfo[sampleI] = pointIndexHit
          (
            info.hit(),
            info.hitPoint(),
            tree.shapes().pointLabels()[info.index()]
          );
        }
      }
    }
  }
}
void mousse::refinementFeatures::findHigherLevel
(
  const pointField& pt,
  const labelList& ptLevel,
  labelList& maxLevel
) const
{
  // Maximum level of any shell. Start off with level of point.
  maxLevel = ptLevel;
  FOR_ALL(*this, featI)
  {
    findHigherLevel(pt, featI, maxLevel);
  }
}
mousse::scalar mousse::refinementFeatures::maxDistance() const
{
  scalar overallMax = -GREAT;
  FOR_ALL(distances_, featI)
  {
    overallMax = max(overallMax, max(distances_[featI]));
  }
  return overallMax;
}
