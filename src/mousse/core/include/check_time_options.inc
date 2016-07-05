// check_time_options.inc
mousse::label startTime = 0;
// Unless -constant is present, skip startTime if it is "constant"
#include "check_constant_option.inc"
// Check -time and -latestTime options
#include "check_time_option.inc"
