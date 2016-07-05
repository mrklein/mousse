#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_CHEMISTRY_READERS_CHEMKIN_READER_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_CHEMISTRY_READERS_CHEMKIN_READER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::chemkinReader
// Description
//   mousse::chemkinReader

#include "chemistry_reader.hpp"
#include "file_name.hpp"
#include "type_info.hpp"
#include "switch.hpp"
#include "hash_ptr_table.hpp"
#include "reaction_list.hpp"
#include "dynamic_list.hpp"
#include "label_list.hpp"
#include "species_table.hpp"
#include "atomic_weights.hpp"
#include "reaction_types.hpp"
#include <FlexLexer.h>


namespace mousse {

class chemkinReader
:
  public chemistryReader<gasHThermoPhysics>,
  public yyFlexLexer
{
public:
  // Public data types
    enum phase
    {
      solid,
      liquid,
      gas
    };
    //- species element
    struct specieElement
    {
      word elementName;
      label nAtoms;
      bool operator==(const specieElement& se) const
      {
        return nAtoms == se.nAtoms && elementName == se.elementName;
      }
      bool operator!=(const specieElement& se) const
      {
        return !operator==(se);
      }
      friend Ostream& operator<<(Ostream& os, const specieElement& se)
      {
        os << se.nAtoms << token::SPACE << se.elementName;
        return os;
      }
    };
private:
  // Private data
    static int yyBufSize;
    label lineNo_;
    //- Table of reaction type keywords
    HashTable<int> reactionKeywordTable_;
    //- Currently supported reaction types
    enum reactionKeyword
    {
      thirdBodyReactionType,
      unimolecularFallOffReactionType,
      chemicallyActivatedBimolecularReactionType,
      TroeReactionType,
      SRIReactionType,
      LandauTellerReactionType,
      reverseLandauTellerReactionType,
      JanevReactionType,
      powerSeriesReactionRateType,
      radiationActivatedReactionType,
      speciesTempReactionType,
      energyLossReactionType,
      plasmaMomentumTransfer,
      collisionCrossSection,
      nonEquilibriumReversibleReactionType,
      duplicateReactionType,
      speciesOrderForward,
      speciesOrderReverse,
      UnitsOfReaction,
      end
    };
    enum reactionType
    {
      irreversible,
      reversible,
      nonEquilibriumReversible,
      unknownReactionType
    };
    static const char* reactionTypeNames[4];
    enum reactionRateType
    {
      Arrhenius,
      thirdBodyArrhenius,
      unimolecularFallOff,
      chemicallyActivatedBimolecular,
      LandauTeller,
      Janev,
      powerSeries,
      unknownReactionRateType
    };
    static const char* reactionRateTypeNames[8];
    enum fallOffFunctionType
    {
      Lindemann,
      Troe,
      SRI,
      unknownFallOffFunctionType
    };
    static const char* fallOffFunctionNames[4];
    void initReactionKeywordTable();
    //- List of elements
    DynamicList<word> elementNames_;
    //- Element indices
    HashTable<label> elementIndices_;
    //- Isotope molecular weights
    HashTable<scalar> isotopeAtomicWts_;
    //- List of species
    DynamicList<word> specieNames_;
    //- Specie indices
    HashTable<label> specieIndices_;
    //- Table of species
    speciesTable& speciesTable_;
    //- Specie phase
    HashTable<phase> speciePhase_;
    //- Table of the thermodynamic data given in the CHEMKIN file
    HashPtrTable<gasHThermoPhysics> speciesThermo_;
    //- Table of species composition
    HashTable<List<specieElement> > specieComposition_;
    //- List of the reactions
    ReactionList<gasHThermoPhysics> reactions_;
    //- Flag to indicate that file is in new format
    Switch newFormat_;
    //- Tolerance for element imbalance in a reaction
    scalar imbalanceTol_;
  // Private Member Functions
    //- Flex lexer to read the CHEMKIN III file
    int lex();
    inline scalar stringToScalar(const string& s)
    {
      string& str = const_cast<string&>(s);
      str.replaceAll(" ", "");
      str.replaceAll("D", "e");
      str.replaceAll("d", "e");
      return atof(str.c_str());
    }
    inline scalar stringToScalar(const char* cstr)
    {
      return stringToScalar(string(cstr));
    }
    inline void correctElementName(word& elementName)
    {
      if (elementName.size() == 2) {
        elementName[1] = tolower(elementName[1]);
      } else if (elementName[0] == 'E') {
        elementName = 'e';
      }
    }
    scalar molecularWeight
    (
      const List<specieElement>& specieComposition
    ) const;
    void finishElements(labelList& currentAtoms);
    void checkCoeffs
    (
      const scalarList& reactionCoeffs,
      const char* reationRateName,
      const label nCoeffs
    ) const;
    template<class ReactionRateType>
    void addReactionType
    (
      const reactionType rType,
      DynamicList<gasHReaction::specieCoeffs>& lhs,
      DynamicList<gasHReaction::specieCoeffs>& rhs,
      const ReactionRateType& rr
    );
    template<template<class, class> class PressureDependencyType>
    void addPressureDependentReaction
    (
      const reactionType rType,
      const fallOffFunctionType fofType,
      DynamicList<gasHReaction::specieCoeffs>& lhs,
      DynamicList<gasHReaction::specieCoeffs>& rhs,
      const scalarList& thirdBodyEfficiencies,
      const scalarList& k0Coeffs,
      const scalarList& kInfCoeffs,
      const HashTable<scalarList>& reactionCoeffsTable,
      const scalar Afactor0,
      const scalar AfactorInf,
      const scalar RR
    );
    void addReaction
    (
      DynamicList<gasHReaction::specieCoeffs>& lhs,
      DynamicList<gasHReaction::specieCoeffs>& rhs,
      const scalarList& thirdBodyEfficiencies,
      const reactionType rType,
      const reactionRateType rrType,
      const fallOffFunctionType fofType,
      const scalarList& ArrheniusReactionCoeffs,
      HashTable<scalarList>& reactionCoeffsTable,
      const scalar RR
    );
    // Read the CHEMKIN files
    void read
    (
      const fileName& CHEMKINFileName,
      const fileName& thermoFileName
    );
public:
  //- Runtime type information
  TYPE_NAME("chemkinReader");
  // Constructors
    //- Construct from CHEMKIN III file name
    chemkinReader
    (
      const fileName& chemkinFile,
      speciesTable& species,
      const fileName& thermoFileName = fileName::null,
      const bool newFormat = false
    );
    //- Construct by getting the CHEMKIN III file name from dictionary
    chemkinReader(const dictionary& thermoDict, speciesTable& species);
    //- Disallow default bitwise copy construct
    chemkinReader(const chemkinReader&) = delete;
    //- Disallow default bitwise assignment
    chemkinReader& operator=(const chemkinReader&) = delete;
  //- Destructor
  virtual ~chemkinReader()
  {}
  // Member functions
    //- List of elements
    const wordList& elementNames() const { return elementNames_; }
    //- Element indices
    const HashTable<label>& elementIndices() const { return elementIndices_; }
    //- Isotope molecular weights
    const HashTable<scalar>& isotopeAtomicWts() const
    {
      return isotopeAtomicWts_;
    }
    //- Table of species
    const speciesTable& species() const { return speciesTable_; }
    //- Specie phase
    const HashTable<phase>& speciePhase() const { return speciePhase_; }
    //- Table of the thermodynamic data given in the CHEMKIN file
    const HashPtrTable<gasHThermoPhysics>& speciesThermo() const
    {
      return speciesThermo_;
    }
    //- Table of species composition
    const HashTable<List<specieElement> >& specieComposition() const
    {
      return specieComposition_;
    }
    //- List of the reactions
    const ReactionList<gasHThermoPhysics>& reactions() const
    {
      return reactions_;
    }
};

}  // namespace mousse

#endif

