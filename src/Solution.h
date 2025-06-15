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
  static const wstring solutionFolder(const Project & project);

  static const wstring solutionName(const ConfigureOptions &options);

  static void writeConfigFolder(wofstream& file,const ConfigureOptions& options);

  static void writeProjectFolders(wofstream &file,const vector<Project>& projects);

  static void writeProjects(wofstream& file,const vector<Project>& projects);

  static void writeProjectsConfiguration(wofstream& file,const ConfigureOptions& options,const vector<Project>& projects);

  static void writeProjectsNesting(wofstream& file,const vector<Project>& projects);

  static void writeVisualStudioVersion(wofstream& file,const ConfigureOptions &options);
};
