// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "searchable_surfaces.hpp"
#include "conformation_surfaces.hpp"
#include "tri_surface_mesh.hpp"
#include "marching_cubes.hpp"
using namespace mousse;
// Main program:
int main(int argc, char *argv[])
{
  argList::addNote
  (
    "Re-sample surfaces used in foamyHexMesh operation"
  );
  //argList::validArgs.append("inputFile");
  argList::validArgs.append("(nx ny nz)");
  argList::validArgs.append("outputName");
  #include "set_root_case.hpp"
  #include "create_time.hpp"
  runTime.functionObjects().off();
  const Vector<label> n(IStringStream(args.args()[1])());
  const fileName exportName = args.args()[2];
  Info<< "Reading surfaces as specified in the foamyHexMeshDict and"
    << " writing re-sampled " << n << " to " << exportName
    << nl << endl;
  cpuTime timer;
  IOdictionary foamyHexMeshDict
  (
    IOobject
    (
      "foamyHexMeshDict",
      runTime.system(),
      runTime,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    )
  );
  // Define/load all geometry
  searchableSurfaces allGeometry
  (
    IOobject
    (
      "cvSearchableSurfaces",
      runTime.constant(),
      "triSurface",
      runTime,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    ),
    foamyHexMeshDict.subDict("geometry"),
    foamyHexMeshDict.lookupOrDefault("singleRegionName", true)
  );
  Info<< "Geometry read in = "
    << timer.cpuTimeIncrement() << " s." << nl << endl;
  Random rndGen(64293*Pstream::myProcNo());
  conformationSurfaces geometryToConformTo
  (
    runTime,
    rndGen,
    allGeometry,
    foamyHexMeshDict.subDict("surfaceConformation")
  );
  Info<< "Set up geometry in = "
    << timer.cpuTimeIncrement() << " s." << nl << endl;
  // Extend
  treeBoundBox bb = geometryToConformTo.globalBounds();
  {
    const vector smallVec = 0.1*bb.span();
    bb.min() -= smallVec;
    bb.max() += smallVec;
  }
  Info<< "Meshing to bounding box " << bb << nl << endl;
  const vector span(bb.span());
  const vector d
  (
    span.x()/(n.x()-1),
    span.y()/(n.y()-1),
    span.z()/(n.z()-1)
  );
  MarchingCubes mc(span.x(), span.y(), span.z() ) ;
  mc.set_resolution(n.x(), n.y(), n.z());
  mc.init_all() ;
  // Generate points
  pointField points(mc.size_x()*mc.size_y()*mc.size_z());
  label pointI = 0;
  point pt;
  for( int k = 0 ; k < mc.size_z() ; k++ )
  {
    pt.z() = bb.min().z() + k*d.z();
    for( int j = 0 ; j < mc.size_y() ; j++ )
    {
      pt.y() = bb.min().y() + j*d.y();
      for( int i = 0 ; i < mc.size_x() ; i++ )
      {
        pt.x() = bb.min().x() + i*d.x();
        points[pointI++] = pt;
      }
    }
  }
  Info<< "Generated " << points.size() << " sampling points in = "
    << timer.cpuTimeIncrement() << " s." << nl << endl;
  // Compute data
  const searchableSurfaces& geometry = geometryToConformTo.geometry();
  const labelList& surfaces = geometryToConformTo.surfaces();
  scalarField signedDist;
  labelList nearestSurfaces;
  searchableSurfacesQueries::signedDistance
  (
    geometry,
    surfaces,
    points,
    scalarField(points.size(), sqr(GREAT)),
    searchableSurface::OUTSIDE,     // for non-closed surfaces treat as
                    // outside
    nearestSurfaces,
    signedDist
  );
  // Fill elements
  pointI = 0;
  for( int k = 0 ; k < mc.size_z() ; k++ )
  {
    for( int j = 0 ; j < mc.size_y() ; j++ )
    {
      for( int i = 0 ; i < mc.size_x() ; i++ )
      {
        mc.set_data(float(signedDist[pointI++]), i, j, k);
      }
    }
  }
  Info<< "Determined signed distance in = "
    << timer.cpuTimeIncrement() << " s." << nl << endl;
  mc.run() ;
  Info<< "Constructed iso surface in = "
    << timer.cpuTimeIncrement() << " s." << nl << endl;
  mc.clean_temps() ;
  // Write output file
  if (mc.ntrigs() > 0)
  {
    Triangle* triangles = mc.triangles();
    List<labelledTri> tris(mc.ntrigs());
    forAll(tris, triI)
    {
      tris[triI] = labelledTri
      (
        triangles[triI].v1,
        triangles[triI].v2,
        triangles[triI].v3,
        0                       // region
      );
    }
    Vertex* vertices = mc.vertices();
    pointField points(mc.nverts());
    forAll(points, pointI)
    {
      Vertex& v = vertices[pointI];
      points[pointI] = point
      (
        bb.min().x() + v.x*span.x()/mc.size_x(),
        bb.min().y() + v.y*span.y()/mc.size_y(),
        bb.min().z() + v.z*span.z()/mc.size_z()
      );
    }
    // Find correspondence to original surfaces
    labelList regionOffsets(surfaces.size());
    label nRegions = 0;
    forAll(surfaces, i)
    {
      const wordList& regions = geometry[surfaces[i]].regions();
      regionOffsets[i] = nRegions;
      nRegions += regions.size();
    }
    geometricSurfacePatchList patches(nRegions);
    nRegions = 0;
    forAll(surfaces, i)
    {
      const wordList& regions = geometry[surfaces[i]].regions();
      forAll(regions, regionI)
      {
        patches[nRegions] = geometricSurfacePatch
        (
          "patch",
          geometry[surfaces[i]].name() + "_" + regions[regionI],
          nRegions
        );
        nRegions++;
      }
    }
    triSurface s(tris, patches, points, true);
    Info<< "Extracted triSurface in = "
      << timer.cpuTimeIncrement() << " s." << nl << endl;
    // Find out region on local surface of nearest point
    {
      List<pointIndexHit> hitInfo;
      labelList hitSurfaces;
      geometryToConformTo.findSurfaceNearest
      (
        s.faceCentres(),
        scalarField(s.size(), sqr(GREAT)),
        hitInfo,
        hitSurfaces
      );
      // Get region
      DynamicList<pointIndexHit> surfInfo(hitSurfaces.size());
      DynamicList<label> surfIndices(hitSurfaces.size());
      forAll(surfaces, surfI)
      {
        // Extract info on this surface
        surfInfo.clear();
        surfIndices.clear();
        forAll(hitSurfaces, triI)
        {
          if (hitSurfaces[triI] == surfI)
          {
            surfInfo.append(hitInfo[triI]);
            surfIndices.append(triI);
          }
        }
        // Calculate sideness of these surface points
        labelList region;
        geometry[surfaces[surfI]].getRegion(surfInfo, region);
        forAll(region, i)
        {
          label triI = surfIndices[i];
          s[triI].region() = regionOffsets[surfI]+region[i];
        }
      }
    }
    Info<< "Re-patched surface in = "
      << timer.cpuTimeIncrement() << " s." << nl << endl;
    triSurfaceMesh smesh
    (
      IOobject
      (
        exportName,
        runTime.constant(), // instance
        "triSurface",
        runTime,            // registry
        IOobject::NO_READ,
        IOobject::AUTO_WRITE,
        false
      ),
      s
    );
    Info<< "writing surfMesh:\n  "
      << smesh.searchableSurface::objectPath() << nl << endl;
    smesh.searchableSurface::write();
    Info<< "Written surface in = "
      << timer.cpuTimeIncrement() << " s." << nl << endl;
  }
  mc.clean_all() ;
  Info<< "End\n" << endl;
  return 0;
}
