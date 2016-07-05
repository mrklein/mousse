  IOporosityModelList pZones{mesh};
  Switch pressureImplicitPorosity{false};
  // nUCorrectors used for pressureImplicitPorosity
  int nUCorr = 0;
  if (pZones.active()) {
    // nUCorrectors for pressureImplicitPorosity
    nUCorr = simple.dict().lookupOrDefault<int>("nUCorrectors", 0);
    pressureImplicitPorosity = nUCorr > 0;
    Info << "Using pressure "
        << (pressureImplicitPorosity ? "implicit" : "explicit")
        << " porosity" << endl;
  }
