// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "ostring_stream.hpp"
#include "os_specific.hpp"
#include "ifstream.hpp"
#if defined(darwin64) && defined(__clang__)
#include "ulong.hpp"
#endif
#include <inttypes.h>
#include <cxxabi.h>
#include <execinfo.h>
#include <dlfcn.h>
#if defined(darwin64)
#include <regex.h>
#endif
namespace mousse
{
string pOpen(const string &cmd, label line=0)
{
  string res = "\n";
  FILE *cmdPipe = popen(cmd.c_str(), "r");
  if (cmdPipe)
  {
    char *buf = NULL;
    // Read line number of lines
    for (label cnt = 0; cnt <= line; cnt++)
    {
      size_t linecap = 0;
      ssize_t linelen;
      linelen = getline(&buf, &linecap, cmdPipe);
      if (linelen < 0)
      {
        break;
      }
      if (cnt == line)
      {
        res = string(buf);
        break;
      }
    }
    if (buf != NULL)
    {
      free(buf);
    }
    pclose(cmdPipe);
  }
  return res.substr(0, res.size() - 1);
}
inline word addressToWord(const uintptr_t addr)
{
  OStringStream nStream;
  nStream << "0x" << hex << addr;
  return nStream.str();
}
void printSourceFileAndLine
(
  Ostream& os,
  const fileName& filename,
  Dl_info *info,
  void *addr
)
{
  uintptr_t address = uintptr_t(addr);
  word myAddress = addressToWord(address);
#if ! defined(darwin64)
  if (filename.ext() == "so")
  {
    // Convert address into offset into dynamic library
    uintptr_t offset = uintptr_t(info->dli_fbase);
    intptr_t relativeAddress = address - offset;
    myAddress = addressToWord(relativeAddress);
  }
#endif
  if (filename[0] == '/')
  {
    string line = pOpen
    (
#if ! defined(darwin64)
      "addr2line -f --demangle=auto --exe "
     + filename
     + " "
     + myAddress,
      1
#else
      "echo 'image lookup -a "
     + myAddress
     + " "
     + filename
     + "'"
     + " | xcrun lldb "
     + "-O 'target create --no-dependents -a x86_64 "
     + filename
     + "' -o '"
     + "target modules load -f "
     + filename
     + " __TEXT "
     + addressToWord(reinterpret_cast<const uintptr_t>(info->dli_fbase))
     + "' 2> /dev/null"
     + " | tail -1"
#endif
    );
#if defined(darwin64)
    {
      const char *buf = line.c_str();
      regex_t re;
      regmatch_t mt[3];
      int st;
      regcomp(&re, ".\\+at \\(.\\+\\):\\(\\d\\+\\)", REG_ENHANCED);
      st = regexec(&re, buf, 3, mt, 0);
      if (st == REG_NOMATCH)
      {
        line = "??:0";
      }
      else
      {
        size_t len = mt[1].rm_eo - mt[1].rm_so;
        string fname(buf + mt[1].rm_so, len);
        len = mt[2].rm_eo - mt[2].rm_so;
        string lnum(buf + mt[2].rm_so, len);
        line = fname + ":" + lnum;
      }
      regfree(&re);
    }
#endif
    if (line == "")
    {
      os  << " addr2line failed";
    }
    else
    {
      word location_preposition = " at ";
      if (line == "??:0")
      {
        line = filename;
        location_preposition = " in ";
      }
      string cwdLine(line.replaceAll(cwd() + '/', ""));
      string homeLine(cwdLine.replaceAll(home(), '~'));
      os  << location_preposition << homeLine.c_str();
    }
  }
}
fileName absolutePath(const char* fn)
{
  fileName fname(fn);
  if (fname[0] != '/' && fname[0] != '~')
  {
    string tmp = pOpen("which " + fname);
    if (tmp[0] == '/' || tmp[0] == '~')
    {
      fname = tmp;
    }
  }
  return fname;
}
string demangleSymbol(const char* sn)
{
  string res;
  int st;
  char* cxx_sname = abi::__cxa_demangle
  (
    sn,
    NULL,
    0,
    &st
  );
  if (st == 0 && cxx_sname)
  {
    res = string(cxx_sname);
    free(cxx_sname);
  }
  else
  {
    res = string(sn);
  }
  return res;
}
}  // namespace mousse
void mousse::error::safePrintStack(std::ostream& os)
{
  // Get raw stack symbols
  void *array[100];
  size_t size = backtrace(array, 100);
  char **strings = backtrace_symbols(array, size);
  // See if they contain function between () e.g. "(__libc_start_main+0xd0)"
  // and see if cplus_demangle can make sense of part before +
  for (size_t i = 0; i < size; i++)
  {
    string msg(strings[i]);
    fileName programFile;
    word address;
    os  << '#' << label(i) << '\t' << msg << std::endl;
  }
}
void mousse::error::printStack(Ostream& os)
{
  // Get raw stack symbols
  const size_t CALLSTACK_SIZE = 128;
  void *callstack[CALLSTACK_SIZE];
  size_t size = backtrace(callstack, CALLSTACK_SIZE);
  Dl_info *info = new Dl_info;
  fileName fname = "???";
  word address;
  for(size_t i=0; i<size; i++)
  {
    int st = dladdr(callstack[i], info);
    os << '#' << label(i) << "  ";
    if (st != 0 && info->dli_fname != NULL && info->dli_fname[0] != '\0')
    {
      fname = absolutePath(info->dli_fname);
      os <<
      (
        (info->dli_sname != NULL)
       ? demangleSymbol(info->dli_sname)
       : "?"
      );
    }
    else
    {
      os << "?";
    }
    printSourceFileAndLine(os, fname, info, callstack[i]);
    os << nl;
  }
  delete info;
}
