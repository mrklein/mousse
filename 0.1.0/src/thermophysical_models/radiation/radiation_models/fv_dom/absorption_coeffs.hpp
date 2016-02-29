#ifndef THERMOPHYSICAL_MODELS_RADIATION_RADIATION_MODELS_FV_DOM_ABSORPTION_COEFFS_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_RADIATION_MODELS_FV_DOM_ABSORPTION_COEFFS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::absorptionCoeffs
// Description
//   Absorption coefficients class used in greyMeanAbsorptionEmission and
//   wideBandAbsorptionEmission
// SourceFiles
//   absorption_coeffs.cpp
#include "list.hpp"
#include "iostreams.hpp"
#include "iodictionary.hpp"
namespace mousse
{
namespace radiation
{
class absorptionCoeffs
{
public:
  // Public data members
    static const int nCoeffs_ = 6;
    typedef FixedList<scalar, nCoeffs_> coeffArray;
private:
  // Private data
    // Temperature limits of applicability for functions
      scalar Tcommon_;
      scalar Tlow_;
      scalar Thigh_;
    // Polynomial using inverse temperatures
    bool invTemp_;
    coeffArray highACoeffs_;
    coeffArray lowACoeffs_;
  // Private Member Functions
    //- Check given temperature is within the range of the fitted coeffs
    void checkT(const scalar T) const;
public:
  // Constructors
    //- Construct from Istream
    absorptionCoeffs(Istream&);
    // Null constructor
    absorptionCoeffs()
    {}
  //- Destructor
  ~absorptionCoeffs();
  // Member functions
    //- Return the coefficients corresponding to the given temperature
    const coeffArray& coeffs(const scalar T) const;
    // Initialise from a dictionary
    void initialise(const dictionary&);
    // Initialise from an Istream
    void initialise(Istream&);
  // Access Functions
    inline bool invTemp() const;
    inline scalar Tcommon() const;
    inline scalar Tlow() const;
    inline scalar Thigh() const;
    inline const coeffArray& highACoeffs() const;
    inline const coeffArray& lowACoeffs() const;
};
}  // namespace mousse
}  // namespace radiation

inline bool mousse::radiation::absorptionCoeffs::invTemp() const
{
  return  invTemp_;
}
inline mousse::scalar mousse::radiation::absorptionCoeffs::Tcommon() const
{
  return  Tcommon_;
}
inline mousse::scalar mousse::radiation::absorptionCoeffs::Tlow() const
{
  return  Tlow_;
}
inline mousse::scalar mousse::radiation::absorptionCoeffs::Thigh() const
{
  return  Thigh_;
}
inline const mousse::radiation::absorptionCoeffs::coeffArray&
mousse::radiation::absorptionCoeffs::highACoeffs() const
{
  return  highACoeffs_;
}
inline const mousse::radiation::absorptionCoeffs::coeffArray&
mousse::radiation::absorptionCoeffs::lowACoeffs() const
{
  return  lowACoeffs_;
}
#endif
