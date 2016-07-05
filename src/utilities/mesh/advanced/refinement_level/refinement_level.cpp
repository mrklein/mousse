// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "cell_set.hpp"
#include "sortable_list.hpp"
#include "label_io_list.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"


using namespace mousse;


// Return true if any cells had to be split to keep a difference between
// neighbouring refinement levels < limitDiff. Puts cells into refCells and
// update refLevel to account for refinement.
bool limitRefinementLevel
(
  const primitiveMesh& mesh,
  labelList& refLevel,
  cellSet& refCells
)
{
  const labelListList& cellCells = mesh.cellCells();
  label oldNCells = refCells.size();
  FOR_ALL(cellCells, cellI) {
    const labelList& cCells = cellCells[cellI];
    FOR_ALL(cCells, i) {
      if (refLevel[cCells[i]] > (refLevel[cellI]+1)) {
        // Found neighbour with >=2 difference in refLevel.
        refCells.insert(cellI);
        refLevel[cellI]++;
        break;
      }
    }
  }
  if (refCells.size() > oldNCells) {
    Info << "Added an additional " << refCells.size() - oldNCells
      << " cells to satisfy 1:2 refinement level"
      << endl;
    return true;
  } else {
    return false;
  }
}


int main(int argc, char *argv[])
{
  argList::addBoolOption
  (
    "readLevel",
    "read level from refinementLevel file"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_poly_mesh.inc"
  Info << "Dividing cells into bins depending on cell volume.\nThis will"
    << " correspond to refinement levels for a mesh with only 2x2x2"
    << " refinement\n"
    << "The upper range for every bin is always 1.1 times the lower range"
    << " to allow for some truncation error."
    << nl << endl;
  const bool readLevel = args.optionFound("readLevel");
  const scalarField& vols = mesh.cellVolumes();
  SortableList<scalar> sortedVols{vols};
  // All cell labels, sorted per bin.
  DynamicList<DynamicList<label>> bins;
  // Lower/upper limits
  DynamicList<scalar> lowerLimits;
  DynamicList<scalar> upperLimits;
  // Create bin0. Have upperlimit as factor times lowerlimit.
  bins.append(DynamicList<label>{});
  lowerLimits.append(sortedVols[0]);
  upperLimits.append(1.1*lowerLimits.last());
  FOR_ALL(sortedVols, i) {
    if (sortedVols[i] > upperLimits.last()) {
      // New value outside of current bin
      // Shrink old bin.
      DynamicList<label>& bin = bins.last();
      bin.shrink();
      Info << "Collected " << bin.size() << " elements in bin "
        << lowerLimits.last() << " .. "
        << upperLimits.last() << endl;
      // Create new bin.
      bins.append(DynamicList<label>{});
      lowerLimits.append(sortedVols[i]);
      upperLimits.append(1.1*lowerLimits.last());
      Info << "Creating new bin " << lowerLimits.last()
        << " .. " << upperLimits.last()
        << endl;
    }
    // Append to current bin.
    DynamicList<label>& bin = bins.last();
    bin.append(sortedVols.indices()[i]);
  }
  Info << endl;
  bins.last().shrink();
  bins.shrink();
  lowerLimits.shrink();
  upperLimits.shrink();
  // Write to cellSets.
  Info << "Volume bins:" << nl;
  FOR_ALL(bins, binI) {
    const DynamicList<label>& bin = bins[binI];
    cellSet cells{mesh, "vol" + name(binI), bin.size()};
    FOR_ALL(bin, i) {
      cells.insert(bin[i]);
    }
    Info << "    " << lowerLimits[binI] << " .. " << upperLimits[binI]
      << "  : writing " << bin.size() << " cells to cellSet "
      << cells.name() << endl;
    cells.write();
  }
  // Convert bins into refinement level.
  // Construct fvMesh to be able to construct volScalarField
  fvMesh fMesh
  {
    {
      fvMesh::defaultRegion,
      runTime.timeName(),
      runTime
    },
    xferCopy(mesh.points()),   // could we safely re-use the data?
    xferCopy(mesh.faces()),
    xferCopy(mesh.cells())
  };
  // Add the boundary patches
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  List<polyPatch*> p{patches.size()};
  FOR_ALL(p, patchI) {
    p[patchI] = patches[patchI].clone(fMesh.boundaryMesh()).ptr();
  }
  fMesh.addFvPatches(p);
  // Refinement level
  IOobject refHeader
  {
    "refinementLevel",
    runTime.timeName(),
    polyMesh::defaultRegion,
    runTime
  };
  if (!readLevel && refHeader.headerOk()) {
    WARNING_IN(args.executable())
      << "Detected " << refHeader.name() << " file in "
      << polyMesh::defaultRegion <<  " directory. Please remove to"
      << " recreate it or use the -readLevel option to use it"
      << endl;
    return 1;
  }
  labelIOList refLevel
  {
    {
      "refinementLevel",
      runTime.timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    labelList{mesh.nCells(), 0}
  };
  if (readLevel) {
    refLevel = labelIOList(refHeader);
  }
  // Construct volScalarField with same info for post processing
  volScalarField postRefLevel
  {
    {
      "refinementLevel",
      runTime.timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    fMesh,
    {"zero", dimless/dimTime, 0}
  };
  // Set cell values
  FOR_ALL(bins, binI) {
    const DynamicList<label>& bin = bins[binI];
    FOR_ALL(bin, i) {
      refLevel[bin[i]] = bins.size() - binI - 1;
      postRefLevel[bin[i]] = refLevel[bin[i]];
    }
  }
  // For volScalarField: set boundary values to same as cell.
  // Note: could also put
  // zeroGradient b.c. on postRefLevel and do evaluate.
  FOR_ALL(postRefLevel.boundaryField(), patchI) {
    const polyPatch& pp = patches[patchI];
    fvPatchScalarField& bField = postRefLevel.boundaryField()[patchI];
    Info << "Setting field for patch "<< endl;
    FOR_ALL(bField, faceI) {
      label own = mesh.faceOwner()[pp.start() + faceI];
      bField[faceI] = postRefLevel[own];
    }
  }
  Info << "Determined current refinement level and writing to "
    << postRefLevel.name() << " (as volScalarField; for post processing)"
    << nl
    << polyMesh::defaultRegion/refLevel.name()
    << " (as labelIOList; for meshing)" << nl
    << endl;
  refLevel.write();
  postRefLevel.write();
  // Find out cells to refine to keep to 2:1 refinement level restriction
  // Cells to refine
  cellSet refCells{mesh, "refCells", 100};
  while (limitRefinementLevel
         (
           mesh,
           refLevel,       // current refinement level
           refCells        // cells to refine
         ))
  {}
  if (refCells.size()) {
    Info << "Collected " << refCells.size() << " cells that need to be"
      << " refined to get closer to overall 2:1 refinement level limit"
      << nl
      << "Written cells to be refined to cellSet " << refCells.name()
      << nl << endl;
    refCells.write();
    Info << "After refinement this tool can be run again to see if the 2:1"
      << " limit is observed all over the mesh" << nl << endl;
  } else {
    Info << "All cells in the mesh observe the 2:1 refinement level limit"
      << nl << endl;
  }
  Info << "\nEnd\n" << endl;
  return 0;
}

