// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_tri_surface_mesh.hpp"
#include "mesh_search.hpp"
#include "tuple2.hpp"
#include "global_index.hpp"
#include "tree_data_cell.hpp"
#include "tree_data_face.hpp"
#include "mesh_tools.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(sampledTriSurfaceMesh, 0);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  sampledSurface,
  sampledTriSurfaceMesh,
  word
);

template<>
const char* NamedEnum<sampledTriSurfaceMesh::samplingSource, 3>::names[] =
{
  "cells",
  "insideCells",
  "boundaryFaces"
};

const NamedEnum<sampledTriSurfaceMesh::samplingSource, 3>
sampledTriSurfaceMesh::samplingSourceNames_;

//- Private class for finding nearest
//  Comprising:
//  - global index
//  - sqr(distance)
typedef Tuple2<scalar, label> nearInfo;
class nearestEqOp
{
public:
  void operator()(nearInfo& x, const nearInfo& y) const
  {
    if (y.first() < x.first()) {
      x = y;
    }
  }
};

}


// Private Member Functions 
const mousse::indexedOctree<mousse::treeDataFace>&
mousse::sampledTriSurfaceMesh::nonCoupledboundaryTree() const
{
  // Variant of meshSearch::boundaryTree() that only does non-coupled
  // boundary faces.
  if (!boundaryTreePtr_.valid()) {
    // all non-coupled boundary faces (not just walls)
    const polyBoundaryMesh& patches = mesh().boundaryMesh();
    labelList bndFaces{mesh().nFaces() - mesh().nInternalFaces()};
    label bndI = 0;
    FOR_ALL(patches, patchI) {
      const polyPatch& pp = patches[patchI];
      if (!pp.coupled()) {
        FOR_ALL(pp, i) {
          bndFaces[bndI++] = pp.start()+i;
        }
      }
    }
    bndFaces.setSize(bndI);
    treeBoundBox overallBb{mesh().points()};
    Random rndGen{123456};
    overallBb = overallBb.extend(rndGen, 1e-4);
    overallBb.min() -= point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
    overallBb.max() += point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
    boundaryTreePtr_.reset
    (
      new indexedOctree<treeDataFace>
      {
        {
          false,                      // do not cache bb
          mesh(),
          bndFaces                    // boundary faces only
        },
        overallBb,                      // overall search domain
        8,                              // maxLevel
        10,                             // leafsize
        3.0                             // duplicity
      }
    );
  }
  return boundaryTreePtr_();
}


bool mousse::sampledTriSurfaceMesh::update(const meshSearch& meshSearcher)
{
  // Find the cells the triangles of the surface are in.
  // Does approximation by looking at the face centres only
  const pointField& fc = surface_.faceCentres();
  List<nearInfo> nearest{fc.size()};
  // Global numbering for cells/faces - only used to uniquely identify local
  // elements
  globalIndex globalCells
  {
    (sampleSource_ == cells || sampleSource_ == insideCells)
    ? mesh().nCells()
    : mesh().nFaces()
  };
  FOR_ALL(nearest, i) {
    nearest[i].first() = GREAT;
    nearest[i].second() = labelMax;
  }
  if (sampleSource_ == cells) {
    // Search for nearest cell
    const indexedOctree<treeDataCell>& cellTree = meshSearcher.cellTree();
    FOR_ALL(fc, triI) {
      pointIndexHit nearInfo = cellTree.findNearest
      (
        fc[triI],
        sqr(GREAT)
      );
      if (nearInfo.hit()) {
        nearest[triI].first() = magSqr(nearInfo.hitPoint()-fc[triI]);
        nearest[triI].second() = globalCells.toGlobal(nearInfo.index());
      }
    }
  } else if (sampleSource_ == insideCells) {
    // Search for cell containing point
    const indexedOctree<treeDataCell>& cellTree = meshSearcher.cellTree();
    FOR_ALL(fc, triI) {
      if (cellTree.bb().contains(fc[triI])) {
        label index = cellTree.findInside(fc[triI]);
        if (index != -1) {
          nearest[triI].first() = 0.0;
          nearest[triI].second() = globalCells.toGlobal(index);
        }
      }
    }
  } else {
    //- Search on all non-coupled boundary faces
    const indexedOctree<treeDataFace>& bTree = nonCoupledboundaryTree();
    FOR_ALL(fc, triI) {
      pointIndexHit nearInfo = bTree.findNearest(fc[triI], sqr(GREAT));
      if (nearInfo.hit()) {
        nearest[triI].first() = magSqr(nearInfo.hitPoint()-fc[triI]);
        nearest[triI].second() =
          globalCells.toGlobal(bTree.shapes().faceLabels()[nearInfo.index()]);
      }
    }
  }
  // See which processor has the nearest. Mark and subset
  Pstream::listCombineGather(nearest, nearestEqOp());
  Pstream::listCombineScatter(nearest);
  labelList cellOrFaceLabels{fc.size(), -1};
  label nFound = 0;
  FOR_ALL(nearest, triI) {
    if (nearest[triI].second() == labelMax) {
      // Not found on any processor. How to map?
    } else if (globalCells.isLocal(nearest[triI].second())) {
      cellOrFaceLabels[triI] = globalCells.toLocal(nearest[triI].second());
      nFound++;
    }
  }
  if (debug) {
    Pout << "Local out of faces:" << cellOrFaceLabels.size()
      << " keeping:" << nFound << endl;
  }
  // Now subset the surface. Do not use triSurface::subsetMesh since requires
  // original surface to be in compact numbering.
  const triSurface& s = surface_;
  // Compact to original triangle
  labelList faceMap{s.size()};
  // Compact to original points
  labelList pointMap{s.points().size()};
  // From original point to compact points
  labelList reversePointMap{s.points().size(), -1};

  {
    label newPointI = 0;
    label newFaceI = 0;
    FOR_ALL(s, faceI) {
      if (cellOrFaceLabels[faceI] != -1) {
        faceMap[newFaceI++] = faceI;
        const triSurface::FaceType& f = s[faceI];
        FOR_ALL(f, fp) {
          if (reversePointMap[f[fp]] == -1) {
            pointMap[newPointI] = f[fp];
            reversePointMap[f[fp]] = newPointI++;
          }
        }
      }
    }
    faceMap.setSize(newFaceI);
    pointMap.setSize(newPointI);
  }
  // Subset cellOrFaceLabels
  cellOrFaceLabels = UIndirectList<label>{cellOrFaceLabels, faceMap}();
  // Store any face per point (without using pointFaces())
  labelList pointToFace{pointMap.size()};
  // Create faces and points for subsetted surface
  faceList& faces = this->storedFaces();
  faces.setSize(faceMap.size());
  FOR_ALL(faceMap, i) {
    const triFace& f = s[faceMap[i]];
    triFace newF
    {
      reversePointMap[f[0]],
      reversePointMap[f[1]],
      reversePointMap[f[2]]
    };
    faces[i] = newF.triFaceFace();
    FOR_ALL(newF, fp) {
      pointToFace[newF[fp]] = i;
    }
  }
  this->storedPoints() = pointField(s.points(), pointMap);
  if (debug) {
    print(Pout);
    Pout << endl;
  }
  // Collect the samplePoints and sampleElements
  if (sampledSurface::interpolate()) {
    samplePoints_.setSize(pointMap.size());
    sampleElements_.setSize(pointMap.size(), -1);
    if (sampleSource_ == cells) {
      // samplePoints_   : per surface point a location inside the cell
      // sampleElements_ : per surface point the cell
      FOR_ALL(points(), pointI) {
        const point& pt = points()[pointI];
        label cellI = cellOrFaceLabels[pointToFace[pointI]];
        sampleElements_[pointI] = cellI;
        // Check if point inside cell
        if (mesh().pointInCell(pt, sampleElements_[pointI],
                               meshSearcher.decompMode())) {
          samplePoints_[pointI] = pt;
        } else {
          // Find nearest point on faces of cell
          const cell& cFaces = mesh().cells()[cellI];
          scalar minDistSqr = VGREAT;
          FOR_ALL(cFaces, i) {
            const face& f = mesh().faces()[cFaces[i]];
            pointHit info = f.nearestPoint(pt, mesh().points());
            if (info.distance() < minDistSqr) {
              minDistSqr = info.distance();
              samplePoints_[pointI] = info.rawPoint();
            }
          }
        }
      }
    } else if (sampleSource_ == insideCells) {
      // samplePoints_   : per surface point a location inside the cell
      // sampleElements_ : per surface point the cell
      FOR_ALL(points(), pointI) {
        const point& pt = points()[pointI];
        label cellI = cellOrFaceLabels[pointToFace[pointI]];
        sampleElements_[pointI] = cellI;
        samplePoints_[pointI] = pt;
      }
    } else {
      // samplePoints_   : per surface point a location on the boundary
      // sampleElements_ : per surface point the boundary face containing
      //                   the location
      FOR_ALL(points(), pointI) {
        const point& pt = points()[pointI];
        label faceI = cellOrFaceLabels[pointToFace[pointI]];
        sampleElements_[pointI] = faceI;
        samplePoints_[pointI] =
          mesh().faces()[faceI].nearestPoint(pt, mesh().points()).rawPoint();
      }
    }
  } else {
    // if sampleSource_ == cells:
    //      samplePoints_   : n/a
    //      sampleElements_ : per surface triangle the cell
    // if sampleSource_ == insideCells:
    //      samplePoints_   : n/a
    //      sampleElements_ : -1 or per surface triangle the cell
    // else:
    //      samplePoints_   : n/a
    //      sampleElements_ : per surface triangle the boundary face
    samplePoints_.clear();
    sampleElements_.transfer(cellOrFaceLabels);
  }
  if (debug) {
    this->clearOut();
    OFstream str{mesh().time().path()/"surfaceToMesh.obj"};
    Info << "Dumping correspondence from local surface (points or faces)"
      << " to mesh (cells or faces) to " << str.name() << endl;
    label vertI = 0;
    if (sampledSurface::interpolate()) {
      if (sampleSource_ == cells || sampleSource_ == insideCells) {
        FOR_ALL(samplePoints_, pointI) {
          meshTools::writeOBJ(str, points()[pointI]);
          vertI++;
          meshTools::writeOBJ(str, samplePoints_[pointI]);
          vertI++;
          label cellI = sampleElements_[pointI];
          meshTools::writeOBJ(str, mesh().cellCentres()[cellI]);
          vertI++;
          str << "l " << vertI-2 << ' ' << vertI-1 << ' ' << vertI
            << nl;
        }
      } else {
        FOR_ALL(samplePoints_, pointI) {
          meshTools::writeOBJ(str, points()[pointI]);
          vertI++;
          meshTools::writeOBJ(str, samplePoints_[pointI]);
          vertI++;
          label faceI = sampleElements_[pointI];
          meshTools::writeOBJ(str, mesh().faceCentres()[faceI]);
          vertI++;
          str << "l " << vertI-2 << ' ' << vertI-1 << ' ' << vertI
            << nl;
        }
      }
    } else {
      if (sampleSource_ == cells || sampleSource_ == insideCells) {
        FOR_ALL(sampleElements_, triI) {
          meshTools::writeOBJ(str, faceCentres()[triI]);
          vertI++;
          label cellI = sampleElements_[triI];
          meshTools::writeOBJ(str, mesh().cellCentres()[cellI]);
          vertI++;
          str << "l " << vertI-1 << ' ' << vertI << nl;
        }
      } else {
        FOR_ALL(sampleElements_, triI) {
          meshTools::writeOBJ(str, faceCentres()[triI]);
          vertI++;
          label faceI = sampleElements_[triI];
          meshTools::writeOBJ(str, mesh().faceCentres()[faceI]);
          vertI++;
          str << "l " << vertI-1 << ' ' << vertI << nl;
        }
      }
    }
  }
  needsUpdate_ = false;
  return true;
}


// Constructors 
mousse::sampledTriSurfaceMesh::sampledTriSurfaceMesh
(
  const word& name,
  const polyMesh& mesh,
  const word& surfaceName,
  const samplingSource sampleSource
)
:
  sampledSurface{name, mesh},
  surface_
  {
    {
      surfaceName,
      mesh.time().constant(), // instance
      "triSurface",           // local
      mesh,                   // registry
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    }
  },
  sampleSource_{sampleSource},
  needsUpdate_{true},
  sampleElements_{0},
  samplePoints_{0}
{}


mousse::sampledTriSurfaceMesh::sampledTriSurfaceMesh
(
  const word& name,
  const polyMesh& mesh,
  const dictionary& dict
)
:
  sampledSurface{name, mesh, dict},
  surface_
  {
    {
      dict.lookup("surface"),
      mesh.time().constant(), // instance
      "triSurface",           // local
      mesh,                   // registry
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    }
  },
  sampleSource_(samplingSourceNames_[dict.lookup("source")]),
  needsUpdate_(true),
  sampleElements_(0),
  samplePoints_(0)
{}


mousse::sampledTriSurfaceMesh::sampledTriSurfaceMesh
(
  const word& name,
  const polyMesh& mesh,
  const triSurface& surface,
  const word& sampleSourceName
)
:
  sampledSurface{name, mesh},
  surface_
  {
    {
      name,
      mesh.time().constant(), // instance
      "triSurface",           // local
      mesh,                  // registry
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    surface
  },
  sampleSource_{samplingSourceNames_[sampleSourceName]},
  needsUpdate_{true},
  sampleElements_{0},
  samplePoints_{0}
{}


// Destructor 
mousse::sampledTriSurfaceMesh::~sampledTriSurfaceMesh()
{}


// Member Functions 
bool mousse::sampledTriSurfaceMesh::needsUpdate() const
{
  return needsUpdate_;
}


bool mousse::sampledTriSurfaceMesh::expire()
{
  // already marked as expired
  if (needsUpdate_) {
    return false;
  }
  sampledSurface::clearGeom();
  MeshStorage::clear();
  boundaryTreePtr_.clear();
  sampleElements_.clear();
  samplePoints_.clear();
  needsUpdate_ = true;
  return true;
}


bool mousse::sampledTriSurfaceMesh::update()
{
  if (!needsUpdate_) {
    return false;
  }
  // Mesh search engine, no triangulation of faces.
  meshSearch meshSearcher{mesh(), polyMesh::FACE_PLANES};
  return update(meshSearcher);
}


bool mousse::sampledTriSurfaceMesh::update(const treeBoundBox& bb)
{
  if (!needsUpdate_) {
    return false;
  }
  // Mesh search engine on subset, no triangulation of faces.
  meshSearch meshSearcher{mesh(), bb, polyMesh::FACE_PLANES};
  return update(meshSearcher);
}


mousse::tmp<mousse::scalarField> mousse::sampledTriSurfaceMesh::sample
(
  const volScalarField& vField
) const
{
  return sampleField(vField);
}


mousse::tmp<mousse::vectorField> mousse::sampledTriSurfaceMesh::sample
(
  const volVectorField& vField
) const
{
  return sampleField(vField);
}


mousse::tmp<mousse::sphericalTensorField> mousse::sampledTriSurfaceMesh::sample
(
  const volSphericalTensorField& vField
) const
{
  return sampleField(vField);
}


mousse::tmp<mousse::symmTensorField> mousse::sampledTriSurfaceMesh::sample
(
  const volSymmTensorField& vField
) const
{
  return sampleField(vField);
}


mousse::tmp<mousse::tensorField> mousse::sampledTriSurfaceMesh::sample
(
  const volTensorField& vField
) const
{
  return sampleField(vField);
}


mousse::tmp<mousse::scalarField> mousse::sampledTriSurfaceMesh::interpolate
(
  const interpolation<scalar>& interpolator
) const
{
  return interpolateField(interpolator);
}


mousse::tmp<mousse::vectorField> mousse::sampledTriSurfaceMesh::interpolate
(
  const interpolation<vector>& interpolator
) const
{
  return interpolateField(interpolator);
}


mousse::tmp<mousse::sphericalTensorField> mousse::sampledTriSurfaceMesh::interpolate
(
  const interpolation<sphericalTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}


mousse::tmp<mousse::symmTensorField> mousse::sampledTriSurfaceMesh::interpolate
(
  const interpolation<symmTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}


mousse::tmp<mousse::tensorField> mousse::sampledTriSurfaceMesh::interpolate
(
  const interpolation<tensor>& interpolator
) const
{
  return interpolateField(interpolator);
}


void mousse::sampledTriSurfaceMesh::print(Ostream& os) const
{
  os << "sampledTriSurfaceMesh: " << name() << " :"
    << "  surface:" << surface_.objectRegistry::name()
    << "  faces:" << faces().size()
    << "  points:" << points().size();
}

