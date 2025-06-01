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
#include "Project.h"

class Projects
{
public:
  static std::vector<Project> create(const std::vector<Config> &configs,const ConfigureOptions &options);

  static void write(const std::vector<Project> &projects);

//private:
//

  //static std::wstring& projectName(const std::wstring name,const ProjectType type);
//
//  //static void addCoderProjects(const ConfigFile &configFile,const ConfigureOptions &options,std::vector<ProjectFile> &projectFiles);
//
//  static std::wstring createProjectsFolder(const std::wstring &rootFolder,const ConfigureOptions &options);
//
//  //static void writeLibrary(const std::wstring projectsFolder,ConfigFile configFile,std::vector<ConfigFile> configFiles,const ConfigureOptions options);
//
//  //static void writeProjectStart();
//};
};
