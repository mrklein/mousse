#include "create_time_controls.inc"
bool correctPhi
{
  pimple.dict().lookupOrDefault<Switch>("correctPhi", true)
};
bool checkMeshCourantNo
{
  pimple.dict().lookupOrDefault<Switch>("checkMeshCourantNo", false)
};
