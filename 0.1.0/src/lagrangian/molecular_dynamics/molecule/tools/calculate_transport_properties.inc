// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

if (writeVacf) {
  OFstream vacfFile{runTime.path()/"vacf"};
  if (!vacf.writeAveraged(vacfFile)) {
    FATAL_ERROR_IN(args.executable())
      << "Failed writing to "
      << vacfFile.name()
      << abort(FatalError);
  }
}
Info << "Diffusion coefficient = " << vacf.integral() << endl;
if (writePacf) {
  OFstream pacfFile{runTime.path()/"pacf"};
  if (!pacf.writeAveraged(pacfFile)) {
    FATAL_ERROR_IN(args.executable())
      << "Failed writing to "
      << pacfFile.name()
      << abort(FatalError);
  }
}
Info << "Viscosity = "
  << pacf.integral()/averageTemperature/moleculeCloud::kb/meshVolume
  << endl;
if (writeHFacf) {
  OFstream hfacfFile{runTime.path()/ + "hfacf"};
  if (!hfacf.writeAveraged(hfacfFile)) {
    FATAL_ERROR_IN(args.executable())
      << "Failed writing to "
      << hfacfFile.name()
      << abort(FatalError);
  }
}
{
  const scalar T2kV =
    averageTemperature*averageTemperature*moleculeCloud::kb*meshVolume;
  Info << "Thermal conductivity = " << hfacf.integral()/T2kV << endl;
}

