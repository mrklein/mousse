//
// addTimeOptions.H
// ~~~~~~~~~~~~~~~~
  mousse::argList::addBoolOption
  (
    "constant",
    "include the 'constant/' dir in the times list"
  );
  mousse::argList::addBoolOption
  (
    "latestTime",
    "select the latest time"
  );
  mousse::argList::addBoolOption
  (
    "noZero",
    "exclude the '0/' dir from the times list"
  );
  mousse::argList::addOption
  (
    "time",
    "time",
    "specify a single time value to select"
  );
