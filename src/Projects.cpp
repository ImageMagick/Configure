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
#include "Projects.h"

std::vector<Project> Projects::create(const std::vector<Config> &configs,const ConfigureOptions &options)
{
  std::vector<Project>
    projects;

  for (const auto& config : configs)
    projects.push_back(Project::create(config,options));

  return(projects);
}

void Projects::write(const std::vector<Project> &projects)
{
  for (const auto& project : projects)
  {
    project.write(projects);
    project.writeFilters();
  }
}

//void ProjectFiles::write(const std::wstring &rootFolder,std::vector<Config> &configs,const ConfigureOptions &options)
//{
//  std::wstring
//    projectFolder;
//
//  std::vector<ProjectFile>
//    projectFiles;
//
//  if (configs.empty())
//  {
//    throwException(L"No configurations found in " + rootFolder);
//  }
//
//  projectFolder=createProjectsFolder(rootFolder,options);
//
//  for (const auto& config : configs)
//  {
//    switch (config.type())
//    {
//    case ProjectType::CODER:
//      //addCoderProjects(configFile,options,projectFiles);
//      break;
//   /* case ProjectType::LIBRARY:
//    case ProjectType::STATIC_LIBRARY:
//      projectFiles.push_back(ProjectFile(configFile.name(),configFile));
//      break;*/
//    default:
//      throwException(L"Unsupported project type: " + std::to_wstring(static_cast<int>(config.type())));
//    }
//  }
//}

//void ProjectFiles::addCoderProjects(const ConfigFile &configFile,const ConfigureOptions &options,std::vector<ProjectFile> &projectFiles)
//{
// /* if (options.buildType == BuildType::STATIC)
//  {
//    projectFiles.push_back(ProjectFile(configFile.name(), configFile));
//  }*/
//}

//std::wstring ProjectFiles::createProjectsFolder(const std::wstring &rootFolder,const ConfigureOptions &options)
//{
//  std::wstring
//    projectFolder;
//
//  projectFolder=rootFolder + L"\\ProjectFiles";
//  if (options.buildType == BuildType::STATIC)
//    projectFolder += L"\\Static";
//  else
//    projectFolder += L"\\Dynamic";
//
//  if (!std::filesystem::exists(projectFolder))
//    std::filesystem::create_directories(projectFolder);
//
//  return(projectFolder);
//}

//void ProjectFiles::writeLibrary(const std::wstring projectsFolder,ConfigFile configFile,std::vector<ConfigFile> configFiles,const ConfigureOptions options)
//{
//  std::wofstream
//    file;
//
//  file=std::wofstream(projectsFolder + L"\\" + configFile.name() + L".vcxproj");
//}
