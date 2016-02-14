// write base thermo fields - not registered since will be re-read by
// thermo package
Info<< "Creating base fields for time " << runTime.timeName() << endl;

{
  volScalarField Ydefault
  {
    {
      "Ydefault",
      runTime.timeName(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE,
      false
    },
    mesh,
    {"Ydefault", dimless, 1}
  };
  Ydefault.write();
  volScalarField p
  {
    {
      "p",
      runTime.timeName(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE,
      false
    },
    mesh,
    {"p", dimPressure, p0}
  };
  p.write();
  volScalarField T
  {
    {
      "T",
      runTime.timeName(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE,
      false
    },
    mesh,
    {"T", dimTemperature, T0}
  };
  T.write();
}
