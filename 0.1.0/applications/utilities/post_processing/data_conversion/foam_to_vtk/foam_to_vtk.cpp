// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "point_mesh.hpp"
#include "vol_point_interpolation.hpp"
#include "empty_poly_patch.hpp"
#include "label_io_field.hpp"
#include "scalar_io_field.hpp"
#include "spherical_tensor_io_field.hpp"
#include "symm_tensor_io_field.hpp"
#include "tensor_io_field.hpp"
#include "face_zone_mesh.hpp"
#include "cloud.hpp"
#include "passive_particle.hpp"
#include "string_list_ops.hpp"
#include "vtk_mesh.hpp"
#include "read_fields.hpp"
#include "write_funs.hpp"
#include "internal_writer.hpp"
#include "patch_writer.hpp"
#include "lagrangian_writer.hpp"
#include "write_face_set.hpp"
#include "write_point_set.hpp"
#include "surface_mesh_writer.hpp"
#include "write_surf_fields.hpp"
template<class GeoField>
void print(const char* msg, Ostream& os, const PtrList<GeoField>& flds)
{
  if (flds.size())
  {
    os  << msg;
    FOR_ALL(flds, i)
    {
      os  << ' ' << flds[i].name();
    }
    os  << endl;
  }
}
void print(Ostream& os, const wordList& flds)
{
  FOR_ALL(flds, i)
  {
    os  << ' ' << flds[i];
  }
  os  << endl;
}
labelList getSelectedPatches
(
  const polyBoundaryMesh& patches,
  const List<wordRe>& excludePatches  //HashSet<word>& excludePatches
)
{
  DynamicList<label> patchIDs(patches.size());
  Info<< "Combining patches:" << endl;
  FOR_ALL(patches, patchI)
  {
    const polyPatch& pp = patches[patchI];
    if (isType<emptyPolyPatch>(pp)
        || (Pstream::parRun() && isType<processorPolyPatch>(pp)))
    {
      Info<< "    discarding empty/processor patch " << patchI
        << " " << pp.name() << endl;
    }
    else if (findStrings(excludePatches, pp.name()))
    {
      Info<< "    excluding patch " << patchI
        << " " << pp.name() << endl;
    }
    else
    {
      patchIDs.append(patchI);
      Info<< "    patch " << patchI << " " << pp.name() << endl;
    }
  }
  return patchIDs.shrink();
}
int main(int argc, char *argv[])
{
  argList::addNote
  (
    "legacy VTK file format writer"
  );
  timeSelector::addOptions();
  #include "add_region_option.inc"
  argList::addOption
  (
    "fields",
    "wordList",
    "only convert the specified fields - eg '(p T U)'"
  );
  argList::addOption
  (
    "cellSet",
    "name",
    "convert a mesh subset corresponding to the specified cellSet"
  );
  argList::addOption
  (
    "faceSet",
    "name",
    "restrict conversion to the specified faceSet"
  );
  argList::addOption
  (
    "pointSet",
    "name",
    "restrict conversion to the specified pointSet"
  );
  argList::addBoolOption
  (
    "ascii",
    "write in ASCII format instead of binary"
  );
  argList::addBoolOption
  (
    "poly",
    "write polyhedral cells without tet/pyramid decomposition"
  );
  argList::addBoolOption
  (
    "surfaceFields",
    "write surfaceScalarFields (e.g., phi)"
  );
  argList::addBoolOption
  (
    "nearCellValue",
    "use cell value on patches instead of patch value itself"
  );
  argList::addBoolOption
  (
    "noInternal",
    "do not generate file for mesh, only for patches"
  );
  argList::addBoolOption
  (
    "noPointValues",
    "no pointFields"
  );
  argList::addBoolOption
  (
    "allPatches",
    "combine all patches into a single file"
  );
  argList::addOption
  (
    "excludePatches",
    "wordReList",
    "a list of patches to exclude - eg '( inlet \".*Wall\" )' "
  );
  argList::addBoolOption
  (
    "noFaceZones",
    "no faceZones"
  );
  argList::addBoolOption
  (
    "noLinks",
    "don't link processor VTK files - parallel only"
  );
  argList::addBoolOption
  (
    "useTimeName",
    "use the time name instead of the time index when naming the files"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  const bool doWriteInternal = !args.optionFound("noInternal");
  const bool doFaceZones     = !args.optionFound("noFaceZones");
  const bool doLinks         = !args.optionFound("noLinks");
  const bool binary          = !args.optionFound("ascii");
  const bool useTimeName     = args.optionFound("useTimeName");
  // decomposition of polyhedral cells into tets/pyramids cells
  vtkTopo::decomposePoly     = !args.optionFound("poly");
  if (binary && (sizeof(floatScalar) != 4 || sizeof(label) != 4))
  {
    FATAL_ERROR_IN(args.executable())
      << "floatScalar and/or label are not 4 bytes in size" << nl
      << "Hence cannot use binary VTK format. Please use -ascii"
      << exit(FatalError);
  }
  const bool nearCellValue = args.optionFound("nearCellValue");
  if (nearCellValue)
  {
    WARNING_IN(args.executable())
      << "Using neighbouring cell value instead of patch value"
      << nl << endl;
  }
  const bool noPointValues = args.optionFound("noPointValues");
  if (noPointValues)
  {
    WARNING_IN(args.executable())
      << "Outputting cell values only" << nl << endl;
  }
  const bool allPatches = args.optionFound("allPatches");
  List<wordRe> excludePatches;
  if (args.optionFound("excludePatches"))
  {
    args.optionLookup("excludePatches")() >> excludePatches;
    Info<< "Not including patches " << excludePatches << nl << endl;
  }
  word cellSetName;
  word faceSetName;
  word pointSetName;
  string vtkName = runTime.caseName();
  if (args.optionReadIfPresent("cellSet", cellSetName))
  {
    vtkName = cellSetName;
  }
  else if (Pstream::parRun())
  {
    // Strip off leading casename, leaving just processor_DDD ending.
    string::size_type i = vtkName.rfind("processor");
    if (i != string::npos)
    {
      vtkName = vtkName.substr(i);
    }
  }
  args.optionReadIfPresent("faceSet", faceSetName);
  args.optionReadIfPresent("pointSet", pointSetName);
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  // VTK/ directory in the case
  fileName fvPath(runTime.path()/"VTK");
  // Directory of mesh (region0 gets filtered out)
  fileName regionPrefix = "";
  if (regionName != polyMesh::defaultRegion)
  {
    fvPath = fvPath/regionName;
    regionPrefix = regionName;
  }
  if (isDir(fvPath))
  {
    if
    (
      args.optionFound("time")
    || args.optionFound("latestTime")
    || cellSetName.size()
    || faceSetName.size()
    || pointSetName.size()
    || regionName != polyMesh::defaultRegion
    )
    {
      Info<< "Keeping old VTK files in " << fvPath << nl << endl;
    }
    else
    {
      Info<< "Deleting old VTK files in " << fvPath << nl << endl;
      rmDir(fvPath);
    }
  }
  mkDir(fvPath);
  // mesh wrapper; does subsetting and decomposition
  vtkMesh vMesh(mesh, cellSetName);
  // Scan for all possible lagrangian clouds
  HashSet<fileName> allCloudDirs;
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    fileNameList cloudDirs
    (
      readDir
      (
        runTime.timePath()/regionPrefix/cloud::prefix,
        fileName::DIRECTORY
      )
    );
    FOR_ALL(cloudDirs, i)
    {
      IOobjectList sprayObjs
      {
        mesh,
        runTime.timeName(),
        cloud::prefix/cloudDirs[i]
      };
      IOobject* positionsPtr = sprayObjs.lookup(word("positions"));
      if (positionsPtr)
      {
        if (allCloudDirs.insert(cloudDirs[i]))
        {
          Info<< "At time: " << runTime.timeName()
            << " detected cloud directory : " << cloudDirs[i]
            << endl;
        }
      }
    }
  }
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info<< "Time: " << runTime.timeName() << endl;
    word timeDesc =
      useTimeName ? runTime.timeName() : mousse::name(runTime.timeIndex());
    // Check for new polyMesh/ and update mesh, fvMeshSubset and cell
    // decomposition.
    polyMesh::readUpdateState meshState = vMesh.readUpdate();
    const fvMesh& mesh = vMesh.mesh();
    if
    (
      meshState == polyMesh::TOPO_CHANGE
    || meshState == polyMesh::TOPO_PATCH_CHANGE
    )
    {
      Info<< "    Read new mesh" << nl << endl;
    }
    // If faceSet: write faceSet only (as polydata)
    if (faceSetName.size())
    {
      // Load the faceSet
      faceSet set{mesh, faceSetName};
      // Filename as if patch with same name.
      mkDir(fvPath/set.name());
      fileName patchFileName(fvPath/set.name()/set.name() + "_" + timeDesc
                             + ".vtk");
      Info<< "    FaceSet   : " << patchFileName << endl;
      writeFaceSet(binary, vMesh, set, patchFileName);
      continue;
    }
    // If pointSet: write pointSet only (as polydata)
    if (pointSetName.size())
    {
      // Load the pointSet
      pointSet set(mesh, pointSetName);
      // Filename as if patch with same name.
      mkDir(fvPath/set.name());
      fileName patchFileName
      (
        fvPath/set.name()/set.name()
       + "_"
       + timeDesc
       + ".vtk"
      );
      Info<< "    pointSet   : " << patchFileName << endl;
      writePointSet(binary, vMesh, set, patchFileName);
      continue;
    }
    // Search for list of objects for this time
    IOobjectList objects{mesh, runTime.timeName()};
    HashSet<word> selectedFields;
    bool specifiedFields = args.optionReadIfPresent
    (
      "fields",
      selectedFields
    );
    // Construct the vol fields (on the original mesh if subsetted)
    PtrList<volScalarField> vsf;
    PtrList<volVectorField> vvf;
    PtrList<volSphericalTensorField> vSpheretf;
    PtrList<volSymmTensorField> vSymmtf;
    PtrList<volTensorField> vtf;
    if (!specifiedFields || selectedFields.size())
    {
      readFields(vMesh, vMesh.baseMesh(), objects, selectedFields, vsf);
      print("    volScalarFields            :", Info, vsf);
      readFields(vMesh, vMesh.baseMesh(), objects, selectedFields, vvf);
      print("    volVectorFields            :", Info, vvf);
      readFields
      (
        vMesh,
        vMesh.baseMesh(),
        objects,
        selectedFields,
        vSpheretf
      );
      print("    volSphericalTensorFields   :", Info, vSpheretf);
      readFields
      (
        vMesh,
        vMesh.baseMesh(),
        objects,
        selectedFields,
        vSymmtf
      );
      print("    volSymmTensorFields        :", Info, vSymmtf);
      readFields(vMesh, vMesh.baseMesh(), objects, selectedFields, vtf);
      print("    volTensorFields            :", Info, vtf);
    }
    label nVolFields =
        vsf.size()
       + vvf.size()
       + vSpheretf.size()
       + vSymmtf.size()
       + vtf.size();
    // Construct pointMesh only if nessecary since constructs edge
    // addressing (expensive on polyhedral meshes)
    if (noPointValues)
    {
      Info<< "    pointScalarFields : switched off"
        << " (\"-noPointValues\" (at your option)\n";
      Info<< "    pointVectorFields : switched off"
        << " (\"-noPointValues\" (at your option)\n";
    }
    PtrList<pointScalarField> psf;
    PtrList<pointVectorField> pvf;
    PtrList<pointSphericalTensorField> pSpheretf;
    PtrList<pointSymmTensorField> pSymmtf;
    PtrList<pointTensorField> ptf;
    if (!noPointValues && !(specifiedFields && selectedFields.empty()))
    {
      readFields
      (
        vMesh,
        pointMesh::New(vMesh.baseMesh()),
        objects,
        selectedFields,
        psf
      );
      print("    pointScalarFields          :", Info, psf);
      readFields
      (
        vMesh,
        pointMesh::New(vMesh.baseMesh()),
        objects,
        selectedFields,
        pvf
      );
      print("    pointVectorFields          :", Info, pvf);
      readFields
      (
        vMesh,
        pointMesh::New(vMesh.baseMesh()),
        objects,
        selectedFields,
        pSpheretf
      );
      print("    pointSphericalTensorFields :", Info, pSpheretf);
      readFields
      (
        vMesh,
        pointMesh::New(vMesh.baseMesh()),
        objects,
        selectedFields,
        pSymmtf
      );
      print("    pointSymmTensorFields      :", Info, pSymmtf);
      readFields
      (
        vMesh,
        pointMesh::New(vMesh.baseMesh()),
        objects,
        selectedFields,
        ptf
      );
      print("    pointTensorFields          :", Info, ptf);
    }
    Info<< endl;
    label nPointFields =
      psf.size()
     + pvf.size()
     + pSpheretf.size()
     + pSymmtf.size()
     + ptf.size();
    if (doWriteInternal)
    {
      //
      // Create file and write header
      //
      fileName vtkFileName
      (
        fvPath/vtkName
       + "_"
       + timeDesc
       + ".vtk"
      );
      Info<< "    Internal  : " << vtkFileName << endl;
      // Write mesh
      internalWriter writer(vMesh, binary, vtkFileName);
      // VolFields + cellID
      writeFuns::writeCellDataHeader
      (
        writer.os(),
        vMesh.nFieldCells(),
        1+nVolFields
      );
      // Write cellID field
      writer.writeCellIDs();
      // Write volFields
      writer.write(vsf);
      writer.write(vvf);
      writer.write(vSpheretf);
      writer.write(vSymmtf);
      writer.write(vtf);
      if (!noPointValues)
      {
        writeFuns::writePointDataHeader
        (
          writer.os(),
          vMesh.nFieldPoints(),
          nVolFields+nPointFields
        );
        // pointFields
        writer.write(psf);
        writer.write(pvf);
        writer.write(pSpheretf);
        writer.write(pSymmtf);
        writer.write(ptf);
        // Interpolated volFields
        volPointInterpolation pInterp(mesh);
        writer.write(pInterp, vsf);
        writer.write(pInterp, vvf);
        writer.write(pInterp, vSpheretf);
        writer.write(pInterp, vSymmtf);
        writer.write(pInterp, vtf);
      }
    }
    // Write surface fields
    if (args.optionFound("surfaceFields"))
    {
      PtrList<surfaceScalarField> ssf;
      readFields
      (
        vMesh,
        vMesh.baseMesh(),
        objects,
        selectedFields,
        ssf
      );
      print("    surfScalarFields  :", Info, ssf);
      PtrList<surfaceVectorField> svf;
      readFields
      (
        vMesh,
        vMesh.baseMesh(),
        objects,
        selectedFields,
        svf
      );
      print("    surfVectorFields  :", Info, svf);
      if (ssf.size() + svf.size() > 0)
      {
        // Rework the scalar fields into vectorfields.
        label sz = svf.size();
        svf.setSize(sz+ssf.size());
        surfaceVectorField n(mesh.Sf()/mesh.magSf());
        FOR_ALL(ssf, i)
        {
          svf.set(sz+i, ssf[i]*n);
          svf[sz+i].rename(ssf[i].name());
        }
        ssf.clear();
        mkDir(fvPath / "surfaceFields");
        fileName surfFileName
        (
          fvPath
         /"surfaceFields"
         /"surfaceFields"
         + "_"
         + timeDesc
         + ".vtk"
        );
        writeSurfFields
        (
          binary,
          vMesh,
          surfFileName,
          svf
        );
      }
    }
    // Write patches (POLYDATA file, one for each patch)
    const polyBoundaryMesh& patches = mesh.boundaryMesh();
    if (allPatches)
    {
      mkDir(fvPath/"allPatches");
      fileName patchFileName;
      if (vMesh.useSubMesh())
      {
        patchFileName =
          fvPath/"allPatches"/cellSetName
         + "_"
         + timeDesc
         + ".vtk";
      }
      else
      {
        patchFileName =
          fvPath/"allPatches"/"allPatches"
         + "_"
         + timeDesc
         + ".vtk";
      }
      Info<< "    Combined patches     : " << patchFileName << endl;
      patchWriter writer
      (
        vMesh,
        binary,
        nearCellValue,
        patchFileName,
        getSelectedPatches(patches, excludePatches)
      );
      // VolFields + patchID
      writeFuns::writeCellDataHeader
      (
        writer.os(),
        writer.nFaces(),
        1+nVolFields
      );
      // Write patchID field
      writer.writePatchIDs();
      // Write volFields
      writer.write(vsf);
      writer.write(vvf);
      writer.write(vSpheretf);
      writer.write(vSymmtf);
      writer.write(vtf);
      if (!noPointValues)
      {
        writeFuns::writePointDataHeader
        (
          writer.os(),
          writer.nPoints(),
          nPointFields
        );
        // Write pointFields
        writer.write(psf);
        writer.write(pvf);
        writer.write(pSpheretf);
        writer.write(pSymmtf);
        writer.write(ptf);
        // no interpolated volFields since I cannot be bothered to
        // create the patchInterpolation for all subpatches.
      }
    }
    else
    {
      FOR_ALL(patches, patchI)
      {
        const polyPatch& pp = patches[patchI];
        if (!findStrings(excludePatches, pp.name()))
        {
          mkDir(fvPath/pp.name());
          fileName patchFileName;
          if (vMesh.useSubMesh())
          {
            patchFileName =
              fvPath/pp.name()/cellSetName
             + "_"
             + timeDesc
             + ".vtk";
          }
          else
          {
            patchFileName =
              fvPath/pp.name()/pp.name()
             + "_"
             + timeDesc
             + ".vtk";
          }
          Info<< "    Patch     : " << patchFileName << endl;
          patchWriter writer
          (
            vMesh,
            binary,
            nearCellValue,
            patchFileName,
            labelList(1, patchI)
          );
          if (!isA<emptyPolyPatch>(pp))
          {
            // VolFields + patchID
            writeFuns::writeCellDataHeader
            (
              writer.os(),
              writer.nFaces(),
              1+nVolFields
            );
            // Write patchID field
            writer.writePatchIDs();
            // Write volFields
            writer.write(vsf);
            writer.write(vvf);
            writer.write(vSpheretf);
            writer.write(vSymmtf);
            writer.write(vtf);
            if (!noPointValues)
            {
              writeFuns::writePointDataHeader
              (
                writer.os(),
                writer.nPoints(),
                nVolFields
               + nPointFields
              );
              // Write pointFields
              writer.write(psf);
              writer.write(pvf);
              writer.write(pSpheretf);
              writer.write(pSymmtf);
              writer.write(ptf);
              PrimitivePatchInterpolation<primitivePatch> pInter
              (
                pp
              );
              // Write interpolated volFields
              writer.write(pInter, vsf);
              writer.write(pInter, vvf);
              writer.write(pInter, vSpheretf);
              writer.write(pInter, vSymmtf);
              writer.write(pInter, vtf);
            }
          }
        }
      }
    }
    // Write faceZones (POLYDATA file, one for each zone)
    if (doFaceZones)
    {
      PtrList<surfaceScalarField> ssf;
      readFields
      (
        vMesh,
        vMesh.baseMesh(),
        objects,
        selectedFields,
        ssf
      );
      print("    surfScalarFields  :", Info, ssf);
      PtrList<surfaceVectorField> svf;
      readFields
      (
        vMesh,
        vMesh.baseMesh(),
        objects,
        selectedFields,
        svf
      );
      print("    surfVectorFields  :", Info, svf);
      const faceZoneMesh& zones = mesh.faceZones();
      FOR_ALL(zones, zoneI)
      {
        const faceZone& fz = zones[zoneI];
        mkDir(fvPath/fz.name());
        fileName patchFileName;
        if (vMesh.useSubMesh())
        {
          patchFileName =
            fvPath/fz.name()/cellSetName
           + "_"
           + timeDesc
           + ".vtk";
        }
        else
        {
          patchFileName =
            fvPath/fz.name()/fz.name()
           + "_"
           + timeDesc
           + ".vtk";
        }
        Info<< "    FaceZone  : " << patchFileName << endl;
        indirectPrimitivePatch pp
        (
          IndirectList<face>(mesh.faces(), fz),
          mesh.points()
        );
        surfaceMeshWriter writer
        (
          binary,
          pp,
          fz.name(),
          patchFileName
        );
        // Number of fields
        writeFuns::writeCellDataHeader
        (
          writer.os(),
          pp.size(),
          ssf.size()+svf.size()
        );
        writer.write(ssf);
        writer.write(svf);
      }
    }
    // Write lagrangian data
    FOR_ALL_CONST_ITER(HashSet<fileName>, allCloudDirs, iter)
    {
      const fileName& cloudName = iter.key();
      // Always create the cloud directory.
      mkDir(fvPath/cloud::prefix/cloudName);
      fileName lagrFileName
      (
        fvPath/cloud::prefix/cloudName/cloudName
       + "_" + timeDesc + ".vtk"
      );
      Info<< "    Lagrangian: " << lagrFileName << endl;
      IOobjectList sprayObjs
      (
        mesh,
        runTime.timeName(),
        cloud::prefix/cloudName
      );
      IOobject* positionsPtr = sprayObjs.lookup(word("positions"));
      if (positionsPtr)
      {
        wordList labelNames(sprayObjs.names(labelIOField::typeName));
        Info<< "        labels            :";
        print(Info, labelNames);
        wordList scalarNames(sprayObjs.names(scalarIOField::typeName));
        Info<< "        scalars           :";
        print(Info, scalarNames);
        wordList vectorNames(sprayObjs.names(vectorIOField::typeName));
        Info<< "        vectors           :";
        print(Info, vectorNames);
        wordList sphereNames
        (
          sprayObjs.names
          (
            sphericalTensorIOField::typeName
          )
        );
        Info<< "        spherical tensors :";
        print(Info, sphereNames);
        wordList symmNames
        (
          sprayObjs.names
          (
            symmTensorIOField::typeName
          )
        );
        Info<< "        symm tensors      :";
        print(Info, symmNames);
        wordList tensorNames(sprayObjs.names(tensorIOField::typeName));
        Info<< "        tensors           :";
        print(Info, tensorNames);
        lagrangianWriter writer
        (
          vMesh,
          binary,
          lagrFileName,
          cloudName,
          false
        );
        // Write number of fields
        writer.writeParcelHeader
        (
          labelNames.size()
         + scalarNames.size()
         + vectorNames.size()
         + sphereNames.size()
         + symmNames.size()
         + tensorNames.size()
        );
        // Fields
        writer.writeIOField<label>(labelNames);
        writer.writeIOField<scalar>(scalarNames);
        writer.writeIOField<vector>(vectorNames);
        writer.writeIOField<sphericalTensor>(sphereNames);
        writer.writeIOField<symmTensor>(symmNames);
        writer.writeIOField<tensor>(tensorNames);
      }
      else
      {
        lagrangianWriter writer
        (
          vMesh,
          binary,
          lagrFileName,
          cloudName,
          true
        );
        // Write number of fields
        writer.writeParcelHeader(0);
      }
    }
  }
  // Link parallel outputs back to undecomposed case for ease of loading
  if (Pstream::parRun() && doLinks)
  {
    mkDir(runTime.path()/".."/"VTK");
    chDir(runTime.path()/".."/"VTK");
    Info<< "Linking all processor files to " << runTime.path()/".."/"VTK"
      << endl;
    // Get list of vtk files
    fileName procVTK
    (
      fileName("..")
     /"processor" + name(Pstream::myProcNo())
     /"VTK"
    );
    fileNameList dirs(readDir(procVTK, fileName::DIRECTORY));
    label sz = dirs.size();
    dirs.setSize(sz+1);
    dirs[sz] = ".";
    FOR_ALL(dirs, i)
    {
      fileNameList subFiles(readDir(procVTK/dirs[i], fileName::FILE));
      FOR_ALL(subFiles, j)
      {
        fileName procFile(procVTK/dirs[i]/subFiles[j]);
        if (exists(procFile))
        {
          string cmd
          (
            "ln -s "
           + procFile
           + " "
           + "processor"
           + name(Pstream::myProcNo())
           + "_"
           + procFile.name()
          );
          if (system(cmd.c_str()) == -1)
          {
            WARNING_IN(args.executable())
              << "Could not execute command " << cmd << endl;
          }
        }
      }
    }
  }
  Info<< "End\n" << endl;
  return 0;
}
