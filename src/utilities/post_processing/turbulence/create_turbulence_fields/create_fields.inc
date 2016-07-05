  Info << "Reading field U\n" << endl;
  volVectorField U
  {
    {
      "U",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    mesh
  };
  #include "create_phi.inc"
  singlePhaseTransportModel laminarTransport{U, phi};
  autoPtr<incompressible::RASModel> RASModel
  {
    incompressible::New<incompressible::RASModel>(U, phi, laminarTransport)
  };
