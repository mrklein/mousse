// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "noise_fft.hpp"
#include "arg_list.hpp"
#include "time.hpp"
#include "function_object_file.hpp"
#include "csv.hpp"

using namespace mousse;

mousse::scalar checkUniformTimeStep(const scalarField& t)
{
  // check that a uniform time step has been applied
  scalar deltaT = -1.0;
  if (t.size() > 1)
  {
    for (label i = 1; i < t.size(); i++)
    {
      scalar dT = t[i] - t[i-1];
      if (deltaT < 0)
      {
        deltaT = dT;
      }
      if (mag(deltaT - dT) > SMALL)
      {
        FATAL_ERROR_IN("checkUniformTimeStep(const scalarField&)")
          << "Unable to process data with a variable time step"
          << exit(FatalError);
      }
    }
  }
  else
  {
    FATAL_ERROR_IN("checkUniformTimeStep(const scalarField&)")
      << "Unable to create FFT with a single value"
      << exit(FatalError);
  }
  return deltaT;
}

int main(int argc, char *argv[])
{
  argList::noParallel();
  #include "add_dict_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_fields.inc"
  Info << "Reading data file" << endl;
  CSV<scalar> pData{"pressure", dict, "Data"};
  // time history data
  const scalarField t{pData.x()};
  // pressure data
  const scalarField p{pData.y()};
  if (t.size() < N)
  {
    FATAL_ERROR_IN(args.executable())
      << "Block size N = " << N
      << " is larger than number of data = " << t.size()
      << exit(FatalError);
  }
  Info << "    read " << t.size() << " values" << nl << endl;
  Info << "Creating noise FFT" << endl;
  noiseFFT nfft{checkUniformTimeStep(t), p};
  nfft -= pRef;
  fileName baseFileName{pData.fName().lessExt()};
  graph Pf{nfft.RMSmeanPf(N, min(nfft.size()/N, nw))};
  Info << "    Creating graph for " << Pf.title() << endl;
  Pf.write(baseFileName + graph::wordify(Pf.title()), graphFormat);
  graph Lf{nfft.Lf(Pf)};
  Info << "    Creating graph for " << Lf.title() << endl;
  Lf.write(baseFileName + graph::wordify(Lf.title()), graphFormat);
  graph Ldelta{nfft.Ldelta(Lf, f1, fU)};
  Info << "    Creating graph for " << Ldelta.title() << endl;
  Ldelta.write(baseFileName + graph::wordify(Ldelta.title()), graphFormat);
  graph Pdelta{nfft.Pdelta(Pf, f1, fU)};
  Info << "    Creating graph for " << Pdelta.title() << endl;
  Pdelta.write(baseFileName + graph::wordify(Pdelta.title()), graphFormat);
  Info << nl << "End\n" << endl;
  return 0;
}
