// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::SemiImplicitSource
// Description
//   Semi-implicit source, described using an input dictionary.  The injection
//   rate coefficients are specified as pairs of Su-Sp coefficients, i.e.
//     \f[
//       S(x) = S_u + S_p x
//     \f]
//   where
//   \vartable
//     S(x)    | net source for field 'x'
//     S_u     | explicit source contribution
//     S_p     | linearised implicit contribution
//   \endvartable
//   Example of the source specification:
//   \verbatim
//   <Type>SemiImplicitSourceCoeffs
//   {
//     volumeMode      absolute; // specific
//     injectionRateSuSp
//     {
//       k           (30.7 0);
//       epsilon     (1.5  0);
//     }
//   }
//   \endverbatim
//   Valid options for the \c volumeMode entry include:
//   - absolute: values are given as \<quantity\>
//   - specific: values are given as \<quantity\>/m3
// SeeAlso
//   mousse::fvOption
// SourceFiles
//   _semi_implicit_source.cpp
#ifndef _semi_implicit_source_hpp_
#define _semi_implicit_source_hpp_
#include "tuple2.hpp"
#include "cell_set_option.hpp"
namespace mousse
{
namespace fv
{
// Forward declaration of classes
template<class Type>
class SemiImplicitSource;
// Forward declaration of friend functions
template<class Type>
Ostream& operator<<
(
  Ostream&,
  const SemiImplicitSource<Type>&
);
template<class Type>
class SemiImplicitSource
:
  public cellSetOption
{
public:
  // Public data
    //- Enumeration for volume types
    enum volumeModeType
    {
      vmAbsolute,
      vmSpecific
    };
    //- Word list of volume mode type names
    static const wordList volumeModeTypeNames_;
protected:
  // Protected data
    //- Volume mode
    volumeModeType volumeMode_;
    //- Volume normalisation
    scalar VDash_;
    //- Source field values
    List<Tuple2<Type, scalar> > injectionRate_;
  // Protected functions
    //- Helper function to convert from a word to a volumeModeType
    volumeModeType wordToVolumeModeType(const word& vtName) const;
    //- Helper function to convert from a volumeModeType to a word
    word volumeModeTypeToWord(const volumeModeType& vtType) const;
    //- Set the local field data
    void setFieldData(const dictionary& dict);
public:
  //- Runtime type information
  TypeName("SemiImplicitSource");
  // Constructors
    //- Construct from components
    SemiImplicitSource
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  // Member Functions
    // Access
      //- Return const access to the volume mode
      inline const volumeModeType& volumeMode() const;
      //- Return const access to the source field values
      inline const List<Tuple2<Type, scalar> >& injectionRate() const;
    // Edit
      //- Return access to the volume mode
      inline volumeModeType& volumeMode();
      //- Return access to the source field values
      inline List<Tuple2<Type, scalar> >& injectionRate();
    // Evaluation
      //- Add explicit contribution to equation
      virtual void addSup
      (
        fvMatrix<Type>& eqn,
        const label fieldI
      );
      //- Add explicit contribution to compressible equation
      virtual void addSup
      (
        const volScalarField& rho,
        fvMatrix<Type>& eqn,
        const label fieldI
      );
    // IO
      //- Read source dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
#   include "_semi_implicit_source.cpp"
#   include "_semi_implicit_source_io.cpp"
#endif
#include "_semi_implicit_source_i.hpp"
#endif
