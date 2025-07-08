/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%  Copyright @ 1999 ImageMagick Studio LLC, a non-profit organization         %
%  dedicated to making software imaging solutions freely available.           %
%                                                                             %
%  You may not use this file except in compliance with the License.  You may  %
%  obtain a copy of the License at                                            %
%                                                                             %
%    http://www.imagemagick.org/script/license.php                            %
%                                                                             %
%  Unless required by applicable law or agreed to in writing, software        %
%  distributed under the License is distributed on an "AS IS" BASIS,          %
%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
%  See the License for the specific language governing permissions and        %
%  limitations under the License.                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/
#pragma once

using namespace std;

enum class Architecture {x86, x64, Arm64};

enum class Compiler {Default, CPP};

enum class PolicyConfig {Limited, Open, Secure, WebSafe};

enum class ProjectType {Undefined, Application, Coder, Demo, Filter, Fuzz, DynamicLibrary, StaticLibrary};

enum class QuantumDepth {Q8, Q16, Q32, Q64};

enum class VisualStudioVersion {VS2017, VS2019, VS2022};

#define VSLATEST VisualStudioVersion::VS2022

static inline bool endsWith(const wstring &s,const wstring &end)
{
  size_t
    index;

  index=s.rfind(end);
  if (index == wstring::npos)
    return(false);

  return(s.substr(index) == end);
}

static inline bool startsWith(const wstring &s,const wstring &start)
{
  size_t
    index;

  index=s.rfind(start);
  return(index == 0);
}

static inline wstring trim(const wstring &input)
{
  wstring
    result;

  result=input;
  result.erase(result.begin(), find_if(result.begin(), result.end(), [](int c) {return(!isspace(c));}));
  result.erase(find_if(result.rbegin(), result.rend(), [](int c) {return(!isspace(c));}).base(), result.end());
  return(result);
}

static inline string wstringToString(const wstring &ws)
{ 
  string
    result;

  transform(ws.begin(), ws.end(), back_inserter(result), [] (wchar_t c) { return((char)c); }); 
  return(result);
}

static inline wstring createGuid(const wstring &name)
{
  hash<string>
    hash;

  size_t
    hash_value;

  wstringstream
    wss;

  wstring
    guid;

  hash_value=hash(wstringToString(name));
  wss << hex << setw(16) << setfill(L'0') << hash_value;
  wss << hex << setw(16) << setfill(L'0') << hash_value;
  guid=wss.str();
  guid.insert(20, 1, '-');
  guid.insert(16, 1, '-');
  guid.insert(12, 1, '-');
  guid.insert(8, 1, '-');
  transform(guid.begin(),guid.end(),guid.begin(),[](wchar_t c) { return(towupper(c)); });
  return(guid);
}

[[noreturn]]
static inline void throwException(const wstring& message)
{
#ifdef _DEBUG
  if (IsDebuggerPresent())
    DebugBreak();
#endif
  throw runtime_error(wstringToString(message));
}

static inline wstring replace(const wstring &str,const wstring &from,const wstring &to)
{
  size_t
    index;

  wstring
    result;

  result=str;
  index=result.find(from);
  while (index != string::npos)
  {
    result.replace(index,from.length(),to);
    index=result.find(from,index+to.length());
  }
  return(result);
}
