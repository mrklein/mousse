// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noiseFFT
// Description
//   FFT of the pressure field
// SourceFiles
//   noise_fft.cpp
#ifndef noise_fft_hpp_
#define noise_fft_hpp_
#include "scalar_field.hpp"
#include "graph.hpp"
namespace mousse
{
class noiseFFT
:
  public scalarField
{
  // Private data
    //- Time spacing of the raw data
    scalar deltat_;
public:
  //- Reference pressure
  static scalar p0;
  // Constructors
    //- Construct from pressure field
    noiseFFT
    (
      const scalar deltat,
      const scalarField& pressure
    );
    //- Construct from Istream
    noiseFFT(Istream&);
    //- Construct from pressure field file name
    noiseFFT(const fileName& pFileName, const label skip = 0);
  // Member Functions
    //- Return the graph of p(t)
    graph pt() const;
    //- Return the nth window
    tmp<scalarField> window(const label N, const label n) const;
    //- Return the Hanning window function
    tmp<scalarField> Hanning(const label N) const;
    //- Return the fft of the given pressure data
    tmp<scalarField> Pf(const tmp<scalarField>& pn) const;
    //- Return the multi-window mean fft of the complete pressure data
    graph meanPf(const label N, const label nw) const;
    //- Return the multi-window RMS mean fft of the complete pressure data
    graph RMSmeanPf(const label N, const label nw) const;
    //- Return the narrow-band PFL (pressure-fluctuation level) spectrum
    graph Lf(const graph& gPf) const;
    //- Return the one-third-octave-band PFL spectrum
    //  starting at octave with mean frequency f1
    graph Ldelta(const graph& gLf, const scalar f1, const scalar fU) const;
    //- Return the one-third-octave-band pressure spectrum
    //  starting at octave with mean frequency f1
    graph Pdelta(const graph& gLf, const scalar f1, const scalar fU) const;
    //- Return the total PFL as the sum of Lf over all frequencies
    scalar Lsum(const graph& gLf) const;
    //- Convert the db into Pa
    scalar dbToPa(const scalar db) const;
    //- Convert the db-field into Pa
    tmp<scalarField> dbToPa(const tmp<scalarField>& db) const;
};
}  // namespace mousse
#endif
