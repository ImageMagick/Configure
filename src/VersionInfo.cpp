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
#include "VersionInfo.h"

VersionInfo::VersionInfo(const Options& options)
  : _options(options)
{
}

const wstring VersionInfo::fullVersion() const
{
  return(_major+L"."+_minor+L"."+_micro+L"."+_patchlevel);
}

const wstring VersionInfo::interfaceVersion() const
{
  return(_libraryCurrent);
}

const wstring VersionInfo::libAddendum() const
{
  if (_isBeta == L"y")
    return(L"-"+_patchlevel+L" (Beta)");
  else
    return(L"-"+_patchlevel);
}

const wstring VersionInfo::libVersionNumber() const
{
  return(_libraryCurrent+L","+_libraryRevision+L","+_libraryAge);
}

const wstring VersionInfo::ppInterfaceVersion() const
{
  return(_ppLibraryCurrent);
}

const wstring VersionInfo::ppLibVersionNumber() const
{
  return(_ppLibraryCurrent+L":"+_ppLibraryRevision+L":"+_ppLibraryAge);
}

const wstring VersionInfo::quantumDepthBits() const
{
  switch (_options.quantumDepth)
  {
    case QuantumDepth::Q8: return(L"8");
    case QuantumDepth::Q16: return(L"16"); 
    case QuantumDepth::Q32: return(L"32"); 
    case QuantumDepth::Q64: return(L"64"); 
    default: throw;
  }
}

const wstring VersionInfo::version() const
{
  return(_major+L"."+_minor+L"."+_micro);
}

const wstring VersionInfo::versionNumber() const
{
  return(_major+L","+_minor+L","+_micro+L","+_patchlevel);
}

const wstring VersionInfo::visualStudioVersionName() const
{
  switch(_options.visualStudioVersion)
  {
    case VisualStudioVersion::VS2017: return(L"VS2017");
    case VisualStudioVersion::VS2019: return(L"VS2019");
    case VisualStudioVersion::VS2022: return(L"VS2022");
    default: throwException(L"Unknown Visual Studio version");
  }
}

const wstring VersionInfo::executeCommand(const wstring &command) const
{
  HANDLE hRead, hWrite;
  SECURITY_ATTRIBUTES sa={ sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

  if (!CreatePipe(&hRead,&hWrite,&sa,0))
    return(L"");

  SetHandleInformation(hRead,HANDLE_FLAG_INHERIT,0);

  PROCESS_INFORMATION pi = {};
  STARTUPINFOW si = {};
  si.cb=sizeof(si);
  si.dwFlags=STARTF_USESTDHANDLES;
  si.hStdOutput=hWrite;
  si.hStdError=hWrite;
  si.hStdInput=NULL;

  wstring cmd=L"cmd.exe /C " + command;
  if (!CreateProcessW(NULL,cmd.data(),NULL,NULL,TRUE,CREATE_NO_WINDOW,NULL,NULL,&si,&pi))
  {
    CloseHandle(hWrite);
    CloseHandle(hRead);
    return(L"");
  }
  CloseHandle(hWrite);

  wstring result;
  char buffer[4096] = { 0 };
  DWORD bytesRead;
  while (ReadFile(hRead,buffer,sizeof(buffer) - 1,&bytesRead,NULL) && bytesRead)
  {
      buffer[bytesRead] = '\0';
      int length=MultiByteToWideChar(CP_UTF8,0,buffer,-1,NULL,0);
      wstring data(length - 1,0);
      MultiByteToWideChar(CP_UTF8,0,buffer,-1,&data[0],length);
      result+=data;
  }
  CloseHandle(hRead);

  DWORD exitCode;
  WaitForSingleObject(pi.hProcess,INFINITE);
  GetExitCodeProcess(pi.hProcess,&exitCode);

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  
  if (exitCode != 0)
    return L"";

  result=replace(result,L"\n",L"");
  return result;
}

const wstring VersionInfo::getFileModificationDate(const wstring &fileName,const wstring &format) const
{
  wchar_t
    buffer[20];

  struct tm
    tm;

  struct _stat64
    attributes;

  if (_wstati64(fileName.c_str(),&attributes) != 0)
    return(L"");
  (void) localtime_s(&tm,&attributes.st_mtime);
  (void) wcsftime(buffer,20,format.c_str(),&tm);
  return(wstring(buffer));
}

optional<VersionInfo> VersionInfo::load(const Options& options)
{
  wstring versionFile = options.rootDirectory + L"ImageMagick\\m4\\version.m4";
  if (!filesystem::exists(versionFile))
    return nullopt;

  VersionInfo versionInfo(options);
  versionInfo.load(versionFile);

  return(versionInfo);
}

void VersionInfo::load(const wstring fileName)
{
  wifstream
    version;

  wstring
    line;

  version.open(fileName);
  if (!version)
    return;

  while (getline(version,line))
  {
    loadValue(line,L"_is_beta",&_isBeta);
    loadValue(line,L"_library_current",&_libraryCurrent);
    loadValue(line,L"_library_revision",&_libraryRevision);
    loadValue(line,L"_library_age",&_libraryAge);
    loadValue(line,L"_lib_version",&_libVersion);
    loadValue(line,L"_major_version",&_major);
    loadValue(line,L"_minor_version",&_minor);
    loadValue(line,L"_micro_version",&_micro);
    loadValue(line,L"_patchlevel_version",&_patchlevel);
    loadValue(line,L"pp_library_current",&_ppLibraryCurrent);
    loadValue(line,L"pp_library_revision",&_ppLibraryRevision);
    loadValue(line,L"pp_library_age",&_ppLibraryAge);
  }

  version.close();

  setGitRevision();
  setReleaseDate();

  if (_major == L"" && _minor == L"" && _micro == L"" && _patchlevel == L"" && _libraryCurrent == L"" &&
      _libraryRevision == L"" && _libraryAge == L"" && _libVersion != L"" && _ppLibraryCurrent == L"" &&
      _ppLibraryRevision == L"" && _ppLibraryAge == L"" && _gitRevision == L"" && _releaseDate == L"")
    throwException(L"Unable to load version information from: " + fileName);
}

void VersionInfo::loadValue(const wstring &line,const wstring &keyword,wstring *value) const
{
  size_t
    index;

  wstring
    line_start;

  line_start=L"m4_define([magick"+keyword+L"], [";
  index=line.find(line_start);
  if (index == string::npos)
    return;
  *value=line.substr(line_start.length(),line.length()-line_start.length()-2);
}

wstring VersionInfo::replaceVariable(const wstring &line,const size_t start) const
{
  static vector<wstring> skipableKeywords={
    L"CODER_PATH",L"CONFIGURE_ARGS",L"CONFIGURE_PATH",L"CXXFLAGS",L"DEFS",L"DISTCHECK_CONFIG_FLAGS",
    L"EXEC_PREFIX_DIR",L"EXECUTABLE_PATH",L"FILTER_PATH",L"host",L"INCLUDE_PATH",L"LIBRARY_ABSOLUTE_PATH",
    L"MAGICK_CFLAGS",L"MAGICK_CPPFLAGS",L"MAGICK_DELEGATES",L"MAGICK_FEATURES",L"MAGICK_LDFLAGS",
    L"MAGICK_LIBS",L"MAGICK_PCFLAGS",L"MAGICK_SECURITY_POLICY",L"MAGICK_TARGET_VENDOR",L"PREFIX_DIR",
    L"SHARE_PATH",L"SHAREARCH_PATH"
  };

  size_t end=line.find(L"@",start + 1);
  if (end == wstring::npos)
    return(line);

  wstring keyword=line.substr(start + 1,end - start - 1);
  if (find(skipableKeywords.begin(), skipableKeywords.end(), keyword) != skipableKeywords.end())
    return(L"");

  if (keyword == L"CC") return(replaceVariable(line,start,end,visualStudioVersionName()));
  else if (keyword == L"CXX") return(replaceVariable(line,start,end,visualStudioVersionName()));
  else if (keyword == L"CHANNEL_MASK_DEPTH") return(replaceVariable(line,start,end,_options.channelMaskDepth()));
  else if (keyword == L"DOCUMENTATION_PATH") return(replaceVariable(line,start,end,L"unavailable"));
  else if (keyword == L"MAGICK_GIT_REVISION") return(replaceVariable(line,start,end,_gitRevision));
  else if (keyword == L"MAGICK_LIB_VERSION_NUMBER") return(replaceVariable(line,start,end,libVersionNumber()));
  else if (keyword == L"MAGICK_LIB_VERSION_TEXT") return(replaceVariable(line,start,end,version()));
  else if (keyword == L"MAGICK_LIBRARY_CURRENT") return(replaceVariable(line,start,end,interfaceVersion()));
  else if (keyword == L"MAGICK_LIBRARY_CURRENT_MIN") return(replaceVariable(line,start,end,interfaceVersion()));
  else if (keyword == L"MAGICK_TARGET_CPU") return(replaceVariable(line,start,end,_options.architectureName()));
  else if (keyword == L"MAGICK_TARGET_OS") return(replaceVariable(line,start,end,L"Windows"));
  else if (keyword == L"MAGICKPP_LIB_VERSION_TEXT") return(replaceVariable(line,start,end,version()));
  else if (keyword == L"MAGICKPP_LIBRARY_VERSION_INFO") return(replaceVariable(line,start,end,ppLibVersionNumber()));
  else if (keyword == L"MAGICKPP_LIBRARY_VERSION_TEXT") return(replaceVariable(line,start,end,version()));
  else if (keyword == L"MAGICKPP_LIBRARY_CURRENT") return(replaceVariable(line,start,end,ppInterfaceVersion()));
  else if (keyword == L"MAGICKPP_LIBRARY_CURRENT_MIN") return(replaceVariable(line,start,end,ppInterfaceVersion()));
  else if (keyword == L"PACKAGE_BASE_VERSION") return(replaceVariable(line,start,end,version()));
  else if (keyword == L"PACKAGE_FULL_VERSION") return(replaceVariable(line,start,end,fullVersion()));
  else if (keyword == L"PACKAGE_NAME") return(replaceVariable(line,start,end,L"ImageMagick"));
  else if (keyword == L"PACKAGE_LIB_VERSION") return(replaceVariable(line,start,end,_libVersion));
  else if (keyword == L"PACKAGE_LIB_VERSION_NUMBER") return(replaceVariable(line,start,end,versionNumber()));
  else if (keyword == L"PACKAGE_RELEASE_DATE") return(replaceVariable(line,start,end,_releaseDate));
  else if (keyword == L"PACKAGE_VERSION_ADDENDUM") return(replaceVariable(line,start,end,libAddendum()));
  else if (keyword == L"QUANTUM_DEPTH") return(replaceVariable(line,start,end,quantumDepthBits()));
  else throwException(L"Unknown variable: " + keyword);
}

wstring VersionInfo::replaceVariable(const wstring& line,const size_t start,const size_t end,const wstring &newValue) const
{
  wstring newLine=line.substr(0,start) + newValue + line.substr(end + 1);

  size_t newStart=newLine.find(L"@");
  if (newStart == wstring::npos)
    return(newLine);

  return(replaceVariable(newLine,newStart));
}

void VersionInfo::setGitRevision()
{
  _gitRevision=executeCommand(L"cd \"" + _options.rootDirectory + L"ImageMagick\"" + L" && git rev-parse --short HEAD");
  if (_gitRevision != L"")
    _gitRevision+=executeCommand(L"cd \"" + _options.rootDirectory + L"ImageMagick\"" + L" && git log -1 --format=:%cd --date=format:%Y%m%d");
  if (_gitRevision == L"")
    _gitRevision=getFileModificationDate(_options.rootDirectory + L"ImageMagick\\m4\\version.m4",L"%Y%m%d");
}

void VersionInfo::setReleaseDate()
{
  _releaseDate=executeCommand(L"cd " + _options.rootDirectory + L"ImageMagick\"" + L" && git log -1 --format=%cd --date=format:%Y-%m-%d");
  if (_releaseDate == L"")
    _releaseDate=getFileModificationDate(_options.rootDirectory + L"ImageMagick\\m4\\version.m4",L"%Y-%m-%d");
}

void VersionInfo::write() const
{
  wstring versionFile=L"ImageMagick\\" + _options.magickCoreName() + L"\\version.h";

  write(L"Build\\package.version.h.in",L"Build\\package.version.h");
  write(L"ImageMagick\\" + _options.magickCoreName() + L"\\version.h.in",versionFile);
  write(L"ImageMagick\\config\\configure.xml.in",L"Artifacts\\bin\\configure.xml");

  filesystem::copy(_options.rootDirectory + versionFile,_options.rootDirectory + L"Build\\version.h",filesystem::copy_options::overwrite_existing);
}

void VersionInfo::write(wstring inputFile,wstring outputFile) const
{
  wifstream input(_options.rootDirectory + inputFile);
  if (!input)
    throwException(L"Unable to open: " + inputFile);

  wofstream output(_options.rootDirectory + outputFile);
  if (!output)
    throwException(L"Unable to open: " + outputFile);
  
  wstring line;
  while (getline(input,line))
  {
    size_t offset=line.find(L"@");
    if (offset != wstring::npos)
    {
      line=replaceVariable(line,offset);
      if (line.empty())
        continue;
    }

    output << line << endl;
  }
}
