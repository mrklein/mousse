// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
inline void mousse::fileName::stripInvalid()
{
  if (debug && string::stripInvalid<fileName>(*this))
  {
    std::cerr
      << "fileName::stripInvalid() called for invalid fileName "
      << this->c_str() << std::endl;
    if (debug > 1)
    {
      std::cerr
        << "    For debug level (= " << debug
        << ") > 1 this is considered fatal" << std::endl;
      std::abort();
    }
    removeRepeated('/');
    removeTrailing('/');
  }
}
// Constructors 
inline mousse::fileName::fileName()
:
  string()
{}
inline mousse::fileName::fileName(const fileName& fn)
:
  string(fn)
{}
inline mousse::fileName::fileName(const word& w)
:
  string(w)
{}
inline mousse::fileName::fileName(const string& str)
:
  string(str)
{
  stripInvalid();
}
inline mousse::fileName::fileName(const std::string& str)
:
  string(str)
{
  stripInvalid();
}
inline mousse::fileName::fileName(const char* str)
:
  string(str)
{
  stripInvalid();
}
// Member Functions 
inline bool mousse::fileName::valid(char c)
{
  return
  (
    !isspace(c)
  && c != '"'   // string quote
  && c != '\''  // string quote
  );
}
