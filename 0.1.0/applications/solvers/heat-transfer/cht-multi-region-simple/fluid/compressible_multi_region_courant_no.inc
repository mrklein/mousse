  scalar CoNum = -GREAT;
  FOR_ALL(fluidRegions, regionI)
  {
    CoNum = max
    (
      compressibleCourantNo
      (
        fluidRegions[regionI],
        runTime,
        rhoFluid[regionI],
        phiFluid[regionI]
      ),
      CoNum
    );
  }
