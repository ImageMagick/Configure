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
#include "Project.h"

class Projects
{
public:
  static vector<Project> create(const Options &options,vector<Config> &configs);

  static void write(const vector<Project> &projects);

private:
  static void createCoderProjects(const Options &options,vector<Config> &configs,vector<Project> &projects);

  static void createDemoProjects(const Options &options,vector<Config> &configs,vector<Project> &projects);

  static void createFilterProjects(const Options &options,vector<Config> &configs,vector<Project> &projects);

  static void createFuzzProjects(const Options &options,vector<Config> &configs,vector<Project> &projects);

  static void createUtilitiesProjects(const Options &options,vector<Config> &configs,vector<Project> &projects);

  static void createUtilityProject(const Project &utilitiesProject,wstring name,wstring fileName,vector<Project> &projects);
};
