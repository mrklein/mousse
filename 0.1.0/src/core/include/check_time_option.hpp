//
// checkTimeOption.H
// ~~~~~~~~~~~~~~~~~
// Check -time and -latestTime options
if (args.optionFound("time"))
{
  mousse::scalar timeValue = args.optionRead<scalar>("time");
  startTime = mousse::Time::findClosestTimeIndex(Times, timeValue);
}
if (args.optionFound("latestTime"))
{
  startTime = Times.size() - 1;
}
