correctUphiBCs(U, phi);

{
  volScalarField pcorr
  {
    {
      "pcorr",
      runTime.timeName(),
      mesh
    },
    mesh,
    {"pcorr", p.dimensions(), 0.0},
    pcorrTypes
  };
  surfaceScalarField rhof{fvc::interpolate(rho, "div(phi,rho)")};
  dimensionedScalar rAUf{"rAUf", dimTime, 1.0};
  mesh.setFluxRequired(pcorr.name());
  while (pimple.correctNonOrthogonal()) {
    fvScalarMatrix pcorrEqn
    {
      fvm::laplacian(rAUf, pcorr) == fvc::ddt(rho) + fvc::div(phi*rhof)
    };
    pcorrEqn.solve();
    if (pimple.finalNonOrthogonalIter()) {
      phi -= pcorrEqn.flux()/rhof;
    }
  }
}
