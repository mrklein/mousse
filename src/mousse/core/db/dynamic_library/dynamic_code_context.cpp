// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_code_context.hpp"
#include "string_ops.hpp"
#include "osha1stream.hpp"


// Constructors 
mousse::dynamicCodeContext::dynamicCodeContext(const dictionary& dict)
:
  dict_{dict},
  code_{},
  localCode_{},
  include_{},
  options_{},
  libs_{}
{
  // expand dictionary entries
  {
    const entry& codeEntry = dict.lookupEntry("code", false, false);
    code_ = stringOps::trim(codeEntry.stream());
    stringOps::inplaceExpand(code_, dict);
  }
  // note: removes any leading/trailing whitespace
  // - necessary for compilation options, convenient for includes
  // and body.
  // optional
  const entry* includePtr = dict.lookupEntryPtr
  (
    "codeInclude",
    false,
    false
  );
  if (includePtr) {
    include_ = stringOps::trim(includePtr->stream());
    stringOps::inplaceExpand(include_, dict);
  }
  // optional
  const entry* optionsPtr = dict.lookupEntryPtr
  (
    "codeOptions",
    false,
    false
  );
  if (optionsPtr) {
    options_ = stringOps::trim(optionsPtr->stream());
    stringOps::inplaceExpand(options_, dict);
  }
  // optional
  const entry* libsPtr = dict.lookupEntryPtr("codeLibs", false, false);
  if (libsPtr) {
    libs_ = stringOps::trim(libsPtr->stream());
    stringOps::inplaceExpand(libs_, dict);
  }
  // optional
  const entry* localPtr = dict.lookupEntryPtr("localCode", false, false);
  if (localPtr) {
    localCode_ = stringOps::trim(localPtr->stream());
    stringOps::inplaceExpand(localCode_, dict);
  }
  // calculate SHA1 digest from include, options, localCode, code
  OSHA1stream os;
  os << include_ << options_ << libs_ << localCode_ << code_;
  sha1_ = os.digest();
  // Add line number after calculating sha1 since includes processorDDD
  // in path which differs between processors.
  {
    const entry& codeEntry = dict.lookupEntry("code", false, false);
    addLineDirective(code_, codeEntry.startLineNumber(), dict.name());
  }
  if (includePtr) {
    addLineDirective(include_, includePtr->startLineNumber(), dict.name());
  }
  // Do not add line directive to options_ (Make/options) and libs since
  // they are preprocessed as a single line at this point. Can be fixed.
  if (localPtr) {
    addLineDirective(localCode_, localPtr->startLineNumber(), dict.name());
  }
}


// Member Functions 
void mousse::dynamicCodeContext::addLineDirective
(
  string& code,
  const label lineNum,
  const fileName& name
)
{
  code = "#line " + mousse::name(lineNum + 1) + " \"" + name + "\"\n" + code;
}
