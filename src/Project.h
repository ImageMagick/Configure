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
#include "Options.h"

class Project
{
public:
  const wstring directory() const { return(_config.directory()); };

  const wstring fileName() const { return(_options.projectsDirectory() + fullName() + L"\\" + fullName() + L".vcxproj"); }

  const wstring fullName() const { return(prefix() + L"_" + name()); }

  const wstring guid() const { return(createGuid(fullName())); };

  const bool isLibrary() const { return(_config.isLibrary()); };

  const wstring name() const { return(_config.name()); };

  const ProjectType type() const { return(_config.type()); };

  void copyConfigInfo(const Config& config);

  static Project create(const Config &config,const Options &options);

  const set<wstring>& references() const { return(_config.references()); };

  void rename(const wstring& name);

  void setFiles(const vector<wstring> files);

  vector<Project> splitToFiles(const vector<wstring> additionalFiles = {}) const;

  void write(const vector<Project> &allProjects) const;

  void writeFilters() const;

  void writeLicense() const;

  void writeMagickBaseconfigDefine() const;

private:
  Project(const Config &config,const Options &options);

  const wstring characterSet() const;

  const Compiler compiler() const;

  const wstring configurationType() const;

  const wstring defines() const;

  const bool hasAsmfiles() const;

  const wstring includeDirectories() const;

  const bool isApplication() const;

  const wstring nasmOptions() const;

  const wstring openMPSupport() const;

  const wstring outputDirectory() const;

  const wstring platformToolset() const;

  const wstring prefix() const;

  const wstring warningLevel() const;

  const wstring additionalDependencies(bool debug) const;

  bool isExcluded(const wstring fileName,set<wstring> &excludes,multiset<wstring> &foundExcludes) const;

  void loadFiles();

  void loadFiles(const wstring directory,set<wstring> &excludes,multiset<wstring> &foundExcludes);

  const wstring runtimeLibrary(bool debug) const;

  const wstring targetName(bool debug) const;

  void writeCompilationConfiguration(wofstream &file) const;

  void writeConfiguration(wofstream &file) const;

  void writeCopyIncludes(wofstream &file) const;
  
  void writeFiles(wofstream &file) const;

  void writeLibProperties(wofstream& file) const;

  void writeLinkProperties(wofstream& file) const;

  void writeOutputProperties(wofstream &file) const;

  void writeProperties(wofstream &file) const;

  void writePropsImports(wofstream& file,bool includeMasm) const;

  void writeReference(wofstream &file,const Project &project) const;

  void writeReferences(wofstream &file,const vector<Project> &allProjects) const;

  void writeTargetsImports(wofstream& file,bool includeMasm) const;

  Config _config;
  set<wstring> _files;
  const Options _options;
};
