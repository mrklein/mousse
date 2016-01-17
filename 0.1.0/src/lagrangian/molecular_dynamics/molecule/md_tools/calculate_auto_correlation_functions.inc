// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

if (mesh.time().timeIndex() % vacf.sampleSteps() == 0)
{
  Field<vector> uVals{molecules.size()};
  label uV = 0;
  FOR_ALL_CONST_ITER(IDLList<molecule>, molecules, mol)
  {
    uVals[uV++] = mol().U();
  }
  vacf.calculateCorrelationFunction(uVals);
}
if (mesh.time().timeIndex() % pacf.sampleSteps() == 0)
{
  vector p = vector::zero;
  FOR_ALL_CONST_ITER(IDLList<molecule>, molecules, mol)
  {
    p.x() +=
      mol().mass() * mol().U().y() * mol().U().z()
     + 0.5*mol().rf().yz();
    p.y() +=
      mol().mass() * mol().U().z() * mol().U().x()
     + 0.5*mol().rf().zx();
    p.z() +=
      mol().mass() * mol().U().x() * mol().U().y()
     + 0.5*mol().rf().xy();
  }
  pacf.calculateCorrelationFunction(p);
}
if (mesh.time().timeIndex() % hfacf.sampleSteps() == 0)
{
  vector s = vector::zero;
  FOR_ALL_CONST_ITER(IDLList<molecule>, molecules, mol)
  {
    s +=
    (
      0.5*mol().mass()*magSqr(mol().U())
     + mol().potentialEnergy()
    )*mol().U()
   + 0.5*(mol().rf() & mol().U());
  }
  hfacf.calculateCorrelationFunction(s);
}
