/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%  Copyright @ 1999 ImageMagick Studio LLC, a non-profit organization         %
%  dedicated to making software imaging solutions freely available.           %
%                                                                             %
%  You may not use this file except in compliance with the License.  You may  %
%  obtain a copy of the License at                                            %
%                                                                             %
%    https://www.imagemagick.org/license/                                     %
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
#include "Project.h"

class Solution
{
public:
  static void write(const Options &options,const vector<Project> &projects);

private:
  static const wstring solutionDirectory(const Project & project);

  static const wstring solutionName(const Options &options);

  static void writeConfigDirectory(wofstream& file,const Options& options);

  static void writeProjectDirectories(wofstream &file,const vector<Project>& projects);

  static void writeProjects(wofstream& file,const vector<Project>& projects);

  static void writeProjectsConfiguration(wofstream& file,const Options& options,const vector<Project>& projects);

  static void writeProjectsNesting(wofstream& file,const vector<Project>& projects);

  static void writeVisualStudioVersion(wofstream& file,const Options &options);
};
