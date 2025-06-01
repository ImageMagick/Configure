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
//
//#include <algorithm>
//#include <functional>
//#include <cctype>
//#include <locale>

enum class Architecture {x86, x64, Arm64};

enum class BuildType {Dynamic, Static};
//
//enum class Compiler {Default, CPP};

enum class ImageMagickVersion { ImageMagick6 = 1, ImageMagick7 = 2};

enum class PolicyConfig {Limited, Open, Secure, WebSafe};

enum class ProjectType {Undefined, Demo, Exe, Fuzz, DynamicLibrary, Module, StaticLibrary, Util};

enum class QuantumDepth {Q8, Q16, Q32, Q64};

enum class VisualStudioVersion {VS2017, VS2019, VS2022};

#define VSLATEST VisualStudioVersion::VS2022

//
//#define VSEARLIEST VisualStudioVersion::VS2017
//
//static const wstring validSrcFilesArr[] = {L".c", L".cpp", L".cc"};
//const vector<wstring> validSrcFiles(validSrcFilesArr, std::end(validSrcFilesArr));
//

static inline bool endsWith(const std::wstring &s,const std::wstring &end)
{
  size_t
    index;

  index=s.rfind(end);
  if (index == std::wstring::npos)
    return(false);

  return(s.substr(index) == end);
}
//
static inline bool startsWith(const std::wstring &s,const std::wstring &start)
{
  size_t
    index;

  index=s.rfind(start);
  return(index == 0);
}
//
//static inline bool contains(const vector<wstring> &container, const wstring s)
//{
//  for (auto& c : container)
//  {
//    if (c == s)
//      return(true);
//    if ((c[0] == '*') && (endsWith(s,c.substr(1))))
//      return(true);
//    if ((c[c.length()-1] == '*') && (startsWith(s,c.substr(0,c.length()-1))))
//      return(true);
//  }
//  return(false);
//}
//
//static bool directoryExists(const std::wstring& folderPath)
//{
//  DWORD
//    folderAttr = GetFileAttributesW(folderPath.c_str());
//
//  if (folderAttr == INVALID_FILE_ATTRIBUTES)
//    return false;
//  return (folderAttr & FILE_ATTRIBUTE_DIRECTORY);
//}
//
static inline std::wstring trim(const std::wstring &input)
{
  std::wstring
    result;

  result=input;
  result.erase(result.begin(), find_if(result.begin(), result.end(), [](int c) {return(!std::isspace(c));}));
  result.erase(find_if(result.rbegin(), result.rend(), [](int c) {return(!std::isspace(c));}).base(), result.end());
  return(result);
}

static inline std::string wstringToString(const std::wstring &ws)
{ 
  std::string
    result;

  transform(ws.begin(), ws.end(), std::back_inserter(result), [] (wchar_t c) { return((char)c); }); 
  return(result);
}

static inline std::wstring createGuid(const std::wstring &name)
{
  std::hash<std::string>
    hash;

  size_t
    hash_value;

  std::wstringstream
    wss;

  std::wstring
    guid;

  hash_value=hash(wstringToString(name));
  wss << std::hex << std::setw(16) << std::setfill(L'0') << hash_value;
  wss << std::hex << std::setw(16) << std::setfill(L'0') << hash_value;
  guid=wss.str();
  guid.insert(20, 1, '-');
  guid.insert(16, 1, '-');
  guid.insert(12, 1, '-');
  guid.insert(8, 1, '-');
  transform(guid.begin(),guid.end(),guid.begin(),[](wchar_t c) { return(towupper(c)); });
  return(guid);
}

[[noreturn]]
static inline void throwException(const std::wstring& message)
{
#ifdef _DEBUG
  if (IsDebuggerPresent())
    __debugbreak();
#endif
  throw std::runtime_error(wstringToString(message));
}
//
//static inline wstring readLicense(const wstring &fileName)
//{
//  wifstream
//    file;
//
//  wstring
//    content;
//
//  file.open(fileName);
//  if (!file)
//    throwException(L"Unable to open license file: " + fileName);
//
//  content=trim(wstring((istreambuf_iterator<wchar_t>(file)),istreambuf_iterator<wchar_t>()));
//
//  file.close();
//
//  return(content);
//}
//
//static inline wstring replace(const wstring &str,const wstring &from,const wstring &to)
//{
//  size_t
//    index;
//
//  wstring
//    result;
//
//  result=str;
//  index=result.find(from);
//  while (index != string::npos)
//  {
//    result.replace(index,from.length(),to);
//    index=result.find(from,index+to.length());
//  }
//  return(result);
//}
//
//static inline VisualStudioVersion parseVisualStudioVersion(const wstring &version)
//{
//  if (version == L"2017")
//    return(VisualStudioVersion::VS2017);
//  else if (version == L"2019")
//    return(VisualStudioVersion::VS2019);
//  else if (version == L"2022")
//    return(VisualStudioVersion::VS2022);
//
//  return(VSEARLIEST);
//}
//
//static inline bool isValidSrcFile(const wstring &fileName)
//{
//  for (auto& ext : validSrcFiles)
//  {
//    if (endsWith(fileName,ext))
//      return(true);
//  }
//
//  return(false);
//}
//
//static inline const wstring pathFromRoot(const wstring &path)
//{
//  return(L"..\\" + path);
//}
//
