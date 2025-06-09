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
#include "Project.h"

class Solution
{
public:
  static void write(const ConfigureOptions &options,const vector<Project> &projects);

private:
  static const wstring solutionFolderGuid(const Project & project);

  static const wstring solutionName(const ConfigureOptions &options);

  static void writeConfigFolder(wofstream& file,const ConfigureOptions& options);

  static void writeProjectFolder(wofstream &file,const wstring &name);

  static void writeProjects(wofstream& file,const vector<Project>& projects);

  static void writeProjectsConfiguration(wofstream& file,const vector<Project>& projects);

  static void writeProjectsNesting(wofstream& file,const vector<Project>& projects);

  static void writeVisualStudioVersion(wofstream& file,const ConfigureOptions &options);
};

//#ifndef __Solution__
//#define __Solution__
//
//#include "Project.h"
//#include "ConfigureWizard.h"
//#include "VersionInfo.h"
//#include "WaitDialog.h"
//
//class Solution
//{
//public:
//  Solution(const ConfigureWizard &wizard);
//
//  int loadProjectFiles() const;
//
//  void loadProjects();
//
//  void write(WaitDialog &waitDialog) const;
//
//private:
//
//  void addConfigFolder(wofstream &file) const;
//
//  void addNestedProjects(wofstream &file,const wstring &name,const wstring &prefix) const;
//
//  void addProjects(wofstream &file,const wstring &prefix) const;
//
//  void addSolutionFolder(wofstream &file,const wstring &name,const wstring &prefix) const;
//
//  void checkKeyword(const wstring keyword) const;
//
//  void createConfigFiles() const;
//
//  const wstring getFileName() const;
//
//  void loadProjectsFromFolder(const wstring &folder,const wstring &filesFolder);
//
//  void replaceVersionVariables(const VersionInfo &versionInfo,wifstream &input,wofstream &output) const;
//
//  void writeInstallerConfig(const VersionInfo &versionInfo) const;
//
//  void writeMagickBaseConfig() const;
//
//  void writeMakeFile() const;
//
//  void writeNotice(const VersionInfo &versionInfo) const;
//
//  void writeThresholdMap() const;
//
//  void writeVersion(const VersionInfo &versionInfo) const;
//
//  void writeVersion(const VersionInfo &versionInfo,const wstring &input,const wstring &output) const;
//
//  void write(wofstream &file) const;
//
//  vector<Project*> _projects;
//  const ConfigureWizard &_wizard;
//};
//
//#endif // __Solution__
