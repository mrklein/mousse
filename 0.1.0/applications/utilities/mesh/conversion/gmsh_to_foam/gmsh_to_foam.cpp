// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "poly_mesh.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "ifstream.hpp"
#include "cell_modeller.hpp"
#include "repatch_poly_topo_changer.hpp"
#include "cell_set.hpp"
#include "face_set.hpp"
using namespace mousse;
// Element type numbers
static label MSHTRI   = 2;
static label MSHQUAD  = 3;
static label MSHTET   = 4;
static label MSHPYR   = 7;
static label MSHPRISM = 6;
static label MSHHEX   = 5;
// Skips till end of section. Returns false if end of file.
bool skipSection(IFstream& inFile)
{
  string line;
  do
  {
    inFile.getLine(line);
    if (!inFile.good())
    {
      return false;
    }
  }
  while (line.size() < 4 || line.substr(0, 4) != "$End");
  return true;
}
void renumber
(
  const Map<label>& mshToFoam,
  labelList& labels
)
{
  FOR_ALL(labels, labelI)
  {
    labels[labelI] = mshToFoam[labels[labelI]];
  }
}
// Find face in pp which uses all vertices in meshF (in mesh point labels)
label findFace(const primitivePatch& pp, const labelList& meshF)
{
  const Map<label>& meshPointMap = pp.meshPointMap();
  // meshF[0] in pp labels.
  if (!meshPointMap.found(meshF[0]))
  {
    Warning<< "Not using gmsh face " << meshF
      << " since zero vertex is not on boundary of polyMesh" << endl;
    return -1;
  }
  // Find faces using first point
  const labelList& pFaces = pp.pointFaces()[meshPointMap[meshF[0]]];
  // Go through all these faces and check if there is one which uses all of
  // meshF vertices (in any order ;-)
  FOR_ALL(pFaces, i)
  {
    label faceI = pFaces[i];
    const face& f = pp[faceI];
    // Count uses of vertices of meshF for f
    label nMatched = 0;
    FOR_ALL(f, fp)
    {
      if (findIndex(meshF, f[fp]) != -1)
      {
        nMatched++;
      }
    }
    if (nMatched == meshF.size())
    {
      return faceI;
    }
  }
  return -1;
}
// Same but find internal face. Expensive addressing.
label findInternalFace(const primitiveMesh& mesh, const labelList& meshF)
{
  const labelList& pFaces = mesh.pointFaces()[meshF[0]];
  FOR_ALL(pFaces, i)
  {
    label faceI = pFaces[i];
    const face& f = mesh.faces()[faceI];
    // Count uses of vertices of meshF for f
    label nMatched = 0;
    FOR_ALL(f, fp)
    {
      if (findIndex(meshF, f[fp]) != -1)
      {
        nMatched++;
      }
    }
    if (nMatched == meshF.size())
    {
      return faceI;
    }
  }
  return -1;
}
// Determine whether cell is inside-out by checking for any wrong-oriented
// face.
bool correctOrientation(const pointField& points, const cellShape& shape)
{
  // Get centre of shape.
  point cc(shape.centre(points));
  // Get outwards pointing faces.
  faceList faces(shape.faces());
  FOR_ALL(faces, i)
  {
    const face& f = faces[i];
    vector n(f.normal(points));
    // Check if vector from any point on face to cc points outwards
    if (((points[f[0]] - cc) & n) < 0)
    {
      // Incorrectly oriented
      return false;
    }
  }
  return true;
}
void storeCellInZone
(
  const label regPhys,
  const label cellI,
  Map<label>& physToZone,
  labelList& zoneToPhys,
  List<DynamicList<label> >& zoneCells
)
{
  Map<label>::const_iterator zoneFnd = physToZone.find(regPhys);
  if (zoneFnd == physToZone.end())
  {
    // New region. Allocate zone for it.
    label zoneI = zoneCells.size();
    zoneCells.setSize(zoneI+1);
    zoneToPhys.setSize(zoneI+1);
    Info<< "Mapping region " << regPhys << " to Foam cellZone "
      << zoneI << endl;
    physToZone.insert(regPhys, zoneI);
    zoneToPhys[zoneI] = regPhys;
    zoneCells[zoneI].append(cellI);
  }
  else
  {
    // Existing zone for region
    zoneCells[zoneFnd()].append(cellI);
  }
}
// Reads mesh format
scalar readMeshFormat(IFstream& inFile)
{
  Info<< "Starting to read mesh format at line "
    << inFile.lineNumber()
    << endl;
  string line;
  inFile.getLine(line);
  IStringStream lineStr(line);
  scalar version;
  label asciiFlag, nBytes;
  lineStr >> version >> asciiFlag >> nBytes;
  Info<< "Read format version " << version << "  ascii " << asciiFlag << endl;
  if (asciiFlag != 0)
  {
    FATAL_IO_ERROR_IN("readMeshFormat(IFstream&)", inFile)
      << "Can only read ascii msh files."
      << exit(FatalIOError);
  }
  inFile.getLine(line);
  IStringStream tagStr(line);
  word tag(tagStr);
  if (tag != "$EndMeshFormat")
  {
    FATAL_IO_ERROR_IN("readMeshFormat(IFstream&)", inFile)
      << "Did not find $ENDNOD tag on line "
      << inFile.lineNumber() << exit(FatalIOError);
  }
  Info<< endl;
  return version;
}
// Reads points and map
void readPoints(IFstream& inFile, pointField& points, Map<label>& mshToFoam)
{
  Info<< "Starting to read points at line " << inFile.lineNumber() << endl;
  string line;
  inFile.getLine(line);
  IStringStream lineStr(line);
  label nVerts;
  lineStr >> nVerts;
  Info<< "Vertices to be read:" << nVerts << endl;
  points.setSize(nVerts);
  mshToFoam.resize(2*nVerts);
  for (label pointI = 0; pointI < nVerts; pointI++)
  {
    label mshLabel;
    scalar xVal, yVal, zVal;
    string line;
    inFile.getLine(line);
    IStringStream lineStr(line);
    lineStr >> mshLabel >> xVal >> yVal >> zVal;
    point& pt = points[pointI];
    pt.x() = xVal;
    pt.y() = yVal;
    pt.z() = zVal;
    mshToFoam.insert(mshLabel, pointI);
  }
  Info<< "Vertices read:" << mshToFoam.size() << endl;
  inFile.getLine(line);
  IStringStream tagStr(line);
  word tag(tagStr);
  if (tag != "$ENDNOD" && tag != "$EndNodes")
  {
    FATAL_IO_ERROR_IN("readPoints(..)", inFile)
      << "Did not find $ENDNOD tag on line "
      << inFile.lineNumber() << exit(FatalIOError);
  }
  Info<< endl;
}
// Reads physical names
void readPhysNames(IFstream& inFile, Map<word>& physicalNames)
{
  Info<< "Starting to read physical names at line " << inFile.lineNumber()
    << endl;
  string line;
  inFile.getLine(line);
  IStringStream lineStr(line);
  label nNames;
  lineStr >> nNames;
  Info<< "Physical names:" << nNames << endl;
  physicalNames.resize(nNames);
  for (label i = 0; i < nNames; i++)
  {
    label regionI;
    string regionName;
    string line;
    inFile.getLine(line);
    IStringStream lineStr(line);
    label nSpaces = lineStr.str().count(' ');
    if (nSpaces == 1)
    {
      lineStr >> regionI >> regionName;
      Info<< "    " << regionI << '\t'
        << string::validate<word>(regionName) << endl;
    }
    else if (nSpaces == 2)
    {
      // >= Gmsh2.4 physical types has tag in front.
      label physType;
      lineStr >> physType >> regionI >> regionName;
      if (physType == 1)
      {
        Info<< "    " << "Line " << regionI << '\t'
          << string::validate<word>(regionName) << endl;
      }
      else if (physType == 2)
      {
        Info<< "    " << "Surface " << regionI << '\t'
          << string::validate<word>(regionName) << endl;
      }
      else if (physType == 3)
      {
        Info<< "    " << "Volume " << regionI << '\t'
          << string::validate<word>(regionName) << endl;
      }
    }
    physicalNames.insert(regionI, string::validate<word>(regionName));
  }
  inFile.getLine(line);
  IStringStream tagStr(line);
  word tag(tagStr);
  if (tag != "$EndPhysicalNames")
  {
    FATAL_IO_ERROR_IN("readPhysicalNames(..)", inFile)
      << "Did not find $EndPhysicalNames tag on line "
      << inFile.lineNumber() << exit(FatalIOError);
  }
  Info<< endl;
}
// Reads cells and patch faces
void readCells
(
  const scalar versionFormat,
  const bool keepOrientation,
  const pointField& points,
  const Map<label>& mshToFoam,
  IFstream& inFile,
  cellShapeList& cells,
  labelList& patchToPhys,
  List<DynamicList<face> >& patchFaces,
  labelList& zoneToPhys,
  List<DynamicList<label> >& zoneCells
)
{
  Info<< "Starting to read cells at line " << inFile.lineNumber() << endl;
  const cellModel& hex = *(cellModeller::lookup("hex"));
  const cellModel& prism = *(cellModeller::lookup("prism"));
  const cellModel& pyr = *(cellModeller::lookup("pyr"));
  const cellModel& tet = *(cellModeller::lookup("tet"));
  face triPoints(3);
  face quadPoints(4);
  labelList tetPoints(4);
  labelList pyrPoints(5);
  labelList prismPoints(6);
  labelList hexPoints(8);
  string line;
  inFile.getLine(line);
  IStringStream lineStr(line);
  label nElems;
  lineStr >> nElems;
  Info<< "Cells to be read:" << nElems << endl << endl;
  // Storage for all cells. Too big. Shrink later
  cells.setSize(nElems);
  label cellI = 0;
  label nTet = 0;
  label nPyr = 0;
  label nPrism = 0;
  label nHex = 0;
  // From gmsh physical region to Foam patch
  Map<label> physToPatch;
  // From gmsh physical region to Foam cellZone
  Map<label> physToZone;
  for (label elemI = 0; elemI < nElems; elemI++)
  {
    string line;
    inFile.getLine(line);
    IStringStream lineStr(line);
    label elmNumber, elmType, regPhys;
    if (versionFormat >= 2)
    {
      lineStr >> elmNumber >> elmType;
      label nTags;
      lineStr>> nTags;
      if (nTags > 0)
      {
        // Assume the first tag is the physical surface
        lineStr >> regPhys;
        for (label i = 1; i < nTags; i++)
        {
          label dummy;
          lineStr>> dummy;
        }
      }
    }
    else
    {
      label regElem, nNodes;
      lineStr >> elmNumber >> elmType >> regPhys >> regElem >> nNodes;
    }
    // regPhys on surface elements is region number.
    if (elmType == MSHTRI)
    {
      lineStr >> triPoints[0] >> triPoints[1] >> triPoints[2];
      renumber(mshToFoam, triPoints);
      Map<label>::iterator regFnd = physToPatch.find(regPhys);
      label patchI = -1;
      if (regFnd == physToPatch.end())
      {
        // New region. Allocate patch for it.
        patchI = patchFaces.size();
        patchFaces.setSize(patchI + 1);
        patchToPhys.setSize(patchI + 1);
        Info<< "Mapping region " << regPhys << " to Foam patch "
          << patchI << endl;
        physToPatch.insert(regPhys, patchI);
        patchToPhys[patchI] = regPhys;
      }
      else
      {
        // Existing patch for region
        patchI = regFnd();
      }
      // Add triangle to correct patchFaces.
      patchFaces[patchI].append(triPoints);
    }
    else if (elmType == MSHQUAD)
    {
      lineStr
        >> quadPoints[0] >> quadPoints[1] >> quadPoints[2]
        >> quadPoints[3];
      renumber(mshToFoam, quadPoints);
      Map<label>::iterator regFnd = physToPatch.find(regPhys);
      label patchI = -1;
      if (regFnd == physToPatch.end())
      {
        // New region. Allocate patch for it.
        patchI = patchFaces.size();
        patchFaces.setSize(patchI + 1);
        patchToPhys.setSize(patchI + 1);
        Info<< "Mapping region " << regPhys << " to Foam patch "
          << patchI << endl;
        physToPatch.insert(regPhys, patchI);
        patchToPhys[patchI] = regPhys;
      }
      else
      {
        // Existing patch for region
        patchI = regFnd();
      }
      // Add quad to correct patchFaces.
      patchFaces[patchI].append(quadPoints);
    }
    else if (elmType == MSHTET)
    {
      storeCellInZone
      (
        regPhys,
        cellI,
        physToZone,
        zoneToPhys,
        zoneCells
      );
      lineStr
        >> tetPoints[0] >> tetPoints[1] >> tetPoints[2]
        >> tetPoints[3];
      renumber(mshToFoam, tetPoints);
      cells[cellI++] = cellShape(tet, tetPoints);
      nTet++;
    }
    else if (elmType == MSHPYR)
    {
      storeCellInZone
      (
        regPhys,
        cellI,
        physToZone,
        zoneToPhys,
        zoneCells
      );
      lineStr
        >> pyrPoints[0] >> pyrPoints[1] >> pyrPoints[2]
        >> pyrPoints[3] >> pyrPoints[4];
      renumber(mshToFoam, pyrPoints);
      cells[cellI++] = cellShape(pyr, pyrPoints);
      nPyr++;
    }
    else if (elmType == MSHPRISM)
    {
      storeCellInZone
      (
        regPhys,
        cellI,
        physToZone,
        zoneToPhys,
        zoneCells
      );
      lineStr
        >> prismPoints[0] >> prismPoints[1] >> prismPoints[2]
        >> prismPoints[3] >> prismPoints[4] >> prismPoints[5];
      renumber(mshToFoam, prismPoints);
      cells[cellI] = cellShape(prism, prismPoints);
      const cellShape& cell = cells[cellI];
      if (!keepOrientation && !correctOrientation(points, cell))
      {
        Info<< "Inverting prism " << cellI << endl;
        // Reorder prism.
        prismPoints[0] = cell[0];
        prismPoints[1] = cell[2];
        prismPoints[2] = cell[1];
        prismPoints[3] = cell[3];
        prismPoints[4] = cell[4];
        prismPoints[5] = cell[5];
        cells[cellI] = cellShape(prism, prismPoints);
      }
      cellI++;
      nPrism++;
    }
    else if (elmType == MSHHEX)
    {
      storeCellInZone
      (
        regPhys,
        cellI,
        physToZone,
        zoneToPhys,
        zoneCells
      );
      lineStr
        >> hexPoints[0] >> hexPoints[1]
        >> hexPoints[2] >> hexPoints[3]
        >> hexPoints[4] >> hexPoints[5]
        >> hexPoints[6] >> hexPoints[7];
      renumber(mshToFoam, hexPoints);
      cells[cellI] = cellShape(hex, hexPoints);
      const cellShape& cell = cells[cellI];
      if (!keepOrientation && !correctOrientation(points, cell))
      {
        Info<< "Inverting hex " << cellI << endl;
        // Reorder hex.
        hexPoints[0] = cell[4];
        hexPoints[1] = cell[5];
        hexPoints[2] = cell[6];
        hexPoints[3] = cell[7];
        hexPoints[4] = cell[0];
        hexPoints[5] = cell[1];
        hexPoints[6] = cell[2];
        hexPoints[7] = cell[3];
        cells[cellI] = cellShape(hex, hexPoints);
      }
      cellI++;
      nHex++;
    }
    else
    {
      Info<< "Unhandled element " << elmType << " at line "
        << inFile.lineNumber() << endl;
    }
  }
  inFile.getLine(line);
  IStringStream tagStr(line);
  word tag(tagStr);
  if (tag != "$ENDELM" && tag != "$EndElements")
  {
    FATAL_IO_ERROR_IN("readCells(..)", inFile)
      << "Did not find $ENDELM tag on line "
      << inFile.lineNumber() << exit(FatalIOError);
  }
  cells.setSize(cellI);
  FOR_ALL(patchFaces, patchI)
  {
    patchFaces[patchI].shrink();
  }
  Info<< "Cells:" << endl
  << "    total:" << cells.size() << endl
  << "    hex  :" << nHex << endl
  << "    prism:" << nPrism << endl
  << "    pyr  :" << nPyr << endl
  << "    tet  :" << nTet << endl
  << endl;
  if (cells.size() == 0)
  {
    FATAL_IO_ERROR_IN("readCells(..)", inFile)
      << "No cells read from file " << inFile.name() << nl
      << "Does your file specify any 3D elements (hex=" << MSHHEX
      << ", prism=" << MSHPRISM << ", pyramid=" << MSHPYR
      << ", tet=" << MSHTET << ")?" << nl
      << "Perhaps you have not exported the 3D elements?"
      << exit(FatalIOError);
  }
  Info<< "CellZones:" << nl
    << "Zone\tSize" << endl;
  FOR_ALL(zoneCells, zoneI)
  {
    zoneCells[zoneI].shrink();
    const labelList& zCells = zoneCells[zoneI];
    if (zCells.size())
    {
      Info<< "    " << zoneI << '\t' << zCells.size() << endl;
    }
  }
  Info<< endl;
}
int main(int argc, char *argv[])
{
  argList::noParallel();
  argList::validArgs.append(".msh file");
  argList::addBoolOption
  (
    "keepOrientation",
    "retain raw orientation for prisms/hexs"
  );
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  mousse::word regionName;
  if (args.optionReadIfPresent("region", regionName))
  {
    mousse::Info
      << "Creating polyMesh for region " << regionName << endl;
  }
  else
  {
    regionName = mousse::polyMesh::defaultRegion;
  }
  const bool keepOrientation = args.optionFound("keepOrientation");
  IFstream inFile(args[1]);
  // Storage for points
  pointField points;
  Map<label> mshToFoam;
  // Storage for all cells.
  cellShapeList cells;
  // Map from patch to gmsh physical region
  labelList patchToPhys;
  // Storage for patch faces.
  List<DynamicList<face> > patchFaces(0);
  // Map from cellZone to gmsh physical region
  labelList zoneToPhys;
  // Storage for cell zones.
  List<DynamicList<label> > zoneCells(0);
  // Name per physical region
  Map<word> physicalNames;
  // Version 1 or 2 format
  scalar versionFormat = 1;
  while (inFile.good())
  {
    string line;
    inFile.getLine(line);
    IStringStream lineStr(line);
    word tag(lineStr);
    if (tag == "$MeshFormat")
    {
      versionFormat = readMeshFormat(inFile);
    }
    else if (tag == "$PhysicalNames")
    {
      readPhysNames(inFile, physicalNames);
    }
    else if (tag == "$NOD" || tag == "$Nodes")
    {
      readPoints(inFile, points, mshToFoam);
    }
    else if (tag == "$ELM" || tag == "$Elements")
    {
      readCells
      (
        versionFormat,
        keepOrientation,
        points,
        mshToFoam,
        inFile,
        cells,
        patchToPhys,
        patchFaces,
        zoneToPhys,
        zoneCells
      );
    }
    else
    {
      Info<< "Skipping tag " << tag << " at line "
        << inFile.lineNumber()
        << endl;
      if (!skipSection(inFile))
      {
        break;
      }
    }
  }
  label nValidCellZones = 0;
  FOR_ALL(zoneCells, zoneI)
  {
    if (zoneCells[zoneI].size())
    {
      nValidCellZones++;
    }
  }
  // Problem is that the orientation of the patchFaces does not have to
  // be consistent with the outwards orientation of the mesh faces. So
  // we have to construct the mesh in two stages:
  // 1. define mesh with all boundary faces in one patch
  // 2. use the read patchFaces to find the corresponding boundary face
  //    and repatch it.
  // Create correct number of patches
  // (but without any faces in it)
  faceListList boundaryFaces(patchFaces.size());
  wordList boundaryPatchNames(boundaryFaces.size());
  FOR_ALL(boundaryPatchNames, patchI)
  {
    label physReg = patchToPhys[patchI];
    Map<word>::const_iterator iter = physicalNames.find(physReg);
    if (iter != physicalNames.end())
    {
      boundaryPatchNames[patchI] = iter();
    }
    else
    {
      boundaryPatchNames[patchI] = word("patch") + name(patchI);
    }
    Info<< "Patch " << patchI << " gets name "
      << boundaryPatchNames[patchI] << endl;
  }
  Info<< endl;
  wordList boundaryPatchTypes(boundaryFaces.size(), polyPatch::typeName);
  word defaultFacesName = "defaultFaces";
  word defaultFacesType = polyPatch::typeName;
  wordList boundaryPatchPhysicalTypes
  (
    boundaryFaces.size(),
    polyPatch::typeName
  );
  polyMesh mesh
  (
    IOobject
    (
      regionName,
      runTime.constant(),
      runTime
    ),
    xferMove(points),
    cells,
    boundaryFaces,
    boundaryPatchNames,
    boundaryPatchTypes,
    defaultFacesName,
    defaultFacesType,
    boundaryPatchPhysicalTypes
  );
  repatchPolyTopoChanger repatcher(mesh);
  // Now use the patchFaces to patch up the outside faces of the mesh.
  // Get the patch for all the outside faces (= default patch added as last)
  const polyPatch& pp = mesh.boundaryMesh().last();
  // Storage for faceZones.
  List<DynamicList<label> > zoneFaces(patchFaces.size());
  // Go through all the patchFaces and find corresponding face in pp.
  FOR_ALL(patchFaces, patchI)
  {
    const DynamicList<face>& pFaces = patchFaces[patchI];
    Info<< "Finding faces of patch " << patchI << endl;
    FOR_ALL(pFaces, i)
    {
      const face& f = pFaces[i];
      // Find face in pp using all vertices of f.
      label patchFaceI = findFace(pp, f);
      if (patchFaceI != -1)
      {
        label meshFaceI = pp.start() + patchFaceI;
        repatcher.changePatchID(meshFaceI, patchI);
      }
      else
      {
        // Maybe internal face? If so add to faceZone with same index
        // - might be useful.
        label meshFaceI = findInternalFace(mesh, f);
        if (meshFaceI != -1)
        {
          zoneFaces[patchI].append(meshFaceI);
        }
        else
        {
          WARNING_IN(args.executable())
            << "Could not match gmsh face " << f
            << " to any of the interior or exterior faces"
            << " that share the same 0th point" << endl;
        }
      }
    }
  }
  Info<< nl;
  // Face zones
  label nValidFaceZones = 0;
  Info<< "FaceZones:" << nl
    << "Zone\tSize" << endl;
  FOR_ALL(zoneFaces, zoneI)
  {
    zoneFaces[zoneI].shrink();
    const labelList& zFaces = zoneFaces[zoneI];
    if (zFaces.size())
    {
      nValidFaceZones++;
      Info<< "    " << zoneI << '\t' << zFaces.size() << endl;
    }
  }
  Info<< endl;
  //Get polyMesh to write to constant
  runTime.setTime(instant(runTime.constant()), 0);
  repatcher.repatch();
  List<cellZone*> cz;
  List<faceZone*> fz;
  // Construct and add the zones. Note that cell ordering does not change
  // because of repatch() and neither does internal faces so we can
  // use the zoneCells/zoneFaces as is.
  if (nValidCellZones > 0)
  {
    cz.setSize(nValidCellZones);
    nValidCellZones = 0;
    FOR_ALL(zoneCells, zoneI)
    {
      if (zoneCells[zoneI].size())
      {
        label physReg = zoneToPhys[zoneI];
        Map<word>::const_iterator iter = physicalNames.find(physReg);
        word zoneName = "cellZone_" + name(zoneI);
        if (iter != physicalNames.end())
        {
          zoneName = iter();
        }
        Info<< "Writing zone " << zoneI << " to cellZone "
          << zoneName << " and cellSet"
          << endl;
        cellSet cset(mesh, zoneName, zoneCells[zoneI]);
        cset.write();
        cz[nValidCellZones] = new cellZone
        (
          zoneName,
          zoneCells[zoneI],
          nValidCellZones,
          mesh.cellZones()
        );
        nValidCellZones++;
      }
    }
  }
  if (nValidFaceZones > 0)
  {
    fz.setSize(nValidFaceZones);
    nValidFaceZones = 0;
    FOR_ALL(zoneFaces, zoneI)
    {
      if (zoneFaces[zoneI].size())
      {
        label physReg = patchToPhys[zoneI];
        Map<word>::const_iterator iter = physicalNames.find(physReg);
        word zoneName = "faceZone_" + name(zoneI);
        if (iter != physicalNames.end())
        {
          zoneName = iter();
        }
        Info<< "Writing zone " << zoneI << " to faceZone "
          << zoneName << " and faceSet"
          << endl;
        faceSet fset(mesh, zoneName, zoneFaces[zoneI]);
        fset.write();
        fz[nValidFaceZones] = new faceZone
        (
          zoneName,
          zoneFaces[zoneI],
          boolList(zoneFaces[zoneI].size(), true),
          nValidFaceZones,
          mesh.faceZones()
        );
        nValidFaceZones++;
      }
    }
  }
  if (cz.size() || fz.size())
  {
    mesh.addZones(List<pointZone*>(0), fz, cz);
  }
  // Remove empty defaultFaces
  label defaultPatchID = mesh.boundaryMesh().findPatchID(defaultFacesName);
  if (mesh.boundaryMesh()[defaultPatchID].size() == 0)
  {
    List<polyPatch*> newPatchPtrList((mesh.boundaryMesh().size() - 1));
    label newPatchI = 0;
    FOR_ALL(mesh.boundaryMesh(), patchI)
    {
      if (patchI != defaultPatchID)
      {
        const polyPatch& patch = mesh.boundaryMesh()[patchI];
        newPatchPtrList[newPatchI] = patch.clone
        (
          mesh.boundaryMesh(),
          newPatchI,
          patch.size(),
          patch.start()
        ).ptr();
        newPatchI++;
      }
    }
    repatcher.changePatches(newPatchPtrList);
  }
  mesh.write();
  Info<< "End\n" << endl;
  return 0;
}
