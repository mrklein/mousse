// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Application
//   mousse-stream-function
// Description
//   Calculates and writes the stream function of velocity field U at each
//   time.
#include "fv_cfd.hpp"
#include "point_fields.hpp"
#include "empty_poly_patch.hpp"
#include "symmetry_plane_poly_patch.hpp"
#include "symmetry_poly_patch.hpp"
#include "wedge_poly_patch.hpp"
#include "os_specific.hpp"
int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  label nD = mesh.nGeometricD();
  if (nD != 2)
  {
    FATAL_ERROR_IN(args.executable())
      << "Case is not 2D, stream-function cannot be computed"
      << exit(FatalError);
  }
  Vector<label> slabNormal{(Vector<label>::one - mesh.geometricD())/2};
  const direction slabDir
  (
    slabNormal
   & Vector<label>(Vector<label>::X, Vector<label>::Y, Vector<label>::Z)
  );
  scalar thickness = vector(slabNormal) & mesh.bounds().span();
  const pointMesh& pMesh = pointMesh::New(mesh);
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info<< nl << "Time: " << runTime.timeName() << endl;
    IOobject phiHeader
    {
      "phi",
      runTime.timeName(),
      mesh,
      IOobject::NO_READ
    };
    if (phiHeader.headerOk())
    {
      mesh.readUpdate();
      Info<< nl << "Reading field phi" << endl;
      surfaceScalarField phi
      {
        // IOobject
        {
          "phi",
          runTime.timeName(),
          mesh,
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        },
        mesh
      };
      pointScalarField streamFunction
      {
        // IOobject
        {
          "streamFunction",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        pMesh,
        {"zero", phi.dimensions(), 0.0}
      };
      labelList visitedPoint{mesh.nPoints()};
      FOR_ALL(visitedPoint, pointI)
      {
        visitedPoint[pointI] = 0;
      }
      label nVisited = 0;
      label nVisitedOld = 0;
      const faceUList& faces = mesh.faces();
      const pointField& points = mesh.points();
      label nInternalFaces = mesh.nInternalFaces();
      vectorField unitAreas{mesh.faceAreas()};
      unitAreas /= mag(unitAreas);
      const polyPatchList& patches = mesh.boundaryMesh();
      bool finished = true;
      // Find the boundary face with zero flux. set the stream function
      // to zero on that face
      bool found = false;
      do
      {
        found = false;
        FOR_ALL(patches, patchI)
        {
          const primitivePatch& bouFaces = patches[patchI];
          if (!isType<emptyPolyPatch>(patches[patchI]))
          {
            FOR_ALL(bouFaces, faceI)
            {
              if (magSqr(phi.boundaryField()[patchI][faceI]) < SMALL)
              {
                const labelList& zeroPoints = bouFaces[faceI];
                // Zero flux face found
                found = true;
                FOR_ALL(zeroPoints, pointI)
                {
                  if (visitedPoint[zeroPoints[pointI]] == 1)
                  {
                    found = false;
                    break;
                  }
                }
                if (found)
                {
                  Info<< "Zero face: patch: " << patchI
                    << "    face: " << faceI << endl;
                  FOR_ALL(zeroPoints, pointI)
                  {
                    streamFunction[zeroPoints[pointI]] = 0;
                    visitedPoint[zeroPoints[pointI]] = 1;
                    nVisited++;
                  }
                  break;
                }
              }
            }
          }
          if (found) break;
        }
        if (!found)
        {
          Info<< "zero flux boundary face not found. "
            << "Using cell as a reference."
            << endl;
          const cellList& c = mesh.cells();
          FOR_ALL(c, cI)
          {
            labelList zeroPoints = c[cI].labels(mesh.faces());
            bool found = true;
            FOR_ALL(zeroPoints, pointI)
            {
              if (visitedPoint[zeroPoints[pointI]] == 1)
              {
                found = false;
                break;
              }
            }
            if (found)
            {
              FOR_ALL(zeroPoints, pointI)
              {
                streamFunction[zeroPoints[pointI]] = 0.0;
                visitedPoint[zeroPoints[pointI]] = 1;
                nVisited++;
              }
              break;
            }
            else
            {
              FATAL_ERROR_IN(args.executable())
                << "Cannot find initialisation face or a cell."
                << abort(FatalError);
            }
          }
        }
        // Loop through all faces. If one of the points on
        // the face has the streamfunction value different
        // from -1, all points with -1 ont that face have the
        // streamfunction value equal to the face flux in
        // that point plus the value in the visited point
        do
        {
          finished = true;
          for
          (
            label faceI = nInternalFaces;
            faceI<faces.size();
            faceI++
          )
          {
            const labelList& curBPoints = faces[faceI];
            bool bPointFound = false;
            scalar currentBStream = 0.0;
            vector currentBStreamPoint(0, 0, 0);
            FOR_ALL(curBPoints, pointI)
            {
              // Check if the point has been visited
              if (visitedPoint[curBPoints[pointI]] == 1)
              {
                // The point has been visited
                currentBStream =
                  streamFunction[curBPoints[pointI]];
                currentBStreamPoint =
                  points[curBPoints[pointI]];
                bPointFound = true;
                break;
              }
            }
            if (bPointFound)
            {
              // Sort out other points on the face
              FOR_ALL(curBPoints, pointI)
              {
                // Check if the point has been visited
                if (visitedPoint[curBPoints[pointI]] == 0)
                {
                  label patchNo =
                    mesh.boundaryMesh().whichPatch(faceI);
                  if
                  (
                    !isType<emptyPolyPatch>
                    (patches[patchNo])
                  && !isType<symmetryPlanePolyPatch>
                    (patches[patchNo])
                  && !isType<symmetryPolyPatch>
                    (patches[patchNo])
                  && !isType<wedgePolyPatch>
                    (patches[patchNo])
                  )
                  {
                    label faceNo =
                      mesh.boundaryMesh()[patchNo]
                      .whichFace(faceI);
                    vector edgeHat =
                      points[curBPoints[pointI]]
                      - currentBStreamPoint;
                    edgeHat.replace(slabDir, 0);
                    edgeHat /= mag(edgeHat);
                    vector nHat = unitAreas[faceI];
                    if (edgeHat.y() > VSMALL)
                    {
                      visitedPoint[curBPoints[pointI]] =
                        1;
                      nVisited++;
                      streamFunction[curBPoints[pointI]]
                        =
                        currentBStream
                       + phi.boundaryField()
                        [patchNo][faceNo]
                        *sign(nHat.x());
                    }
                    else if (edgeHat.y() < -VSMALL)
                    {
                      visitedPoint[curBPoints[pointI]] =
                        1;
                      nVisited++;
                      streamFunction[curBPoints[pointI]]
                        =
                        currentBStream
                       - phi.boundaryField()
                        [patchNo][faceNo]
                        *sign(nHat.x());
                    }
                    else
                    {
                      if (edgeHat.x() > VSMALL)
                      {
                        visitedPoint
                          [curBPoints[pointI]] = 1;
                        nVisited++;
                        streamFunction
                          [curBPoints[pointI]] =
                          currentBStream
                         + phi.boundaryField()
                          [patchNo][faceNo]
                          *sign(nHat.y());
                      }
                      else if (edgeHat.x() < -VSMALL)
                      {
                        visitedPoint
                          [curBPoints[pointI]] = 1;
                        nVisited++;
                        streamFunction
                          [curBPoints[pointI]] =
                          currentBStream
                         - phi.boundaryField()
                          [patchNo][faceNo]
                          *sign(nHat.y());
                      }
                    }
                  }
                }
              }
            }
            else
            {
              finished = false;
            }
          }
          for (label faceI=0; faceI<nInternalFaces; faceI++)
          {
            // Get the list of point labels for the face
            const labelList& curPoints = faces[faceI];
            bool pointFound = false;
            scalar currentStream = 0.0;
            point currentStreamPoint(0, 0, 0);
            FOR_ALL(curPoints, pointI)
            {
              // Check if the point has been visited
              if (visitedPoint[curPoints[pointI]] == 1)
              {
                // The point has been visited
                currentStream =
                  streamFunction[curPoints[pointI]];
                currentStreamPoint =
                  points[curPoints[pointI]];
                pointFound = true;
                break;
              }
            }
            if (pointFound)
            {
              // Sort out other points on the face
              FOR_ALL(curPoints, pointI)
              {
                // Check if the point has been visited
                if (visitedPoint[curPoints[pointI]] == 0)
                {
                  vector edgeHat =
                    points[curPoints[pointI]]
                   - currentStreamPoint;
                  edgeHat.replace(slabDir, 0);
                  edgeHat /= mag(edgeHat);
                  vector nHat = unitAreas[faceI];
                  if (edgeHat.y() > VSMALL)
                  {
                    visitedPoint[curPoints[pointI]] = 1;
                    nVisited++;
                    streamFunction[curPoints[pointI]] =
                      currentStream
                     + phi[faceI]*sign(nHat.x());
                  }
                  else if (edgeHat.y() < -VSMALL)
                  {
                    visitedPoint[curPoints[pointI]] = 1;
                    nVisited++;
                    streamFunction[curPoints[pointI]] =
                      currentStream
                     - phi[faceI]*sign(nHat.x());
                  }
                }
              }
            }
            else
            {
              finished = false;
            }
          }
          Info<< ".";
          if (nVisited == nVisitedOld)
          {
            // Find new seed.  This must be a
            // multiply connected domain
            Info<< nl << "Exhausted a seed. Looking for new seed "
              << "(this is correct for multiply connected "
              << "domains).";
            break;
          }
          else
          {
            nVisitedOld = nVisited;
          }
        } while (!finished);
        Info<< endl;
      } while (!finished);
      // Normalise the stream-function by the 2D mesh thickness
      streamFunction /= thickness;
      streamFunction.boundaryField() = 0.0;
      streamFunction.write();
    }
    else
    {
      WARNING_IN(args.executable())
        << "Flux field does not exist."
        << " Stream function not calculated" << endl;
    }
  }
  Info<< "\nEnd\n" << endl;
  return 0;
}
