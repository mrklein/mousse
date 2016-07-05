// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "hierarch_geom_decomp.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "pstream_reduce_ops.hpp"
#include "sortable_list.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(hierarchGeomDecomp, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  decompositionMethod,
  hierarchGeomDecomp,
  dictionary
);

}


void mousse::hierarchGeomDecomp::setDecompOrder()
{
  const word order(geomDecomDict_.lookup("order"));
  if (order.size() != 3) {
    FATAL_IO_ERROR_IN
    (
      "hierarchGeomDecomp::hierarchGeomDecomp"
      "(const dictionary& decompositionDict)",
      decompositionDict_
    )
    << "number of characters in order (" << order << ") != 3"
    << exit(FatalIOError);
  }
  for (label i = 0; i < 3; ++i) {
    if (order[i] == 'x') {
      decompOrder_[i] = 0;
    } else if (order[i] == 'y') {
      decompOrder_[i] = 1;
    } else if (order[i] == 'z') {
      decompOrder_[i] = 2;
    } else {
      FATAL_IO_ERROR_IN
      (
        "hierarchGeomDecomp::hierarchGeomDecomp"
        "(const dictionary& decompositionDict)",
        decompositionDict_
      )
      << "Illegal decomposition order " << order << endl
      << "It should only contain x, y or z" << exit(FatalError);
    }
  }
}


mousse::label mousse::hierarchGeomDecomp::findLower
(
  const List<scalar>& l,
  const scalar t,
  const label initLow,
  const label initHigh
)
{
  if (initHigh <= initLow) {
    return initLow;
  }
  label low = initLow;
  label high = initHigh;
  while ((high - low) > 1) {
    label mid = (low + high)/2;
    if (l[mid] < t) {
      low = mid;
    } else {
      high = mid;
    }
  }
  // high and low can still differ by one. Choose best.
  label tIndex = -1;
  if (l[high-1] < t) {
    tIndex = high;
  } else {
    tIndex = low;
  }
  return tIndex;
}


// Create a mapping between the index and the weighted size.
// For convenience, sortedWeightedSize is one size bigger than current. This
// avoids extra tests.
void mousse::hierarchGeomDecomp::calculateSortedWeightedSizes
(
  const labelList& current,
  const labelList& indices,
  const scalarField& weights,
  const label globalCurrentSize,
  scalarField& sortedWeightedSizes
)
{
  // Evaluate cumulative weights.
  sortedWeightedSizes[0] = 0;
  FOR_ALL(current, i) {
    label pointI = current[indices[i]];
    sortedWeightedSizes[i + 1] = sortedWeightedSizes[i] + weights[pointI];
  }
  // Non-dimensionalise and multiply by size.
  scalar globalCurrentLength =
    returnReduce(sortedWeightedSizes[current.size()], sumOp<scalar>());
  // Normalise weights by global sum of weights and multiply through
  // by global size.
  sortedWeightedSizes *= (globalCurrentSize/globalCurrentLength);
}


// Find position in values so between minIndex and this position there
// are wantedSize elements.
void mousse::hierarchGeomDecomp::findBinary
(
  const label sizeTol,
  const List<scalar>& values,
  const label minIndex,       // index of previous value
  const scalar minValue,      // value at minIndex
  const scalar maxValue,      // global max of values
  const scalar wantedSize,    // wanted size
  label& mid,                 // index where size of bin is
                // wantedSize (to within sizeTol)
  scalar& midValue            // value at mid
)
{
  label low = minIndex;
  scalar lowValue = minValue;
  scalar highValue = maxValue;
  // (one beyond) index of highValue
  label high = values.size();
  // Safeguards to avoid infinite loop.
  scalar midValuePrev = VGREAT;
  while (true) {
    label size = returnReduce(mid-minIndex, sumOp<label>());
    if (debug) {
      Pout << "    low:" << low << " lowValue:" << lowValue
        << " high:" << high << " highValue:" << highValue
        << " mid:" << mid << " midValue:" << midValue << endl
        << "    globalSize:" << size << " wantedSize:" << wantedSize
        << " sizeTol:" << sizeTol << endl;
    }
    if (wantedSize < size - sizeTol) {
      high = mid;
      highValue = midValue;
    } else if (wantedSize > size + sizeTol) {
      low = mid;
      lowValue = midValue;
    } else {
      break;
    }
    // Update mid, midValue
    midValue = 0.5*(lowValue+highValue);
    mid = findLower(values, midValue, low, high);
    // Safeguard if same as previous.
    bool hasNotChanged = (mag(midValue-midValuePrev) < SMALL);
    if (returnReduce(hasNotChanged, andOp<bool>())) {
      WARNING_IN("hierarchGeomDecomp::findBinary(..)")
        << "unable to find desired decomposition split, making do!"
        << endl;
      break;
    }
    midValuePrev = midValue;
  }
}


// Find position in values so between minIndex and this position there
// are wantedSize elements.
void mousse::hierarchGeomDecomp::findBinary
(
  const label sizeTol,
  const List<scalar>& sortedWeightedSizes,
  const List<scalar>& values,
  const label minIndex,       // index of previous value
  const scalar minValue,      // value at minIndex
  const scalar maxValue,      // global max of values
  const scalar wantedSize,    // wanted size
  label& mid,                 // index where size of bin is
                // wantedSize (to within sizeTol)
  scalar& midValue            // value at mid
)
{
  label low = minIndex;
  scalar lowValue = minValue;
  scalar highValue = maxValue;
  // (one beyond) index of highValue
  label high = values.size();
  // Safeguards to avoid infinite loop.
  scalar midValuePrev = VGREAT;
  while (true) {
    scalar weightedSize =
      returnReduce(sortedWeightedSizes[mid] - sortedWeightedSizes[minIndex],
                   sumOp<scalar>());
    if (debug) {
      Pout << "    low:" << low << " lowValue:" << lowValue
        << " high:" << high << " highValue:" << highValue
        << " mid:" << mid << " midValue:" << midValue << endl
        << "    globalSize:" << weightedSize
        << " wantedSize:" << wantedSize
        << " sizeTol:" << sizeTol << endl;
    }
    if (wantedSize < weightedSize - sizeTol) {
      high = mid;
      highValue = midValue;
    } else if (wantedSize > weightedSize + sizeTol) {
      low = mid;
      lowValue = midValue;
    } else {
      break;
    }
    // Update mid, midValue
    midValue = 0.5*(lowValue+highValue);
    mid = findLower(values, midValue, low, high);
    // Safeguard if same as previous.
    bool hasNotChanged = (mag(midValue-midValuePrev) < SMALL);
    if (returnReduce(hasNotChanged, andOp<bool>())) {
      WARNING_IN("hierarchGeomDecomp::findBinary(..)")
        << "unable to find desired deomposition split, making do!"
        << endl;
      break;
    }
    midValuePrev = midValue;
  }
}


// Sort points into bins according to one component. Recurses to next component.
void mousse::hierarchGeomDecomp::sortComponent
(
  const label sizeTol,
  const pointField& points,
  const labelList& current,       // slice of points to decompose
  const direction componentIndex, // index in decompOrder_
  const label mult,               // multiplication factor for finalDecomp
  labelList& finalDecomp
)
{
  // Current component
  label compI = decompOrder_[componentIndex];
  if (debug) {
    Pout << "sortComponent : Sorting slice of size " << current.size()
      << " in component " << compI << endl;
  }
  // Storage for sorted component compI
  SortableList<scalar> sortedCoord{current.size()};
  FOR_ALL(current, i) {
    label pointI = current[i];
    sortedCoord[i] = points[pointI][compI];
  }
  sortedCoord.sort();
  label globalCurrentSize = returnReduce(current.size(), sumOp<label>());
  scalar minCoord =
    returnReduce(sortedCoord.size() ? sortedCoord[0] : GREAT, minOp<scalar>());
  scalar maxCoord =
    returnReduce
    (
      sortedCoord.size() ? sortedCoord.last() : -GREAT,
      maxOp<scalar>()
    );
  if (debug) {
    Pout << "sortComponent : minCoord:" << minCoord
      << " maxCoord:" << maxCoord << endl;
  }
  // starting index (in sortedCoord) of bin (= local)
  label leftIndex = 0;
  // starting value of bin (= global since coordinate)
  scalar leftCoord = minCoord;
  // Sort bins of size n
  for (label bin = 0; bin < n_[compI]; bin++) {
    // Now we need to determine the size of the bin (dx). This is
    // determined by the 'pivot' values - everything to the left of this
    // value goes in the current bin, everything to the right into the next
    // bins.
    // Local number of elements
    label localSize = -1;     // offset from leftOffset
    // Value at right of bin (leftIndex+localSize-1)
    scalar rightCoord = -GREAT;
    if (bin == n_[compI]-1) {
      // Last bin. Copy all.
      localSize = current.size()-leftIndex;
      rightCoord = maxCoord;                  // note: not used anymore
    } else if (Pstream::nProcs() == 1) {
      // No need for binary searching of bin size
      localSize = label(current.size()/n_[compI]);
      rightCoord = sortedCoord[leftIndex+localSize];
    } else {
      // For the current bin (starting at leftCoord) we want a rightCoord
      // such that the sum of all sizes are globalCurrentSize/n_[compI].
      // We have to iterate to obtain this.
      label rightIndex = current.size();
      rightCoord = maxCoord;
      // Calculate rightIndex/rightCoord to have wanted size
      findBinary
        (
          sizeTol,
          sortedCoord,
          leftIndex,
          leftCoord,
          maxCoord,
          globalCurrentSize/n_[compI],  // wanted size
          rightIndex,
          rightCoord
        );
      localSize = rightIndex - leftIndex;
    }
    if (debug) {
      Pout << "For component " << compI << ", bin " << bin
        << " copying" << endl
        << "from " << leftCoord << " at local index "
        << leftIndex << endl
        << "to " << rightCoord << " localSize:"
        << localSize << endl
        << endl;
    }
    // Copy localSize elements starting from leftIndex.
    labelList slice(localSize);
    FOR_ALL(slice, i) {
      label pointI = current[sortedCoord.indices()[leftIndex+i]];
      // Mark point into correct bin
      finalDecomp[pointI] += bin*mult;
      // And collect for next sorting action
      slice[i] = pointI;
    }
    // Sort slice in next component
    if (componentIndex < 2) {
      string oldPrefix;
      if (debug) {
        oldPrefix = Pout.prefix();
        Pout.prefix() = "  " + oldPrefix;
      }
      sortComponent
        (
          sizeTol,
          points,
          slice,
          componentIndex+1,
          mult*n_[compI],     // Multiplier to apply to decomposition.
          finalDecomp
        );
      if (debug) {
        Pout.prefix() = oldPrefix;
      }
    }
    // Step to next bin.
    leftIndex += localSize;
    leftCoord = rightCoord;
  }
}


// Sort points into bins according to one component. Recurses to next component.
void mousse::hierarchGeomDecomp::sortComponent
(
  const label sizeTol,
  const scalarField& weights,
  const pointField& points,
  const labelList& current,       // slice of points to decompose
  const direction componentIndex, // index in decompOrder_
  const label mult,               // multiplication factor for finalDecomp
  labelList& finalDecomp
)
{
  // Current component
  label compI = decompOrder_[componentIndex];
  if (debug) {
    Pout << "sortComponent : Sorting slice of size " << current.size()
      << " in component " << compI << endl;
  }
  // Storage for sorted component compI
  SortableList<scalar> sortedCoord{current.size()};
  FOR_ALL(current, i) {
    label pointI = current[i];
    sortedCoord[i] = points[pointI][compI];
  }
  sortedCoord.sort();
  label globalCurrentSize = returnReduce(current.size(), sumOp<label>());
  // Now evaluate local cumulative weights, based on the sorting.
  // Make one bigger than the nodes.
  scalarField sortedWeightedSizes{current.size()+1, 0};
  calculateSortedWeightedSizes
    (
      current,
      sortedCoord.indices(),
      weights,
      globalCurrentSize,
      sortedWeightedSizes
    );
  scalar minCoord =
    returnReduce(sortedCoord.size() ? sortedCoord[0] : GREAT, minOp<scalar>());
  scalar maxCoord =
    returnReduce(sortedCoord.size() ? sortedCoord.last() : -GREAT,
                 maxOp<scalar>());
  if (debug) {
    Pout << "sortComponent : minCoord:" << minCoord
      << " maxCoord:" << maxCoord << endl;
  }
  // starting index (in sortedCoord) of bin (= local)
  label leftIndex = 0;
  // starting value of bin (= global since coordinate)
  scalar leftCoord = minCoord;
  // Sort bins of size n
  for (label bin = 0; bin < n_[compI]; bin++) {
    // Now we need to determine the size of the bin (dx). This is
    // determined by the 'pivot' values - everything to the left of this
    // value goes in the current bin, everything to the right into the next
    // bins.
    // Local number of elements
    label localSize = -1;     // offset from leftOffset
    // Value at right of bin (leftIndex+localSize-1)
    scalar rightCoord = -GREAT;
    if (bin == n_[compI]-1) {
      // Last bin. Copy all.
      localSize = current.size()-leftIndex;
      rightCoord = maxCoord;                  // note: not used anymore
    } else {
      // For the current bin (starting at leftCoord) we want a rightCoord
      // such that the sum of all weighted sizes are
      // globalCurrentLength/n_[compI].
      // We have to iterate to obtain this.
      label rightIndex = current.size();
      rightCoord = maxCoord;
      // Calculate rightIndex/rightCoord to have wanted size
      findBinary
        (
          sizeTol,
          sortedWeightedSizes,
          sortedCoord,
          leftIndex,
          leftCoord,
          maxCoord,
          globalCurrentSize/n_[compI],  // wanted size
          rightIndex,
          rightCoord
        );
      localSize = rightIndex - leftIndex;
    }
    if (debug) {
      Pout << "For component " << compI << ", bin " << bin
        << " copying" << endl
        << "from " << leftCoord << " at local index "
        << leftIndex << endl
        << "to " << rightCoord << " localSize:"
        << localSize << endl
        << endl;
    }
    // Copy localSize elements starting from leftIndex.
    labelList slice{localSize};
    FOR_ALL(slice, i) {
      label pointI = current[sortedCoord.indices()[leftIndex+i]];
      // Mark point into correct bin
      finalDecomp[pointI] += bin*mult;
      // And collect for next sorting action
      slice[i] = pointI;
    }
    // Sort slice in next component
    if (componentIndex < 2) {
      string oldPrefix;
      if (debug) {
        oldPrefix = Pout.prefix();
        Pout.prefix() = "  " + oldPrefix;
      }
      sortComponent
        (
          sizeTol,
          weights,
          points,
          slice,
          componentIndex+1,
          mult*n_[compI],     // Multiplier to apply to decomposition.
          finalDecomp
        );
      if (debug) {
        Pout.prefix() = oldPrefix;
      }
    }
    // Step to next bin.
    leftIndex += localSize;
    leftCoord = rightCoord;
  }
}


mousse::labelList mousse::hierarchGeomDecomp::decompose
(
  const pointField& points
)
{
  // construct a list for the final result
  labelList finalDecomp{points.size(), 0};
  // Start off with every point sorted onto itself.
  labelList slice{points.size()};
  FOR_ALL(slice, i) {
    slice[i] = i;
  }
  pointField rotatedPoints{rotDelta_ & points};
  // Calculate tolerance of cell distribution. For large cases finding
  // distibution to the cell exact would cause too many iterations so allow
  // some slack.
  label allSize = points.size();
  reduce(allSize, sumOp<label>());
  const label sizeTol = max(1, label(1e-3*allSize/nProcessors_));
  // Sort recursive
  sortComponent
    (
      sizeTol,
      rotatedPoints,
      slice,
      0,              // Sort first component in decompOrder.
      1,              // Offset for different x bins.
      finalDecomp
    );
  return finalDecomp;
}


mousse::labelList mousse::hierarchGeomDecomp::decompose
(
  const pointField& points,
  const scalarField& weights
)
{
  // construct a list for the final result
  labelList finalDecomp{points.size(), 0};
  // Start off with every point sorted onto itself.
  labelList slice{points.size()};
  FOR_ALL(slice, i) {
    slice[i] = i;
  }
  pointField rotatedPoints{rotDelta_ & points};
  // Calculate tolerance of cell distribution. For large cases finding
  // distibution to the cell exact would cause too many iterations so allow
  // some slack.
  label allSize = points.size();
  reduce(allSize, sumOp<label>());
  const label sizeTol = max(1, label(1e-3*allSize/nProcessors_));
  // Sort recursive
  sortComponent
    (
      sizeTol,
      weights,
      rotatedPoints,
      slice,
      0,              // Sort first component in decompOrder.
      1,              // Offset for different x bins.
      finalDecomp
    );
  return finalDecomp;
}


// Constructors 
mousse::hierarchGeomDecomp::hierarchGeomDecomp
(
  const dictionary& decompositionDict
)
:
  geomDecomp{decompositionDict, typeName},
  decompOrder_{}
{
  setDecompOrder();
}
