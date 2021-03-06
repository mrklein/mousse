// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "layer_parameters.hpp"
#include "poly_boundary_mesh.hpp"
#include "unit_conversion.hpp"
#include "refinement_surfaces.hpp"
#include "searchable_surfaces.hpp"
#include "medial_axis_mesh_mover.hpp"


// Static Data Members
const mousse::scalar mousse::layerParameters::defaultConcaveAngle = 90;


// Private Member Functions 
mousse::scalar mousse::layerParameters::layerExpansionRatio
(
  const label n,
  const scalar totalOverFirst
) const
{
  if (n <= 1) {
    return 1.0;
  }
  //scalar totalOverFirst = totalThickness/firstLayerThickess;
  const label maxIters = 10;
  const scalar tol = 1e-8;
  if (mag(n - totalOverFirst) < tol) {
    return 1.0;
  }
  // Calculate the bounds of the solution
  scalar minR;
  scalar maxR;
  if (totalOverFirst < n) {
    minR = 0.0;
    maxR = pow(totalOverFirst/n, 1/(n - 1));
  } else {
    minR = pow(totalOverFirst/n, 1/(n - 1));
    maxR = totalOverFirst/(n - 1);
  }
  // Starting guess
  scalar r = 0.5*(minR + maxR);
  for (label i = 0; i < maxIters; ++i) {
    const scalar prevr = r;
    const scalar fx = pow(r, n) - totalOverFirst*r - (1 - totalOverFirst);
    const scalar dfx = n*pow(r, n - 1) - totalOverFirst;
    r -= fx/dfx;
    const scalar error = mag(r - prevr);
    if (error < tol) {
      break;
    }
  }
  return r;
}


// Constructors 
// Construct from dictionary
mousse::layerParameters::layerParameters
(
  const dictionary& dict,
  const polyBoundaryMesh& boundaryMesh
)
:
  dict_{dict},
  numLayers_{boundaryMesh.size(), -1},
  relativeSizes_{dict.lookup("relativeSizes")},
  layerSpec_{ILLEGAL},
  firstLayerThickness_{boundaryMesh.size(), -123},
  finalLayerThickness_{boundaryMesh.size(), -123},
  thickness_{boundaryMesh.size(), -123},
  expansionRatio_{boundaryMesh.size(), -123},
  minThickness_
  {
    boundaryMesh.size(),
    readScalar(dict.lookup("minThickness"))
  },
  featureAngle_{readScalar(dict.lookup("featureAngle"))},
  concaveAngle_
  {
    dict.lookupOrDefault("concaveAngle", defaultConcaveAngle)
  },
  nGrow_{readLabel(dict.lookup("nGrow"))},
  maxFaceThicknessRatio_
  {
    readScalar(dict.lookup("maxFaceThicknessRatio"))
  },
  nBufferCellsNoExtrude_
  {
    readLabel(dict.lookup("nBufferCellsNoExtrude"))
  },
  nLayerIter_{readLabel(dict.lookup("nLayerIter"))},
  nRelaxedIter_{labelMax},
  additionalReporting_{dict.lookupOrDefault("additionalReporting", false)},
  meshShrinker_
  {
    dict.lookupOrDefault
    (
      "meshShrinker",
      medialAxisMeshMover::typeName
    )
  }
{
  // Detect layer specification mode
  label nSpec = 0;
  bool haveFirst = dict.found("firstLayerThickness");
  if (haveFirst) {
    firstLayerThickness_ =
      scalarField
      {
        boundaryMesh.size(),
        readScalar(dict.lookup("firstLayerThickness"))
      };
    nSpec++;
  }
  bool haveFinal = dict.found("finalLayerThickness");
  if (haveFinal) {
    finalLayerThickness_ =
      scalarField
      {
        boundaryMesh.size(),
        readScalar(dict.lookup("finalLayerThickness"))
      };
    nSpec++;
  }
  bool haveTotal = dict.found("thickness");
  if (haveTotal) {
    thickness_ =
      scalarField
      {
        boundaryMesh.size(),
        readScalar(dict.lookup("thickness"))
      };
    nSpec++;
  }
  bool haveExp = dict.found("expansionRatio");
  if (haveExp) {
    expansionRatio_ =
      scalarField
      {
        boundaryMesh.size(),
        readScalar(dict.lookup("expansionRatio"))
      };
    nSpec++;
  }
  if (haveFirst && haveTotal) {
    layerSpec_ = FIRST_AND_TOTAL;
    Info << "Layer thickness specified as first layer and overall thickness."
      << endl;
  } else if (haveFirst && haveExp) {
    layerSpec_ = FIRST_AND_EXPANSION;
    Info << "Layer thickness specified as first layer and expansion ratio."
      << endl;
  } else if (haveFinal && haveTotal) {
    layerSpec_ = FINAL_AND_TOTAL;
    Info << "Layer thickness specified as final layer and overall thickness."
      << endl;
  } else if (haveFinal && haveExp) {
    layerSpec_ = FINAL_AND_EXPANSION;
    Info << "Layer thickness specified as final layer and expansion ratio."
      << endl;
  } else if (haveTotal && haveExp) {
    layerSpec_ = TOTAL_AND_EXPANSION;
    Info << "Layer thickness specified as overall thickness"
      << " and expansion ratio." << endl;
  }
  if (layerSpec_ == ILLEGAL || nSpec != 2) {
    FATAL_IO_ERROR_IN
    (
      "layerParameters::layerParameters"
      "(const dictionary&, const polyBoundaryMesh&)",
      dict
    )
    << "Over- or underspecified layer thickness."
    << " Please specify" << nl
    << "    first layer thickness ('firstLayerThickness')"
    << " and overall thickness ('thickness') or" << nl
    << "    first layer thickness ('firstLayerThickness')"
    << " and expansion ratio ('expansionRatio') or" << nl
    << "    final layer thickness ('finalLayerThickness')"
    << " and expansion ratio ('expansionRatio') or" << nl
    << "    final layer thickness ('finalLayerThickness')"
    << " and overall thickness ('thickness') or" << nl
    << "    overall thickness ('thickness')"
    << " and expansion ratio ('expansionRatio'"
    << exit(FatalIOError);
  }
  dict.readIfPresent("nRelaxedIter", nRelaxedIter_);
  if (nLayerIter_ < 0 || nRelaxedIter_ < 0) {
    FATAL_IO_ERROR_IN("layerParameters::layerParameters(..)", dict)
      << "Layer iterations should be >= 0." << endl
      << "nLayerIter:" << nLayerIter_
      << " nRelaxedIter:" << nRelaxedIter_
      << exit(FatalIOError);
  }
  const dictionary& layersDict = dict.subDict("layers");
  FOR_ALL_CONST_ITER(dictionary, layersDict, iter) {
    if (iter().isDict()) {
      const keyType& key = iter().keyword();
      const labelHashSet patchIDs
      {
        boundaryMesh.patchSet(List<wordRe>(1, wordRe(key)))
      };
      if (patchIDs.size() == 0) {
        IO_WARNING_IN("layerParameters::layerParameters(..)", layersDict)
          << "Layer specification for " << key
          << " does not match any patch." << endl
          << "Valid patches are " << boundaryMesh.names() << endl;
      } else {
        const dictionary& layerDict = iter().dict();
        FOR_ALL_CONST_ITER(labelHashSet, patchIDs, patchIter) {
          label patchI = patchIter.key();
          numLayers_[patchI] = readLabel(layerDict.lookup("nSurfaceLayers"));
          switch (layerSpec_) {
            case FIRST_AND_TOTAL:
              layerDict.readIfPresent
              (
                "firstLayerThickness",
                firstLayerThickness_[patchI]
              );
              layerDict.readIfPresent
              (
                "thickness",
                thickness_[patchI]
              );
              break;
            case FIRST_AND_EXPANSION:
              layerDict.readIfPresent
              (
                "firstLayerThickness",
                firstLayerThickness_[patchI]
              );
              layerDict.readIfPresent
              (
                "expansionRatio",
                expansionRatio_[patchI]
              );
              break;
            case FINAL_AND_TOTAL:
              layerDict.readIfPresent
              (
                "finalLayerThickness",
                finalLayerThickness_[patchI]
              );
              layerDict.readIfPresent
              (
                "thickness",
                thickness_[patchI]
              );
              break;
            case FINAL_AND_EXPANSION:
              layerDict.readIfPresent
              (
                "finalLayerThickness",
                finalLayerThickness_[patchI]
              );
              layerDict.readIfPresent
              (
                "expansionRatio",
                expansionRatio_[patchI]
              );
              break;
            case TOTAL_AND_EXPANSION:
              layerDict.readIfPresent
              (
                "thickness",
                thickness_[patchI]
              );
              layerDict.readIfPresent
              (
                "expansionRatio",
                expansionRatio_[patchI]
              );
              break;
            default:
              FATAL_IO_ERROR_IN
              (
                "layerParameters::layerParameters(..)",
                dict
              )
              << "problem." << exit(FatalIOError);
              break;
          }
          layerDict.readIfPresent
          (
            "minThickness",
            minThickness_[patchI]
          );
        }
      }
    }
  }
}


// Member Functions 
mousse::scalar mousse::layerParameters::layerThickness
(
  const label nLayers,
  const scalar firstLayerThickess,
  const scalar finalLayerThickess,
  const scalar totalThickness,
  const scalar expansionRatio
) const
{
  switch (layerSpec_) {
    case FIRST_AND_TOTAL:
    case FINAL_AND_TOTAL:
    case TOTAL_AND_EXPANSION:
      {
        return totalThickness;
      }
      break;
    case FIRST_AND_EXPANSION:
      {
        if (mag(expansionRatio-1) < SMALL) {
          return firstLayerThickess*nLayers;
        } else {
          return firstLayerThickess
            *(1.0 - pow(expansionRatio, nLayers))/(1.0 - expansionRatio);
        }
      }
      break;
    case FINAL_AND_EXPANSION:
      {
        if (mag(expansionRatio-1) < SMALL) {
          return finalLayerThickess*nLayers;
        } else {
          scalar invExpansion = 1.0/expansionRatio;
          const scalar n = 1.0 - pow(invExpansion, nLayers);
          const scalar d = 1.0 - invExpansion;
          return finalLayerThickess*n/d;
        }
      }
      break;
    default:
      {
        FATAL_ERROR_IN("layerParameters::layerThickness(..)")
          << "Illegal thickness specification " << layerSpec_
          << exit(FatalError);
        return -VGREAT;
      }
  }
}


mousse::scalar mousse::layerParameters::layerExpansionRatio
(
  const label nLayers,
  const scalar firstLayerThickess,
  const scalar finalLayerThickess,
  const scalar totalThickness,
  const scalar expansionRatio
) const
{
  switch (layerSpec_) {
    case FIRST_AND_EXPANSION:
    case FINAL_AND_EXPANSION:
    case TOTAL_AND_EXPANSION:
      {
        return expansionRatio;
      }
      break;
    case FIRST_AND_TOTAL:
      {
        return
          layerExpansionRatio
          (
            nLayers,
            totalThickness/firstLayerThickess
          );
      }
      break;
    case FINAL_AND_TOTAL:
      {
        return
          1.0/layerExpansionRatio(nLayers, totalThickness/finalLayerThickess);
      }
      break;
    default:
      {
        FATAL_ERROR_IN("layerParameters::layerThickness(..)")
          << "Illegal thickness specification" << exit(FatalError);
        return -VGREAT;
      }
  }
}


mousse::scalar mousse::layerParameters::firstLayerThickness
(
  const label nLayers,
  const scalar firstLayerThickess,
  const scalar finalLayerThickess,
  const scalar totalThickness,
  const scalar expansionRatio
) const
{
  switch (layerSpec_) {
    case FIRST_AND_EXPANSION:
    case FIRST_AND_TOTAL:
      {
        return firstLayerThickess;
      }
    case FINAL_AND_EXPANSION:
      {
        return finalLayerThickess*pow(1.0/expansionRatio, nLayers-1);
      }
      break;
    case FINAL_AND_TOTAL:
      {
        scalar r =
          layerExpansionRatio
          (
            nLayers,
            firstLayerThickess,
            finalLayerThickess,
            totalThickness,
            expansionRatio
          );
        return finalLayerThickess/pow(r, nLayers-1);
      }
      break;
    case TOTAL_AND_EXPANSION:
      {
        scalar r =
          finalLayerThicknessRatio
          (
            nLayers,
            expansionRatio
          );
        scalar finalThickness = r*totalThickness;
        return finalThickness/pow(expansionRatio, nLayers-1);
      }
      break;
    default:
      {
        FATAL_ERROR_IN("layerParameters::layerThickness(..)")
          << "Illegal thickness specification" << exit(FatalError);
        return -VGREAT;
      }
  }
}


mousse::scalar mousse::layerParameters::finalLayerThicknessRatio
(
  const label nLayers,
  const scalar expansionRatio
) const
{
  if (nLayers > 0) {
    if (mag(expansionRatio-1) < SMALL) {
      return 1.0/nLayers;
    } else {
      const scalar n = 1.0 - expansionRatio;
      const scalar d = 1.0 - pow(expansionRatio, nLayers);
      return
        pow(expansionRatio, nLayers - 1)*n/d;
    }
  } else {
    return 0.0;
  }
}

