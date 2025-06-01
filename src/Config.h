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
#include "stdafx.h"

#include "ConfigureOptions.h"

class Config
{
public:
  const std::set<std::wstring> defines(const ProjectType projectType) const;

  const std::wstring directory() const { return(_directory); }
  
  const std::map<std::wstring, std::wstring> includeArtifacts() const { return(_includeArtifacts); }

  const std::set<std::wstring> excludes(const Architecture architecture) const;

  const std::wstring name() const { return(_name); }

  const std::set<std::wstring> includes() const { return(_includes); }

  const bool isMagickProject() const { return(_isMagickProject); }

  const bool isOptional() const { return(_isOptional); }

  const std::wstring moduleDefinitionFile() const { return(_moduleDefinitionFile); }
  
  const std::set<std::wstring> Config::nasmIncludes(const Architecture architecture) const;

  const std::set<std::wstring>& references() const { return(_references); }

  const ProjectType type() const { return(_type); }

  const bool useNasm() const { return(_useNasm); }

  const bool useUnicode() const { return(_useUnicode); }

  const bool isImageMagick7Only() const { return(_isImageMagick7Only); }

  static Config load(const std::wstring name,const std::wstring &directory,const std::wstring &configFile);

private:
  Config(const std::wstring &name,const std::wstring &directory);

  void addIncludeArtifacts(std::wifstream &config);

  static void addIncludes(std::wifstream &config,std::set<std::wstring> &container);

  static void addLines(std::wifstream &config,std::wstring &value);

  static void addLines(std::wifstream &config,std::set<std::wstring> &container);

  void load(const std::wstring &configFile);

  static std::wstring readLine(std::wifstream &stream);

  static std::vector<std::wstring> readLines(std::wifstream &config);

  bool _disabledArm64;
  std::set<std::wstring> _dynamicDefines;
  std::set<std::wstring> _excludesArm64;
  std::set<std::wstring> _excludesX64;
  std::set<std::wstring> _excludesX86;
  std::set<std::wstring> _includes;
  std::set<std::wstring> _includesNasm;
  std::set<std::wstring> _includesNasmX64;
  std::set<std::wstring> _includesNasmX86;
  std::map<std::wstring,std::wstring> _includeArtifacts;
  bool _incompatibleLicense;
  bool _isImageMagick7Only;
  bool _isMagickProject;
  bool _isOptional;
  std::set<std::wstring> _licenses;
  std::set<std::wstring> _linkedLibs;
  std::wstring _magickBaseconfigDefine;
  std::wstring _moduleFileName;
  std::wstring _moduleDefinitionFile;
  std::wstring _name;
  std::wstring _directory;
  std::set<std::wstring> _references;
  std::set<std::wstring> _staticDefines;
  ProjectType _type;
  bool _useNasm;
  bool _useOpenCL;
  bool _useUnicode;
};
