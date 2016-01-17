// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
template<class RhoFieldType>
void mousse::porosityModels::DarcyForchheimer::apply
(
  scalarField& Udiag,
  vectorField& Usource,
  const scalarField& V,
  const RhoFieldType& rho,
  const scalarField& mu,
  const vectorField& U
) const
{
  FOR_ALL(cellZoneIDs_, zoneI)
  {
    const tensorField& dZones = D_[zoneI];
    const tensorField& fZones = F_[zoneI];
    const labelList& cells = mesh_.cellZones()[cellZoneIDs_[zoneI]];
    FOR_ALL(cells, i)
    {
      const label cellI = cells[i];
      const label j = this->fieldIndex(i);
      const tensor Cd =
        mu[cellI]*dZones[j] + (rho[cellI]*mag(U[cellI]))*fZones[j];
      const scalar isoCd = tr(Cd);
      Udiag[cellI] += V[cellI]*isoCd;
      Usource[cellI] -= V[cellI]*((Cd - I*isoCd) & U[cellI]);
    }
  }
}
template<class RhoFieldType>
void mousse::porosityModels::DarcyForchheimer::apply
(
  tensorField& AU,
  const RhoFieldType& rho,
  const scalarField& mu,
  const vectorField& U
) const
{
  FOR_ALL(cellZoneIDs_, zoneI)
  {
    const tensorField& dZones = D_[zoneI];
    const tensorField& fZones = F_[zoneI];
    const labelList& cells = mesh_.cellZones()[cellZoneIDs_[zoneI]];
    FOR_ALL(cells, i)
    {
      const label cellI = cells[i];
      const label j = this->fieldIndex(i);
      const tensor D = dZones[j];
      const tensor F = fZones[j];
      AU[cellI] += mu[cellI]*D + (rho[cellI]*mag(U[cellI]))*F;
    }
  }
}
