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
#include "Config.h"

Config::Config(const std::wstring &name,const std::wstring &directory)
  : _name(name),
    _directory(directory)
{
  _disabledArm64=false;
  _incompatibleLicense=false;
  _isOptional=false;
  _isImageMagick7Only=false;
  _isMagickProject=false;
  _type=ProjectType::Undefined;
  _useNasm=false;
  _useOpenCL=false;
  _useUnicode=false;
}

const std::set<std::wstring> Config::defines(const ProjectType projectType) const
{
  return((projectType == ProjectType::StaticLibrary ? _staticDefines : _dynamicDefines));
}

const std::set<std::wstring> Config::excludes(const Architecture architecture) const
{
  switch (architecture)
  {
  case Architecture::Arm64: return(_excludesArm64);
  case Architecture::x64: return(_excludesX64);
  case Architecture::x86: return(_excludesX86);
  default: throwException(L"Unknown architecture");
  }
}

const std::set<std::wstring> Config::nasmIncludes(const Architecture architecture) const
{
  switch (architecture)
  {
  case Architecture::x64: return(_includesNasmX64);
  case Architecture::x86: return(_includesNasmX86);
  default: throwException(L"Unsupported architecture");
  }
}

Config Config::load(const std::wstring name,const std::wstring &directory,const std::wstring &configFile)
{
  Config config(name,directory);
  config.load(configFile);

  return(config);
}

void Config::addIncludeArtifacts(std::wifstream &config)
{
  std::set<std::wstring>
    artifacts;

  addLines(config,artifacts);

  for (const auto& artifact : artifacts)
  {
    size_t offset=artifact.find(L" -> ");
    if (offset == std::wstring::npos)
      throwException(L"Invalid include artifact: " + artifact);

    std::wstring source=_directory + artifact.substr(0,offset);
    std::wstring target=artifact.substr(offset + 4);
    if (target == L".")
      _includeArtifacts[source]=L"";
    else
      _includeArtifacts[source]=target;
  }
}

void Config::addIncludes(std::wifstream &config,std::set<std::wstring> &container)
{
  std::set<std::wstring>
    names;

  addLines(config,names);

  for (const auto& name : names)
  {
    if (name == L".")
      container.insert(L"");
    else
      container.insert(name);
  }
}

void Config::addLines(std::wifstream &config,std::wstring &value)
{
  for (auto& line : readLines(config))
    value+=line+L"\n";
}

void Config::addLines(std::wifstream &config,std::set<std::wstring> &container)
{
  for (auto& line : readLines(config))
    container.insert(line);
}

void Config::load(const std::wstring &configFile)
{
  std::wifstream
    config;

  std::wstring
    line;

  std::set<std::wstring>
    defines,
    excludes,
    includesNasm;

  if (!std::filesystem::exists(configFile))
    throwException(L"Unable to open config file: " + configFile);

  config.open(configFile);
  while (!config.eof())
  {
    line=readLine(config);
    if (line == L"[DEFINES]")
      addLines(config,defines);
    else if (line == L"[DEMO]")
      _type=ProjectType::Demo;
    else if (line == L"[DYNAMIC_LIBRARY]")
      _type=ProjectType::DynamicLibrary;
    else if (line == L"[FUZZ]")
      _type=ProjectType::Fuzz;
    else if (line == L"[DIRECTORY]")
      _directory=readLine(config);
    else if (line == L"[DISABLED_ARM64]")
      _disabledArm64=true;
    else if (line == L"[DYNAMIC_DEFINES]")
      addLines(config,_dynamicDefines);
    else if (line == L"[EXE]")
      _type=ProjectType::Exe;
    else if (line == L"[EXCLUDES]")
      addLines(config,excludes);
    else if (line == L"[EXCLUDES_ARM64]")
      addLines(config,_excludesArm64);
    else if (line == L"[EXCLUDES_X64]")
      addLines(config,_excludesX64);
    else if (line == L"[EXCLUDES_X86]")
      addLines(config,_excludesX86);
    else if (line == L"[INCLUDES]")
      addIncludes(config,_includes);
    else if (line == L"[INCLUDES_NASM]")
      addIncludes(config,includesNasm);
    else if (line == L"[INCLUDES_NASM_X64]")
      addIncludes(config,_includesNasmX64);
    else if (line == L"[INCLUDES_NASM_X86]")
      addIncludes(config,_includesNasmX86);
    else if (line == L"[INCLUDE_ARTIFACTS]")
      addIncludeArtifacts(config);
    else if (line == L"[INCOMPATIBLE_LICENSE]")
      _incompatibleLicense=true;
    else if (line == L"[LICENSE]")
      addLines(config,_licenses);
    else if (line == L"[LINKED_LIBS]")
      addLines(config,_linkedLibs);
    else if (line == L"[MAGICK_BASECONFIG_DEFINE]")
      addLines(config,_magickBaseconfigDefine);
    else if (line == L"[MAGICK_PROJECT]")
      _isMagickProject=true;
    else if (line == L"[MODULE_DEFINITION_FILE]")
      _moduleDefinitionFile=readLine(config);
    else if (line == L"[NASM]")
      _useNasm=true;
    else if (line == L"[ONLY_IMAGEMAGICK7]")
      _isImageMagick7Only=true;
    else if (line == L"[OPENCL]")
      _useOpenCL=true;
    else if (line == L"[OPTIONAL]")
      _isOptional=true;
    else if (line == L"[STATIC_LIBRARY]")
      _type=ProjectType::StaticLibrary;
    else if (line == L"[STATIC_DEFINES]")
      addLines(config,_staticDefines);
    else if (line == L"[UNICODE]")
      _useUnicode=true;
    else if (line == L"[REFERENCES]")
      addLines(config,_references);
    else if (line == L"[UTIL]")
      _type=ProjectType::Util;
    else if (!line.empty())
      throwException(L"Unknown section in config file: " + line);
  }

  config.close();

  for (const auto& define : defines)
  {
    _dynamicDefines.insert(define);
    _staticDefines.insert(define);
  }

  for (const auto& exclude : excludes)
  {
    _excludesArm64.insert(exclude);
    _excludesX64.insert(exclude);
    _excludesX86.insert(exclude);
  }

  for (const auto& include : includesNasm)
  {
    _includesNasmX64.insert(include);
    _includesNasmX86.insert(include);
  }
}

std::wstring Config::readLine(std::wifstream &stream)
{
  std::wstring
    line;

  getline(stream,line);
  return(trim(line));
}

std::vector<std::wstring> Config::readLines(std::wifstream &config)
{
  std::vector<std::wstring>
    lines;

  while (!config.eof())
  {
    std::wstring line=readLine(config);
    if (line.empty())
      return(lines);

    lines.push_back(line);
  }

  return(lines);
}
