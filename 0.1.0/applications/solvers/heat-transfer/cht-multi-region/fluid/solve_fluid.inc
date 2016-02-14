if (finalIter)
{
  mesh.data::add("finalIteration", true);
}
if (frozenFlow)
{
  #include "e_eqn.inc"
}
else
{
  if (oCorr == 0)
  {
    #include "rho_eqn.inc"
  }
  #include "u_eqn.inc"
  #include "e_eqn.inc"
  // --- PISO loop
  for (int corr=0; corr<nCorr; corr++)
  {
    #include "p_eqn.inc"
  }
  turb.correct();
  rho = thermo.rho();
}
if (finalIter)
{
  mesh.data::remove("finalIteration");
}
