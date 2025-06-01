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

#include "Config.h"
#include "ConfigureOptions.h"

class Project
{
public:
  const std::wstring fileName() const { return(_options->projectsDirectory() + fullName() + L"\\" + fullName() + L".vcxproj"); }

  const std::wstring fullName() const { return(prefix() + L"_" + name()); }

  const std::wstring guid() const { return(_guid); };

  const std::wstring name() const { return(_config->name()); };

  const ProjectType type() const { return(_type); };

  static Project create(const Config &config,const ConfigureOptions &options);

  void write(const std::vector<Project> &allProjects) const;

  void writeFilters() const;

private:
  Project(const std::wstring guid,const Config &config,const ConfigureOptions &options);

  const std::wstring additionalDependencies() const;

  const std::wstring characterSet() const;

  const std::wstring configurationType() const;

  const std::wstring defines() const;

  const bool hasAsmfiles() const;

  const std::wstring includeDirectories() const;

  const std::wstring openMPSupport() const;

  const std::wstring outputDirectory() const;

  const std::wstring platformToolset() const;

  const std::wstring prefix() const;

  const std::wstring warningLevel() const;

  bool isExcluded(const std::wstring fileName,std::set<std::wstring> &excludes,std::multiset<std::wstring> &foundExcludes) const;

  static const std::wstring libraryName(const std::wstring &project,bool debug);

  void loadFiles();

  void loadFiles(const std::wstring directory,std::set<std::wstring> &excludes,std::multiset<std::wstring> &foundExcludes);

  const std::wstring nasmOptions() const;

  const std::wstring runtimeLibrary(bool debug) const;

  const std::wstring targetName(bool debug) const;

  void writeCompilationConfiguration(std::wofstream &file) const;

  void writeConfiguration(std::wofstream &file) const;

  void writeCopyIncludes(std::wofstream &file) const;
  
  void writeFiles(std::wofstream &file) const;

  void writeLibProperties(std::wofstream& file) const;

  void writeLinkProperties(std::wofstream& file) const;

  void writeOutputProperties(std::wofstream &file) const;

  void writeProperties(std::wofstream &file) const;

  void writePropsImports(std::wofstream& file,bool includeMasm) const;

  void writeReferences(std::wofstream &file,const std::vector<Project> &allProjects) const;

  void writeTargetsImports(std::wofstream& file,bool includeMasm) const;

  const std::wstring _guid;
  const Config* _config;
  const ConfigureOptions* _options;
  std::set<std::wstring> _files;
  ProjectType _type;
};

//#ifndef __Project__
//#define __Project__
//
//#include "ConfigureWizard.h"
//#include "ProjectFile.h"
//#include "Shared.h"
//
//class Project
//{
//public:
//  Compiler compiler() const;
//
//  const wstring configDefine() const;
//
//  const vector<wstring> &defines();
//
//  const vector<wstring> &definesDll();
//
//  const vector<wstring> &definesLib();
//
//  const vector<wstring> &dependencies();
//
//  const vector<wstring> &directories();
//
//  const vector<wstring> &excludes();
//
//  const vector<ProjectFile*> &files() const;
//
//  const vector<wstring> &includes();
//
//  const vector<wstring> &includesNasm();
//
//  const vector<wstring> &platformExcludes(Platform platform);
//
//  const wstring configPath(const wstring &subPath) const;
//
//  const wstring filePath(const wstring &subPath) const;
//
//  bool isConsole() const;
//
//  bool isDll() const;
//
//  bool isExe() const;
//
//  bool isFuzz() const;
//
//  bool isLib() const;
//
//  bool isModule() const;
//
//  bool isOptimizationDisable() const;
//
//  bool isSupported(const VisualStudioVersion visualStudioVersion) const;
//
//  const vector<wstring> &libraries();
//
//  const wstring moduleDefinitionFile() const;
//
//  const wstring name() const;
//
//  const wstring notice() const;
//
//  const vector<wstring> &references();
//
//  bool treatWarningAsError() const;
//
//  bool useNasm() const;
//
//  bool useOpenCL() const;
//
//  bool useUnicode() const;
//
//  const wstring version() const;
//
//  int warningLevel() const;
//
//  void checkFiles(const VisualStudioVersion visualStudioVersion);
//
//  static Project* create(const ConfigureWizard &wizard,const wstring &configFolder,const wstring &filesFolder,const wstring& name);
//
//  bool loadFiles();
//
//  void mergeProjectFiles();
//
//  bool shouldSkip() const;
//
//  void updateProjectNames();
//
//  void updateProjectNames(vector<wstring> &vector);
//
//private:
//  Project(const ConfigureWizard &wizard,const wstring &configFolder,const wstring &filesFolder,const wstring &name);
//
//  void addLines(wifstream &config,wstring &value);
//
//  void addLines(wifstream &config,vector<wstring> &container);
//
//  void loadConfig(wifstream &config);
//
//  void loadModules();
//
//  const vector<wstring> readLicenseFilenames(const wstring &line) const;
//
//  void setNoticeAndVersion();
//
//  wstring               _configDefine;
//  wstring               _configFolder;
//  vector<wstring>       _defines;
//  vector<wstring>       _definesDll;
//  vector<wstring>       _definesLib;
//  vector<wstring>       _dependencies;
//  vector<wstring>       _directories;
//  bool                  _disabledARM64;
//  bool                  _disableOptimization;
//  vector<wstring>       _excludes;
//  vector<wstring>       _excludesX86;
//  vector<wstring>       _excludesX64;
//  vector<wstring>       _excludesARM64;
//  vector<ProjectFile*>  _files;
//  wstring               _filesFolder;
//  bool                  _hasIncompatibleLicense;
//  vector<wstring>       _includes;
//  vector<wstring>       _includesNasm;
//  bool                  _isOptional;
//  vector<wstring>       _libraries;
//  vector<wstring>       _licenseFileNames;
//  bool                  _magickProject;
//  VisualStudioVersion   _minimumVisualStudioVersion;
//  wstring               _moduleDefinitionFile;
//  wstring               _modulePrefix;
//  wstring               _name;
//  wstring               _notice;
//  bool                  _onlyImageMagick7;
//  wstring               _path;
//  vector<wstring>       _references;
//  ProjectType           _type;
//  bool                  _useNasm;
//  bool                  _useOpenCL;
//  bool                  _useUnicode;
//  vector<wstring>       _versions;
//  const ConfigureWizard &_wizard;
//};
//
//#endif // __Project__
