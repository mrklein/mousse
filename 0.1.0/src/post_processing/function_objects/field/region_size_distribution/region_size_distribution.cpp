// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_size_distribution.hpp"
#include "vol_fields.hpp"
#include "region_split.hpp"
#include "fvc_volume_integrate.hpp"
#include "mathematical_constants.hpp"
#include "string_list_ops.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(regionSizeDistribution, 0);

//- Plus op for FixedList<scalar>
template<class T, unsigned Size>
class ListPlusEqOp
{
  public:
  void operator()
  (
    FixedList<T, Size>& x,
    const FixedList<T, Size>& y
  ) const
  {
    FOR_ALL(x, i) {
      x[i] += y[i];
    }
  }
};

}


// Private Member Functions 
void mousse::regionSizeDistribution::writeGraph
(
  const coordSet& coords,
  const word& valueName,
  const scalarField& values
) const
{
  const wordList valNames{1, valueName};
  fileName outputPath = baseTimeDir();
  mkDir(outputPath);
  OFstream str{outputPath/formatterPtr_().getFileName(coords, valNames)};
  Info << "Writing distribution of " << valueName << " to " << str.name()
    << endl;
  List<const scalarField*> valPtrs{1};
  valPtrs[0] = &values;
  formatterPtr_().write(coords, valNames, valPtrs, str);
}


void mousse::regionSizeDistribution::writeAlphaFields
(
  const regionSplit& regions,
  const Map<label>& patchRegions,
  const Map<scalar>& regionVolume,
  const volScalarField& alpha
) const
{
  const scalar maxDropletVol = pow(maxDiam_, 3)/6.0;
  // Split alpha field
  // ~~~~~~~~~~~~~~~~~
  // Split into
  //  - liquidCore            : region connected to inlet patches
  //  - per region a volume   : for all other regions
  //  - backgroundAlpha       : remaining alpha
  // Construct field
  volScalarField liquidCore
  {
    // IOobject
    {
      alphaName_ + "_liquidCore",
      obr_.time().timeName(),
      obr_,
      IOobject::NO_READ
    },
    alpha,
    fvPatchField<scalar>::calculatedType()
  };
  volScalarField backgroundAlpha
  {
    // IOobject
    {
      alphaName_ + "_background",
      obr_.time().timeName(),
      obr_,
      IOobject::NO_READ
    },
    alpha,
    fvPatchField<scalar>::calculatedType()
  };
  // Knock out any cell not in patchRegions
  FOR_ALL(liquidCore, cellI) {
    label regionI = regions[cellI];
    if (patchRegions.found(regionI)) {
      backgroundAlpha[cellI] = 0;
    } else {
      liquidCore[cellI] = 0;
      scalar regionVol = regionVolume[regionI];
      if (regionVol < maxDropletVol) {
        backgroundAlpha[cellI] = 0;
      }
    }
  }
  liquidCore.correctBoundaryConditions();
  backgroundAlpha.correctBoundaryConditions();
  Info << "    Volume of liquid-core = "
    << fvc::domainIntegrate(liquidCore).value()
    << endl;
  Info << "    Volume of background  = "
    << fvc::domainIntegrate(backgroundAlpha).value()
    << endl;
  Info << "Writing liquid-core field to " << liquidCore.name() << endl;
  liquidCore.write();
  Info << "Writing background field to " << backgroundAlpha.name() << endl;
  backgroundAlpha.write();
}


mousse::Map<mousse::label> mousse::regionSizeDistribution::findPatchRegions
(
  const polyMesh& mesh,
  const regionSplit& regions
) const
{
  // Mark all regions starting at patches
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Count number of patch faces (just for initial sizing)
  const labelHashSet patchIDs{mesh.boundaryMesh().patchSet(patchNames_)};
  label nPatchFaces = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs, iter) {
    nPatchFaces += mesh.boundaryMesh()[iter.key()].size();
  }
  Map<label> patchRegions{nPatchFaces};
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs, iter) {
    const polyPatch& pp = mesh.boundaryMesh()[iter.key()];
    // Collect all regions on the patch
    const labelList& faceCells = pp.faceCells();
    FOR_ALL(faceCells, i) {
      patchRegions.insert
      (
        regions[faceCells[i]],
        Pstream::myProcNo()     // dummy value
      );
    }
  }
  // Make sure all the processors have the same set of regions
  Pstream::mapCombineGather(patchRegions, minEqOp<label>());
  Pstream::mapCombineScatter(patchRegions);
  return patchRegions;
}


mousse::tmp<mousse::scalarField> mousse::regionSizeDistribution::divide
(
  const scalarField& num,
  const scalarField& denom
)
{
  tmp<scalarField> tresult{new scalarField{num.size()}};
  scalarField& result = tresult();
  FOR_ALL(denom, i) {
    if (denom[i] != 0) {
      result[i] = num[i]/denom[i];
    } else {
      result[i] = 0.0;
    }
  }
  return tresult;
}


void mousse::regionSizeDistribution::writeGraphs
(
  const word& fieldName,              // name of field
  const labelList& indices,           // index of bin for each region
  const scalarField& sortedField,     // per region field data
  const scalarField& binCount,        // per bin number of regions
  const coordSet& coords              // graph data for bins
) const
{
  if (!Pstream::master())
    return;
  // Calculate per-bin average
  scalarField binSum{nBins_, 0.0};
  FOR_ALL(sortedField, i) {
    binSum[indices[i]] += sortedField[i];
  }
  scalarField binAvg{divide(binSum, binCount)};
  // Per bin deviation
  scalarField binSqrSum{nBins_, 0.0};
  FOR_ALL(sortedField, i) {
    binSqrSum[indices[i]] += mousse::sqr(sortedField[i]);
  }
  scalarField binDev
  {
    sqrt(divide(binSqrSum, binCount) - mousse::sqr(binAvg))
  };
  // Write average
  writeGraph(coords, fieldName + "_sum", binSum);
  // Write average
  writeGraph(coords, fieldName + "_avg", binAvg);
  // Write deviation
  writeGraph(coords, fieldName + "_dev", binDev);
}


void mousse::regionSizeDistribution::writeGraphs
(
  const word& fieldName,              // name of field
  const scalarField& cellField,       // per cell field data
  const regionSplit& regions,         // per cell the region(=droplet)
  const labelList& sortedRegions,     // valid regions in sorted order
  const scalarField& sortedNormalisation,
  const labelList& indices,           // per region index of bin
  const scalarField& binCount,        // per bin number of regions
  const coordSet& coords              // graph data for bins
) const
{
  // Sum on a per-region basis. Parallel reduced.
  Map<scalar> regionField{regionSum(regions, cellField)};
  // Extract in region order
  scalarField sortedField
  {
    sortedNormalisation*extractData(sortedRegions, regionField)
  };
  writeGraphs
  (
    fieldName,      // name of field
    indices,        // index of bin for each region
    sortedField,    // per region field data
    binCount,       // per bin number of regions
    coords          // graph data for bins
  );
}


// Constructors 
mousse::regionSizeDistribution::regionSizeDistribution
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  functionObjectFile{obr, name, typeName},
  name_{name},
  obr_{obr},
  active_{true},
  alphaName_{dict.lookup("field")},
  patchNames_{dict.lookup("patches")}
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (isA<fvMesh>(obr_)) {
    read(dict);
  } else {
    active_ = false;
    WARNING_IN
    (
      "regionSizeDistribution::regionSizeDistribution"
      "("
      "  const word&,  "
      "  const objectRegistry&, "
      "  const dictionary&, "
      "  const bool"
      ")"
    )
    << "No fvMesh available, deactivating " << name_ << nl
    << endl;
  }
}


// Destructor 
mousse::regionSizeDistribution::~regionSizeDistribution()
{}


// Member Functions 
void mousse::regionSizeDistribution::read(const dictionary& dict)
{
  if (!active_)
    return;
  dict.lookup("field") >> alphaName_;
  dict.lookup("patches") >> patchNames_;
  dict.lookup("threshold") >> threshold_;
  dict.lookup("maxDiameter") >> maxDiam_;
  minDiam_ = 0.0;
  dict.readIfPresent("minDiameter", minDiam_);
  dict.lookup("nBins") >> nBins_;
  dict.lookup("fields") >> fields_;
  word format(dict.lookup("setFormat"));
  formatterPtr_ = writer<scalar>::New(format);
  if (dict.found("coordinateSystem")) {
    coordSysPtr_.reset(new coordinateSystem{obr_, dict});
    Info << "Transforming all vectorFields with coordinate system "
      << coordSysPtr_().name() << endl;
  }
}


void mousse::regionSizeDistribution::execute()
{
  // Do nothing - only valid on write
}


void mousse::regionSizeDistribution::end()
{
  // Do nothing - only valid on write
}


void mousse::regionSizeDistribution::timeSet()
{
  // Do nothing - only valid on write
}


void mousse::regionSizeDistribution::write()
{
  if (!active_)
    return;
  Info << type() << " " << name_ << " output:" << nl;
  const fvMesh& mesh = refCast<const fvMesh>(obr_);
  autoPtr<volScalarField> alphaPtr;
  if (obr_.foundObject<volScalarField>(alphaName_)) {
    Info << "    Looking up field " << alphaName_ << endl;
  } else {
    Info << "    Reading field " << alphaName_ << endl;
    alphaPtr.reset
    (
      new volScalarField
      {
        {
          alphaName_,
          mesh.time().timeName(),
          mesh,
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        },
        mesh
      }
    );
  }
  const volScalarField& alpha =
    (
      alphaPtr.valid()
      ? alphaPtr()
      : obr_.lookupObject<volScalarField>(alphaName_)
    );
  Info << "    Volume of alpha          = "
    << fvc::domainIntegrate(alpha).value()
    << endl;
  const scalar meshVol = gSum(mesh.V());
  const scalar maxDropletVol = 1.0/6.0*pow(maxDiam_, 3);
  const scalar delta = (maxDiam_ - minDiam_)/nBins_;
  Info << "    Mesh volume              = " << meshVol << endl;
  Info << "    Maximum droplet diameter = " << maxDiam_ << endl;
  Info << "    Maximum droplet volume   = " << maxDropletVol << endl;
  // Determine blocked faces
  boolList blockedFace{mesh.nFaces(), false};
  label nBlocked = 0;

  {
    for (label faceI = 0; faceI < mesh.nInternalFaces(); faceI++) {
      scalar ownVal = alpha[mesh.faceOwner()[faceI]];
      scalar neiVal = alpha[mesh.faceNeighbour()[faceI]];
      if ((ownVal < threshold_ && neiVal > threshold_)
          || (ownVal > threshold_ && neiVal < threshold_)) {
        blockedFace[faceI] = true;
        nBlocked++;
      }
    }
    // Block coupled faces
    FOR_ALL(alpha.boundaryField(), patchI) {
      const fvPatchScalarField& fvp = alpha.boundaryField()[patchI];
      if (!fvp.coupled())
        continue;
      tmp<scalarField> townFld{fvp.patchInternalField()};
      const scalarField& ownFld = townFld();
      tmp<scalarField> tnbrFld{fvp.patchNeighbourField()};
      const scalarField& nbrFld = tnbrFld();
      label start = fvp.patch().patch().start();
      FOR_ALL(ownFld, i) {
        scalar ownVal = ownFld[i];
        scalar neiVal = nbrFld[i];
        if ((ownVal < threshold_ && neiVal > threshold_)
            || (ownVal > threshold_ && neiVal < threshold_)) {
          blockedFace[start+i] = true;
          nBlocked++;
        }
      }
    }
  }
  regionSplit regions{mesh, blockedFace};
  Info << "    Determined " << regions.nRegions()
    << " disconnected regions" << endl;
  if (debug) {
    volScalarField region
    {
      {
        "region",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      {"zero", dimless, 0}
    };
    Info << "    Dumping region as volScalarField to " << region.name()
      << endl;
    FOR_ALL(regions, cellI) {
      region[cellI] = regions[cellI];
    }
    region.correctBoundaryConditions();
    region.write();
  }
  // Determine regions connected to supplied patches
  Map<label> patchRegions{findPatchRegions(mesh, regions)};
  // Sum all regions
  const scalarField alphaVol{alpha.internalField()*mesh.V()};
  Map<scalar> allRegionVolume{regionSum(regions, mesh.V())};
  Map<scalar> allRegionAlphaVolume{regionSum(regions, alphaVol)};
  Map<label> allRegionNumCells
  {
    regionSum(regions, labelField{mesh.nCells(), 1.0})
  };
  if (debug) {
    Info << "    " << token::TAB << "Region"
      << token::TAB << "Volume(mesh)"
      << token::TAB << "Volume(" << alpha.name() << "):"
      << token::TAB << "nCells"
      << endl;
    scalar meshSumVol = 0.0;
    scalar alphaSumVol = 0.0;
    label nCells = 0;
    Map<scalar>::const_iterator vIter = allRegionVolume.begin();
    Map<scalar>::const_iterator aIter = allRegionAlphaVolume.begin();
    Map<label>::const_iterator numIter = allRegionNumCells.begin();
    for (;
         vIter != allRegionVolume.end()
         && aIter != allRegionAlphaVolume.end();
         ++vIter, ++aIter, ++numIter) {
      Info << "    " << token::TAB << vIter.key()
        << token::TAB << vIter()
        << token::TAB << aIter()
        << token::TAB << numIter()
        << endl;
      meshSumVol += vIter();
      alphaSumVol += aIter();
      nCells += numIter();
    }
    Info << "    " << token::TAB << "Total:"
      << token::TAB << meshSumVol
      << token::TAB << alphaSumVol
      << token::TAB << nCells
      << endl;
    Info << endl;
  }
  {
    Info << "    Patch connected regions (liquid core):" << endl;
    Info << token::TAB << "    Region"
      << token::TAB << "Volume(mesh)"
      << token::TAB << "Volume(" << alpha.name() << "):"
      << endl;
    FOR_ALL_CONST_ITER(Map<label>, patchRegions, iter) {
      label regionI = iter.key();
      Info << "    " << token::TAB << iter.key()
        << token::TAB << allRegionVolume[regionI]
        << token::TAB << allRegionAlphaVolume[regionI] << endl;
    }
    Info << endl;
  }
  {
    Info << "    Background regions:" << endl;
    Info << "    " << token::TAB << "Region"
      << token::TAB << "Volume(mesh)"
      << token::TAB << "Volume(" << alpha.name() << "):"
      << endl;
    Map<scalar>::const_iterator vIter = allRegionVolume.begin();
    Map<scalar>::const_iterator aIter = allRegionAlphaVolume.begin();
    for (;
         vIter != allRegionVolume.end()
         && aIter != allRegionAlphaVolume.end();
         ++vIter, ++aIter) {
      if (!patchRegions.found(vIter.key())
          && vIter() >= maxDropletVol) {
        Info << "    " << token::TAB << vIter.key()
          << token::TAB << vIter()
          << token::TAB << aIter() << endl;
      }
    }
    Info << endl;
  }
  // Split alpha field
  // ~~~~~~~~~~~~~~~~~
  // Split into
  //  - liquidCore            : region connected to inlet patches
  //  - per region a volume   : for all other regions
  //  - backgroundAlpha       : remaining alpha
  writeAlphaFields(regions, patchRegions, allRegionVolume, alpha);
  // Extract droplet-only allRegionVolume, i.e. delete liquid core
  // (patchRegions) and background regions from maps.
  // Note that we have to use mesh volume (allRegionVolume) and not
  // allRegionAlphaVolume since background might not have alpha in it.
  FOR_ALL_ITER(Map<scalar>, allRegionVolume, vIter) {
    label regionI = vIter.key();
    if (patchRegions.found(regionI) || vIter() >= maxDropletVol) {
      allRegionVolume.erase(vIter);
      allRegionAlphaVolume.erase(regionI);
      allRegionNumCells.erase(regionI);
    }
  }
  if (allRegionVolume.size()) {
    // Construct mids of bins for plotting
    pointField xBin{nBins_};
    scalar x = 0.5*delta;
    FOR_ALL(xBin, i) {
      xBin[i] = point{x, 0, 0};
      x += delta;
    }
    const coordSet coords{"diameter", "x", xBin, mag(xBin)};
    // Get in region order the alpha*volume and diameter
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    const labelList sortedRegions = allRegionAlphaVolume.sortedToc();
    scalarField sortedVols
    {
      extractData(sortedRegions, allRegionAlphaVolume)
    };
    // Calculate the diameters
    scalarField sortedDiameters{sortedVols.size()};
    FOR_ALL(sortedDiameters, i) {
      sortedDiameters[i] =
        mousse::cbrt(sortedVols[i]*6/constant::mathematical::pi);
    }
    // Determine the bin index for all the diameters
    labelList indices{sortedDiameters.size()};
    FOR_ALL(sortedDiameters, i) {
      indices[i] = (sortedDiameters[i]-minDiam_)/delta;
    }
    // Calculate the counts per diameter bin
    scalarField binCount{nBins_, 0.0};
    FOR_ALL(sortedDiameters, i) {
      binCount[indices[i]] += 1.0;
    }
    // Write counts
    if (Pstream::master()) {
      writeGraph(coords, "count", binCount);
    }
    // Write to screen
    {
      Info << "    Bins:" << endl;
      Info << "    " << token::TAB << "Bin"
        << token::TAB << "Min diameter"
        << token::TAB << "Count:"
        << endl;
      scalar diam = 0.0;
      FOR_ALL(binCount, binI) {
        Info << "    " << token::TAB << binI
          << token::TAB << diam
          << token::TAB << binCount[binI] << endl;
        diam += delta;
      }
      Info << endl;
    }
    // Write average and deviation of droplet volume.
    writeGraphs
    (
      "volume",           // name of field
      indices,            // per region the bin index
      sortedVols,         // per region field data
      binCount,           // per bin number of regions
      coords              // graph data for bins
    );
    // Collect some more field
    {
      wordList scalarNames{obr_.names(volScalarField::typeName)};
      labelList selected = findStrings(fields_, scalarNames);
      FOR_ALL(selected, i) {
        const word& fldName = scalarNames[selected[i]];
        Info << "    Scalar field " << fldName << endl;
        const scalarField& fld =
          obr_.lookupObject<volScalarField>(fldName).internalField();
        writeGraphs
        (
          fldName,            // name of field
          alphaVol*fld,       // per cell field data
          regions,            // per cell the region(=droplet)
          sortedRegions,      // valid regions in sorted order
          1.0/sortedVols,     // per region normalisation
          indices,            // index of bin for each region
          binCount,           // per bin number of regions
          coords              // graph data for bins
        );
      }
    }
    {
      wordList vectorNames{obr_.names(volVectorField::typeName)};
      labelList selected = findStrings(fields_, vectorNames);
      FOR_ALL(selected, i) {
        const word& fldName = vectorNames[selected[i]];
        Info << "    Vector field " << fldName << endl;
        vectorField fld =
          obr_.lookupObject<volVectorField>(fldName).internalField();
        if (coordSysPtr_.valid()) {
          Info << "Transforming vector field " << fldName
            << " with coordinate system "
            << coordSysPtr_().name()
            << endl;
          fld = coordSysPtr_().localVector(fld);
        }
        // Components
        for (direction cmp = 0; cmp < vector::nComponents; cmp++) {
          writeGraphs
          (
            fldName + vector::componentNames[cmp],
            alphaVol*fld.component(cmp),// per cell field data
            regions,        // per cell the region(=droplet)
            sortedRegions,  // valid regions in sorted order
            1.0/sortedVols, // per region normalisation
            indices,        // index of bin for each region
            binCount,       // per bin number of regions
            coords          // graph data for bins
          );
        }
        // Magnitude
        writeGraphs
        (
          fldName + "mag",    // name of field
          alphaVol*mag(fld),  // per cell field data
          regions,            // per cell the region(=droplet)
          sortedRegions,      // valid regions in sorted order
          1.0/sortedVols,     // per region normalisation
          indices,            // index of bin for each region
          binCount,           // per bin number of regions
          coords              // graph data for bins
        );
      }
    }
  }
}

