// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "complex_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "token.hpp"

namespace mousse
{
DEFINE_COMPOUND_TYPE_NAME(List<complex>, complexList);
ADD_COMPOUND_TO_RUN_TIME_SELECTION_TABLE(List<complex>, complexList);
complexField ComplexField(const UList<scalar>& re, const UList<scalar>& im)
{
  complexField cf(re.size());
  FOR_ALL(cf, i)
  {
    cf[i].Re() = re[i];
    cf[i].Im() = im[i];
  }
  return cf;
}
complexField ReComplexField(const UList<scalar>& sf)
{
  complexField cf(sf.size());
  FOR_ALL(cf, i)
  {
    cf[i].Re() = sf[i];
    cf[i].Im() = 0.0;
  }
  return cf;
}
complexField ImComplexField(const UList<scalar>& sf)
{
  complexField cf(sf.size());
  FOR_ALL(cf, i)
  {
    cf[i].Re() = 0.0;
    cf[i].Im() = sf[i];
  }
  return cf;
}
scalarField ReImSum(const UList<complex>& cf)
{
  scalarField sf(cf.size());
  FOR_ALL(sf, i)
  {
    sf[i] = cf[i].Re() + cf[i].Im();
  }
  return sf;
}
scalarField Re(const UList<complex>& cf)
{
  scalarField sf(cf.size());
  FOR_ALL(sf, i)
  {
    sf[i] = cf[i].Re();
  }
  return sf;
}
scalarField Im(const UList<complex>& cf)
{
  scalarField sf(cf.size());
  FOR_ALL(sf, i)
  {
    sf[i] = cf[i].Im();
  }
  return sf;
}

DEFINE_COMPOUND_TYPE_NAME(List<complexVector>, complexVectorList);
ADD_COMPOUND_TO_RUN_TIME_SELECTION_TABLE(List<complexVector>, complexVectorList);
complexVectorField ComplexField
(
  const UList<vector>& re,
  const UList<vector>& im
)
{
  complexVectorField cvf(re.size());
  for (direction cmpt=0; cmpt<vector::nComponents; cmpt++)
  {
    FOR_ALL(cvf, i)
    {
      cvf[i].component(cmpt).Re() = re[i].component(cmpt);
      cvf[i].component(cmpt).Im() = im[i].component(cmpt);
    }
  }
  return cvf;
}
complexVectorField ReComplexField(const UList<vector>& vf)
{
  complexVectorField cvf(vf.size());
  for (direction cmpt=0; cmpt<vector::nComponents; cmpt++)
  {
    FOR_ALL(cvf, i)
    {
      cvf[i].component(cmpt).Re() = vf[i].component(cmpt);
      cvf[i].component(cmpt).Im() = 0.0;
    }
  }
  return cvf;
}
complexVectorField ImComplexField(const UList<vector>& vf)
{
  complexVectorField cvf(vf.size());
  for (direction cmpt=0; cmpt<vector::nComponents; cmpt++)
  {
    FOR_ALL(cvf, i)
    {
      cvf[i].component(cmpt).Re() = 0.0;
      cvf[i].component(cmpt).Im() = vf[i].component(cmpt);
    }
  }
  return cvf;
}
vectorField ReImSum(const UList<complexVector>& cvf)
{
  vectorField vf(cvf.size());
  for (direction cmpt=0; cmpt<vector::nComponents; cmpt++)
  {
    FOR_ALL(cvf, i)
    {
      vf[i].component(cmpt) =
        cvf[i].component(cmpt).Re() + cvf[i].component(cmpt).Im();
    }
  }
  return vf;
}
vectorField Re(const UList<complexVector>& cvf)
{
  vectorField vf(cvf.size());
  for (direction cmpt=0; cmpt<vector::nComponents; cmpt++)
  {
    FOR_ALL(cvf, i)
    {
      vf[i].component(cmpt) = cvf[i].component(cmpt).Re();
    }
  }
  return vf;
}
vectorField Im(const UList<complexVector>& cvf)
{
  vectorField vf(cvf.size());
  for (direction cmpt=0; cmpt<vector::nComponents; cmpt++)
  {
    FOR_ALL(cvf, i)
    {
      vf[i].component(cmpt) = cvf[i].component(cmpt).Im();
    }
  }
  return vf;
}
complexVectorField operator^
(
  const UList<vector>& vf,
  const UList<complexVector>& cvf
)
{
  return ComplexField(vf^Re(cvf), vf^Im(cvf));
}
}  // namespace mousse
