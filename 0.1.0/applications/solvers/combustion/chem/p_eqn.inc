{
  rho = thermo.rho();
  if (constProp == "volume")
  {
    scalar invW = 0.0;
    FOR_ALL(Y, i)
    {
      invW += Y[i][0]/specieData[i].W();
    }
    Rspecific[0] = 1000.0*constant::physicoChemical::R.value()*invW;
    p[0] = rho0*Rspecific[0]*thermo.T()[0];
    rho[0] = rho0;
  }
}
