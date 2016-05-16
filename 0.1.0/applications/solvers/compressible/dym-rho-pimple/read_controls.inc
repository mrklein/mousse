#include "read_time_controls.inc"
correctPhi = pimple.dict().lookupOrDefault<Switch>("correctPhi", true);
checkMeshCourantNo =
  pimple.dict().lookupOrDefault<Switch>("checkMeshCourantNo", false);

