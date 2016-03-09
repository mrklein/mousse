// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "distribution_model.hpp"
#include "make_graph.hpp"
#include "ofstream.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_fields.inc"
  label iCheck = 100;
  for (label i=1; i<=nSamples; i++)
  {
    scalar ps = p->sample();
    label n = static_cast<label>((ps - xMin)*nIntervals/(xMax - xMin));
    samples[n]++;
    if (writeData)
    {
      filePtr() << ps << nl;
    }
    if (i % iCheck == 0)
    {
      Info << "    processed " << i << " samples" << endl;
      if (i == 10*iCheck)
      {
        iCheck *= 10;
      }
    }
  }
  scalarField x{nIntervals};
  FOR_ALL(x, i)
  {
    x[i] = xMin + i*(xMax - xMin)/(nIntervals - 1);
  }
  makeGraph(x, samples, p->type(), pdfPath, runTime.graphFormat());
  Info << "End\n" << endl;
  return 0;
}
