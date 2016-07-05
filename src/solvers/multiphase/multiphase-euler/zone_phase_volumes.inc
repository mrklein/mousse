{
  const scalarField& V = mesh.V();
  FOR_ALL(mesh.cellZones(), czi)
  {
    const labelList& cellLabels = mesh.cellZones()[czi];
    FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, fluid.phases(), iter)
    {
      const volScalarField& alpha = iter();
      scalar phaseVolume = 0;
      FOR_ALL(cellLabels, cli)
      {
        label celli = cellLabels[cli];
        phaseVolume += alpha[celli]*V[celli];
      }
      reduce(phaseVolume, sumOp<scalar>());
      Info << alpha.name()
        << " phase volume in zone " << mesh.cellZones()[czi].name()
        << " = " << phaseVolume*1e6 << " ml " << endl;
    }
  }
}
