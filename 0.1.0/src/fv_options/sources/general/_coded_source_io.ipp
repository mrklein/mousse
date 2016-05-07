// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_coded_source.hpp"
#include "string_ops.hpp"


// Member Functions 
template<class Type>
bool mousse::fv::CodedSource<Type>::read(const dictionary& dict)
{
  if (cellSetOption::read(dict)) {
    coeffs_.lookup("fieldNames") >> fieldNames_;
    applied_.setSize(fieldNames_.size(), false);
    coeffs_.lookup("redirectType") >> redirectType_;
    // Code snippets
    {
      const entry& e =
        coeffs_.lookupEntry
        (
          "codeCorrect",
          false,
          false
        );
      codeCorrect_ = stringOps::trim(e.stream());
      stringOps::inplaceExpand(codeCorrect_, coeffs_);
      dynamicCodeContext::addLineDirective
      (
        codeCorrect_,
        e.startLineNumber(),
        coeffs_.name()
      );
    }
    {
      const entry& e =
        coeffs_.lookupEntry
        (
          "codeAddSup",
          false,
          false
        );
      codeAddSup_ = stringOps::trim(e.stream());
      stringOps::inplaceExpand(codeAddSup_, coeffs_);
      dynamicCodeContext::addLineDirective
        (
          codeAddSup_,
          e.startLineNumber(),
          coeffs_.name()
        );
    }
    {
      const entry& e =
        coeffs_.lookupEntry
        (
          "codeSetValue",
          false,
          false
        );
      codeSetValue_ = stringOps::trim(e.stream());
      stringOps::inplaceExpand(codeSetValue_, coeffs_);
      dynamicCodeContext::addLineDirective
        (
          codeSetValue_,
          e.startLineNumber(),
          coeffs_.name()
        );
    }
    return true;
  }
  return false;
}

