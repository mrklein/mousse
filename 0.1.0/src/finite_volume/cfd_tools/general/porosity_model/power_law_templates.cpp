// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
template<class RhoFieldType>
void mousse::porosityModels::powerLaw::apply
(
  scalarField& Udiag,
  const scalarField& V,
  const RhoFieldType& rho,
  const vectorField& U
) const
{
  const scalar C0 = C0_;
  const scalar C1m1b2 = (C1_ - 1.0)/2.0;
  FOR_ALL(cellZoneIDs_, zoneI)
  {
    const labelList& cells = mesh_.cellZones()[cellZoneIDs_[zoneI]];
    FOR_ALL(cells, i)
    {
      const label cellI = cells[i];
      Udiag[cellI] +=
        V[cellI]*rho[cellI]*C0*pow(magSqr(U[cellI]), C1m1b2);
    }
  }
}
template<class RhoFieldType>
void mousse::porosityModels::powerLaw::apply
(
  tensorField& AU,
  const RhoFieldType& rho,
  const vectorField& U
) const
{
  const scalar C0 = C0_;
  const scalar C1m1b2 = (C1_ - 1.0)/2.0;
  FOR_ALL(cellZoneIDs_, zoneI)
  {
    const labelList& cells = mesh_.cellZones()[cellZoneIDs_[zoneI]];
    FOR_ALL(cells, i)
    {
      const label cellI = cells[i];
      AU[cellI] =
        AU[cellI] + I*(rho[cellI]*C0*pow(magSqr(U[cellI]), C1m1b2));
    }
  }
}
