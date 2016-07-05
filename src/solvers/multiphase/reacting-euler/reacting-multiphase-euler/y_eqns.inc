{
  autoPtr<phaseSystem::massTransferTable> massTransferPtr{fluid.massTransfer()};
  phaseSystem::massTransferTable& massTransfer = massTransferPtr();
  FOR_ALL(phases, phasei)
  {
    phaseModel& phase = phases[phasei];
    PtrList<volScalarField>& Y = phase.Y();
    const volScalarField& alpha = phase;
    const volScalarField& rho = phase.rho();
    FOR_ALL(Y, i)
    {
      tmp<fvScalarMatrix> YiEqn{phase.YiEqn(Y[i])};
      if (YiEqn.valid())
      {
        YiEqn =
        (
          YiEqn
          ==
          *massTransfer[Y[i].name()]
          + fvOptions(alpha, rho, Y[i])
        );
        YiEqn->relax();
        YiEqn->solve(mesh.solver("Yi"));
      }
    }
  }
  fluid.massTransfer(); // updates interfacial mass flow rates
}
