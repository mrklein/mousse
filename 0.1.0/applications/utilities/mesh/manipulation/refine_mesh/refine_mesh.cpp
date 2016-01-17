// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "poly_mesh.hpp"
#include "time.hpp"
#include "undoable_mesh_cutter.hpp"
#include "hex_cell_looper.hpp"
#include "cell_set.hpp"
#include "twod_point_corrector.hpp"
#include "directions.hpp"
#include "ofstream.hpp"
#include "multi_dir_refinement.hpp"
#include "label_io_list.hpp"
#include "wedge_poly_patch.hpp"
#include "plane.hpp"
#include "sub_field.hpp"
using namespace mousse;
// Max cos angle for edges to be considered aligned with axis.
static const scalar edgeTol = 1e-3;
// Print edge statistics on mesh.
void printEdgeStats(const primitiveMesh& mesh)
{
  label nX = 0;
  label nY = 0;
  label nZ = 0;
  scalar minX = GREAT;
  scalar maxX = -GREAT;
  vector x(1, 0, 0);
  scalar minY = GREAT;
  scalar maxY = -GREAT;
  vector y(0, 1, 0);
  scalar minZ = GREAT;
  scalar maxZ = -GREAT;
  vector z(0, 0, 1);
  scalar minOther = GREAT;
  scalar maxOther = -GREAT;
  const edgeList& edges = mesh.edges();
  FOR_ALL(edges, edgeI)
  {
    const edge& e = edges[edgeI];
    vector eVec(e.vec(mesh.points()));
    scalar eMag = mag(eVec);
    eVec /= eMag;
    if (mag(eVec & x) > 1-edgeTol)
    {
      minX = min(minX, eMag);
      maxX = max(maxX, eMag);
      nX++;
    }
    else if (mag(eVec & y) > 1-edgeTol)
    {
      minY = min(minY, eMag);
      maxY = max(maxY, eMag);
      nY++;
    }
    else if (mag(eVec & z) > 1-edgeTol)
    {
      minZ = min(minZ, eMag);
      maxZ = max(maxZ, eMag);
      nZ++;
    }
    else
    {
      minOther = min(minOther, eMag);
      maxOther = max(maxOther, eMag);
    }
  }
  Pout<< "Mesh edge statistics:" << endl
    << "    x aligned :  number:" << nX << "\tminLen:" << minX
    << "\tmaxLen:" << maxX << endl
    << "    y aligned :  number:" << nY << "\tminLen:" << minY
    << "\tmaxLen:" << maxY << endl
    << "    z aligned :  number:" << nZ << "\tminLen:" << minZ
    << "\tmaxLen:" << maxZ << endl
    << "    other     :  number:" << mesh.nEdges() - nX - nY - nZ
    << "\tminLen:" << minOther
    << "\tmaxLen:" << maxOther << endl << endl;
}
int main(int argc, char *argv[])
{
  argList::addNote
  (
    "refine cells in multiple directions"
  );
  #include "add_overwrite_option.inc"
  #include "add_region_option.inc"
  #include "add_dict_option.inc"
  mousse::argList::addBoolOption
  (
    "all",
    "Refine all cells"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  #include "create_named_poly_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  printEdgeStats(mesh);
  //
  // Read/construct control dictionary
  //
  const bool readDict = args.optionFound("dict");
  const bool refineAllCells = args.optionFound("all");
  const bool overwrite = args.optionFound("overwrite");
  // List of cells to refine
  labelList refCells;
  // Dictionary to control refinement
  dictionary refineDict;
  const word dictName("refineMeshDict");
  if (readDict)
  {
    fileName dictPath = args["dict"];
    if (isDir(dictPath))
    {
      dictPath = dictPath/dictName;
    }
    IOobject dictIO
    (
      dictPath,
      mesh,
      IOobject::MUST_READ
    );
    if (!dictIO.headerOk())
    {
      FATAL_ERROR_IN(args.executable())
        << "Cannot open specified refinement dictionary "
        << dictPath
        << exit(FatalError);
    }
    Info<< "Refining according to " << dictPath << nl << endl;
    refineDict = IOdictionary(dictIO);
  }
  else if (!refineAllCells)
  {
    IOobject dictIO
    {
      dictName,
      runTime.system(),
      mesh,
      IOobject::MUST_READ
    };
    if (dictIO.headerOk())
    {
      Info<< "Refining according to " << dictName << nl << endl;
      refineDict = IOdictionary{dictIO};
    }
    else
    {
      Info<< "Refinement dictionary " << dictName << " not found" << endl;
    }
  }
  if (refineDict.size())
  {
    const word setName{refineDict.lookup("set")};
    cellSet cells(mesh, setName);
    Pout<< "Read " << cells.size() << " cells from cellSet "
      << cells.instance()/cells.local()/cells.name()
      << endl << endl;
    refCells = cells.toc();
  }
  else
  {
    Info<< "Refining all cells" << nl << endl;
    // Select all cells
    refCells.setSize(mesh.nCells());
    FOR_ALL(mesh.cells(), cellI)
    {
      refCells[cellI] = cellI;
    }
    if (mesh.nGeometricD() == 3)
    {
      Info<< "3D case; refining all directions" << nl << endl;
      wordList directions(3);
      directions[0] = "tan1";
      directions[1] = "tan2";
      directions[2] = "normal";
      refineDict.add("directions", directions);
      // Use hex cutter
      refineDict.add("useHexTopology", "true");
    }
    else
    {
      const Vector<label> dirs{mesh.geometricD()};
      wordList directions{2};
      if (dirs.x() == -1)
      {
        Info<< "2D case; refining in directions y,z\n" << endl;
        directions[0] = "tan2";
        directions[1] = "normal";
      }
      else if (dirs.y() == -1)
      {
        Info<< "2D case; refining in directions x,z\n" << endl;
        directions[0] = "tan1";
        directions[1] = "normal";
      }
      else
      {
        Info<< "2D case; refining in directions x,y\n" << endl;
        directions[0] = "tan1";
        directions[1] = "tan2";
      }
      refineDict.add("directions", directions);
      // Use standard cutter
      refineDict.add("useHexTopology", "false");
    }
    refineDict.add("coordinateSystem", "global");
    dictionary coeffsDict;
    coeffsDict.add("tan1", vector(1, 0, 0));
    coeffsDict.add("tan2", vector(0, 1, 0));
    refineDict.add("globalCoeffs", coeffsDict);
    refineDict.add("geometricCut", "false");
    refineDict.add("writeMesh", "false");
  }
  string oldTimeName(runTime.timeName());
  if (!overwrite)
  {
    runTime++;
  }
  // Multi-directional refinement (does multiple iterations)
  multiDirRefinement multiRef(mesh, refCells, refineDict);
  // Write resulting mesh
  if (overwrite)
  {
    mesh.setInstance(oldInstance);
  }
  mesh.write();
  // Get list of cell splits.
  // (is for every cell in old mesh the cells they have been split into)
  const labelListList& oldToNew = multiRef.addedCells();
  // Create cellSet with added cells for easy inspection
  cellSet newCells{mesh, "refinedCells", refCells.size()};
  FOR_ALL(oldToNew, oldCellI)
  {
    const labelList& added = oldToNew[oldCellI];
    FOR_ALL(added, i)
    {
      newCells.insert(added[i]);
    }
  }
  Pout<< "Writing refined cells (" << newCells.size() << ") to cellSet "
    << newCells.instance()/newCells.local()/newCells.name()
    << endl << endl;
  newCells.write();
  //
  // Invert cell split to construct map from new to old
  //
  labelIOList newToOld
  {
    // IOobject
    {
      "cellMap",
      runTime.timeName(),
      polyMesh::meshSubDir,
      mesh,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    mesh.nCells()
  };
  newToOld.note() =
    "From cells in mesh at "
   + runTime.timeName()
   + " to cells in mesh at "
   + oldTimeName;
  FOR_ALL(oldToNew, oldCellI)
  {
    const labelList& added = oldToNew[oldCellI];
    if (added.size())
    {
      FOR_ALL(added, i)
      {
        newToOld[added[i]] = oldCellI;
      }
    }
    else
    {
      // Unrefined cell
      newToOld[oldCellI] = oldCellI;
    }
  }
  Info<< "Writing map from new to old cell to "
    << newToOld.objectPath() << nl << endl;
  newToOld.write();
  printEdgeStats(mesh);
  Info<< "End\n" << endl;
  return 0;
}
