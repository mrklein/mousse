// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

Info << nl << "Creating autocorrelation functions." << endl;
IOdictionary mdTransportProperitesDict
(
  IOobject
  (
    "mdTransportProperitesDict",
    mesh.time().system(),
    mesh,
    IOobject::MUST_READ_IF_MODIFIED,
    IOobject::NO_WRITE,
    false
  )
);
const dictionary& autocorrelationFunctionDict
(
  mdTransportProperitesDict.subDict("autocorrelationFunctions")
);
//- Velocity autocorrelation function
Info << tab << "velocty" << endl;
const dictionary& velocityACFDict
(
  autocorrelationFunctionDict.subDict("velocity")
);
correlationFunction<vector> vacf
(
  mesh,
  velocityACFDict,
  molecules.size()
);
bool writeVacf(Switch(velocityACFDict.lookup("writeFile")));
//- Pressure autocorrelation function
Info << tab << "pressure" << endl;
const dictionary& pressureACFDict
(
  autocorrelationFunctionDict.subDict("pressure")
);
correlationFunction<vector> pacf
(
  mesh,
  pressureACFDict,
  1
);
bool writePacf(Switch(pressureACFDict.lookup("writeFile")));
//- Heat flux autocorrelation function
Info << tab << "heat flux" << endl;
const dictionary& heatFluxACFDict
(
  autocorrelationFunctionDict.subDict("heatFlux")
);
correlationFunction<vector> hfacf
(
  mesh,
  heatFluxACFDict,
  1
);
bool writeHFacf(Switch(heatFluxACFDict.lookup("writeFile")));
