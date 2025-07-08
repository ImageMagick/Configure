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

Config::Config(const wstring &name,const wstring &directory)
  : _name(name),
    _directory(directory)
{
  _disabledForArm64=false;
  _hasIncompatibleLicense=false;
  _isOptional=false;
  _isImageMagick7Only=false;
  _isMagickProject=false;
  _type=ProjectType::Undefined;
  _useNasm=false;
  _useOpenCL=false;
  _useUnicode=false;
}

const set<wstring>& Config::excludes(const Architecture architecture) const
{
  switch (architecture)
  {
  case Architecture::Arm64: return(_excludesArm64);
  case Architecture::x64: return(_excludesX64);
  case Architecture::x86: return(_excludesX86);
  default: throwException(L"Unknown architecture");
  }
}

const set<wstring>& Config::nasmIncludes(const Architecture architecture) const
{
  switch (architecture)
  {
  case Architecture::x64: return(_includesNasmX64);
  case Architecture::x86: return(_includesNasmX86);
  default: throwException(L"Unsupported architecture");
  }
}

void Config::addIncludeArtifacts(wifstream &config)
{
  set<wstring>
    artifacts;

  addLines(config,artifacts);

  for (const auto& artifact : artifacts)
  {
    size_t offset=artifact.find(L" -> ");
    if (offset == wstring::npos)
      throwException(L"Invalid include artifact: " + artifact);

    wstring source=_directory + artifact.substr(0,offset);
    wstring target=artifact.substr(offset + 4);
    if (target == L".")
      _includeArtifacts[source]=L"";
    else
      _includeArtifacts[source]=target;
  }
}

void Config::addIncludes(wifstream &config,set<wstring> &container)
{
  set<wstring>
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

void Config::addLines(wifstream &config,wstring &value)
{
  for (auto& line : readLines(config))
    value+=line+L"\n";
}

void Config::addLines(wifstream &config,set<wstring> &container)
{
  for (auto& line : readLines(config))
    container.insert(line);
}

const Config Config::copyInfo(const Config &config) const
{
  Config newConfig(*this);

  for (auto& define : config._dynamicDefines)
    newConfig._dynamicDefines.insert(define);

  for (auto& define : config._staticDefines)
    newConfig._staticDefines.insert(define);

  for (auto& references : config._references)
    newConfig._references.insert(references);

  for (auto& references : config._coderReferences)
    newConfig._coderReferences.insert(references);

  return(newConfig);
}

Config Config::load(const wstring name,const wstring &directory,const wstring &configFile)
{
  Config config(name,directory);
  config.load(configFile);

  return(config);
}

void Config::load(const wstring &configFile)
{
  wifstream
    config;

  wstring
    line;

  set<wstring>
    defines,
    excludes,
    includesNasm;

  if (!filesystem::exists(configFile))
    throwException(L"Unable to open config file: " + configFile);

  config.open(configFile);
  while (!config.eof())
  {
    line=readLine(config);
    if (line == L"[APPLICATION]")
      _type=ProjectType::Application;
    else if (line == L"[CODER]")
      _type=ProjectType::Coder;
    else if (line == L"[CODER_REFERENCES]")
      addLines(config,_coderReferences);
    else if (line == L"[DEFINES]")
      addLines(config,defines);
    else if (line == L"[DEMO]")
      _type=ProjectType::Demo;
    else if (line == L"[DYNAMIC_LIBRARY]")
      _type=ProjectType::DynamicLibrary;
    else if (line == L"[FILTER]")
      _type=ProjectType::Filter;
    else if (line == L"[FUZZ]")
      _type=ProjectType::Fuzz;
    else if (line == L"[DIRECTORY]")
      _directory=readLine(config);
    else if (line == L"[DISABLED_ARM64]")
      _disabledForArm64=true;
    else if (line == L"[DYNAMIC_DEFINES]")
      addLines(config,_dynamicDefines);
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
      _hasIncompatibleLicense=true;
    else if (line == L"[LICENSE]")
      addLines(config,_licenses);
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
    else if (!line.empty())
      throwException(L"Unknown section in config file: " + line);
  }

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

  const wstring resourceFileName=configFile.substr(0,configFile.find_last_of(L"\\") + 1) + L"ImageMagick.rc";
  if (filesystem::exists(resourceFileName))
    _resourceFileName=resourceFileName;
}

void Config::rename(const wstring& name)
{
  _name=name;
}

void Config::removeReference(const wstring& name)
{
  _references.erase(name);
}

wstring Config::readLine(wifstream &stream)
{
  wstring
    line;

  getline(stream,line);
  return(trim(line));
}

vector<wstring> Config::readLines(wifstream &config)
{
  vector<wstring>
    lines;

  while (!config.eof())
  {
    wstring line=readLine(config);
    if (line.empty())
      return(lines);

    lines.push_back(line);
  }

  return(lines);
}

void Config::updateForImageMagick6()
{
  if (_name == L"MagickCore")
    _name = L"magick";

  if (_references.erase(L"MagickCore"))
    _references.insert(L"magick");

  if (_references.erase(L"MagickWand"))
    _references.insert(L"wand");
}
