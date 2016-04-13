bool LTS = fv::localEulerDdt::enabled(mesh);
tmp<volScalarField> trDeltaT;
if (LTS) {
  Info << "Using LTS" << endl;
  trDeltaT = tmp<volScalarField>
  {
    new volScalarField
    {
      {
        fv::localEulerDdt::rDeltaTName,
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
      },
      mesh,
      {"one", dimless/dimTime, 1},
      zeroGradientFvPatchScalarField::typeName
    }
  };
}
