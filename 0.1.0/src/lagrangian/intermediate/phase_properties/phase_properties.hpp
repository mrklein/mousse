#ifndef LAGRANGIAN_INTERMEDIATE_PHASE_PROPERTIES_PHASE_PROPERTIES_HPP_
#define LAGRANGIAN_INTERMEDIATE_PHASE_PROPERTIES_PHASE_PROPERTIES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phaseProperties
// Description
//   Helper class to manage multi-specie phase properties
// SourceFiles
//   phase_properties.cpp
//   phase_properties_io.cpp
#include "named_enum.hpp"
#include "tuple2.hpp"
#include "ptr_list.hpp"
#include "vol_fields.hpp"
namespace mousse
{
class phaseProperties
{
public:
  // Public data
    //- Phase type enumeration
    enum phaseType
    {
      GAS,
      LIQUID,
      SOLID,
      UNKNOWN
    };
    //- Corresponding word representations for phase type enumerations
    static const NamedEnum<phaseType, 4> phaseTypeNames;
private:
 // Private data
    //- Phase type
    phaseType phase_;
    //- State label (s), (l), (g) etc.
    word stateLabel_;
    //- List of specie names
    List<word> names_;
    //- List of specie mass fractions
    scalarField Y_;
    //- Map to carrier id
    labelList carrierIds_;
  // Private Member Functions
    //- Reorder species to be consistent with the given specie name list
    void reorder(const wordList& specieNames);
    //- Set carrier ids
    void setCarrierIds(const wordList& carrierNames);
    //- Check the total mass fraction
    void checkTotalMassFraction() const;
    //- Set the state label
    word phaseToStateLabel(const phaseType pt) const;
public:
  // Constructors
    //- Null constructor
    phaseProperties();
    //- Construct from Istream
    phaseProperties(Istream&);
    //- Construct as copy
    phaseProperties(const phaseProperties&);
  //- Destructor
  ~phaseProperties();
  // Public Member Functions
    //- Reorder species to be consistent with the corresponding
    //  phase specie name list
    void reorder
    (
      const wordList& gasNames,
      const wordList& liquidNames,
      const wordList& solidNames
    );
    // Access
      //- Return const access to the phase type
      phaseType phase() const;
      //- Return const access to the phase state label
      const word& stateLabel() const;
      //- Return word representation of the phase type
      word phaseTypeName() const;
      //- Return the list of specie names
      const List<word>& names() const;
      //- Return const access to a specie name
      const word& name(const label speciei) const;
      //- Return const access to all specie mass fractions
      const scalarField& Y() const;
      //- Return non-const access to a specie mass fraction
      scalar& Y(const label speciei);
      //- Return const access to the map to the carrier ids
      const labelList& carrierIds() const;
      //- Return the id of a specie in the local list by name
      //  Returns -1 if not found
      label id(const word& specieName) const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, phaseProperties&);
    friend Ostream& operator<<(Ostream&, const phaseProperties&);
};
}  // namespace mousse
#endif
