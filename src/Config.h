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

#include "Options.h"

class Config
{
public:
  const set<wstring>& coderReferences() const { return(_coderReferences); }

  const wstring directory() const { return(_directory); }

  const bool disabledForArm64() const { return(_disabledForArm64); }

  const set<wstring>& dynamicDefines() const { return(_dynamicDefines); }
  
  const map<wstring, wstring>& includeArtifacts() const { return(_includeArtifacts); }

  const set<wstring>& excludes(const Architecture architecture) const;

  const wstring name() const { return(_name); }

  const set<wstring>& includes() const { return(_includes); }

  const bool hasIncompatibleLicense() const { return(_hasIncompatibleLicense); }

  const bool isImageMagick7Only() const { return(_isImageMagick7Only); }

  const bool isLibrary() const { return(_type == ProjectType::DynamicLibrary || _type == ProjectType::StaticLibrary); }

  const bool isMagickProject() const { return(_isMagickProject); }

  const bool isOptional() const { return(_isOptional); }

  const set<wstring>& licenses() const { return(_licenses); }

  const wstring magickBaseconfigDefine() const { return(_magickBaseconfigDefine); }

  const wstring moduleDefinitionFile() const { return(_moduleDefinitionFile); }
  
  const set<wstring>& Config::nasmIncludes(const Architecture architecture) const;

  const bool useNasm() const { return(_useNasm); }

  const bool useOpenCL() const { return(_useOpenCL); }

  const bool useUnicode() const { return(_useUnicode); }

  const set<wstring>& references() const { return(_references); }

  const wstring resourceFileName() const { return(_resourceFileName); }

  const set<wstring>& staticDefines() const { return(_staticDefines); }

  const ProjectType type() const { return(_type); }

  const Config copyInfo(const Config &config) const;

  static Config load(const wstring name,const wstring &directory,const wstring &configFile);

  void rename(const wstring& name);

  void removeReference(const wstring& name);

  void updateForImageMagick6();

private:
  Config(const wstring &name,const wstring &directory);

  void addIncludeArtifacts(wifstream &config);

  static void addIncludes(wifstream &config,set<wstring> &container);

  static void addLines(wifstream &config,wstring &value);

  static void addLines(wifstream &config,set<wstring> &container);

  void load(const wstring &configFile);

  static wstring readLine(wifstream &stream);

  static vector<wstring> readLines(wifstream &config);
  
  set<wstring> _coderReferences;
  bool _disabledForArm64;
  set<wstring> _dynamicDefines;
  set<wstring> _excludesArm64;
  set<wstring> _excludesX64;
  set<wstring> _excludesX86;
  bool _hasIncompatibleLicense;
  set<wstring> _includes;
  set<wstring> _includesNasm;
  set<wstring> _includesNasmX64;
  set<wstring> _includesNasmX86;
  map<wstring,wstring> _includeArtifacts;
  bool _isImageMagick7Only;
  bool _isMagickProject;
  bool _isOptional;
  set<wstring> _licenses;
  wstring _magickBaseconfigDefine;
  wstring _moduleFileName;
  wstring _moduleDefinitionFile;
  wstring _name;
  wstring _directory;
  set<wstring> _references;
  wstring _resourceFileName;
  set<wstring> _staticDefines;
  ProjectType _type;
  bool _useNasm;
  bool _useOpenCL;
  bool _useUnicode;
};
