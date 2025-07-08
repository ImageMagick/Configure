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

vector<Project> Projects::create(const Options &options,vector<Config> &configs)
{
  vector<Project>
    projects;

  for (auto& config : configs)
  {
    if (config.type() == ProjectType::Coder || 
        config.type() == ProjectType::Demo ||
        config.type() == ProjectType::Filter || 
        config.type() == ProjectType::Fuzz ||
        config.name() == L"utilities")
      continue;

    projects.push_back(Project::create(config,options));
  }

  createCoderProjects(options,configs,projects);
  createDemoProjects(options,configs,projects);
  createFilterProjects(options,configs,projects);
  createFuzzProjects(options,configs,projects);
  createUtilitiesProjects(options,configs,projects);

  return(projects);
}

void Projects::createCoderProjects(const Options &options,vector<Config> &configs,vector<Project> &projects)
{
  auto codersConfig=find_if(configs.begin(),configs.end(),[&](const auto &config) { return (config.name() == L"coders"); });
  if (codersConfig == configs.end())
    return;

  set<wstring> allNames;
  for (const auto& project : projects)
    allNames.insert(project.name());

  Project codersProject=Project::create(*codersConfig,options);
  
  if (options.isStaticBuild)
  {
    for (const auto& config : configs)
    {
      if (config.type() == ProjectType::Coder && config.name() != L"coders")
        codersProject.copyConfigInfo(config);
    }

    projects.push_back(codersProject);
  }
  else
  {
    for (auto& coderProject : codersProject.splitToFiles())
    {
      auto coderConfig=find_if(configs.begin(),configs.end(),[&](const auto &config) { return (config.type() == ProjectType::Coder && config.name() == coderProject.name()); });
      if (coderConfig != configs.end())
        coderProject.copyConfigInfo(coderConfig[0]);

      projects.push_back(coderProject);
    }
  }
}

void Projects::createDemoProjects(const Options &options,vector<Config> &configs,vector<Project> &projects)
{
  auto demoConfig=find_if(configs.begin(),configs.end(),[&](const auto &config) { return (config.type() == ProjectType::Demo); });
  if (demoConfig == configs.end())
    return;

  Project demoProject=Project::create(*demoConfig,options);
  for (const auto& project : demoProject.splitToFiles())
    projects.push_back(project);
}

void Projects::createFilterProjects(const Options &options,vector<Config> &configs,vector<Project> &projects)
{
  auto filtersConfig=find_if(configs.begin(),configs.end(),[&](const auto &config) { return (config.name() == L"filters"); });
  if (filtersConfig == configs.end())
    return;

  Project filtersProject=Project::create(*filtersConfig,options);
  
  if (options.isStaticBuild)
  {
    projects.push_back(filtersProject);
  }
  else
  {
    for (const auto& filterProject : filtersProject.splitToFiles())
      projects.push_back(filterProject);
  }
}

void Projects::createFuzzProjects(const Options &options,vector<Config> &configs,vector<Project> &projects)
{
  auto fuzzConfig=find_if(configs.begin(),configs.end(),[&](const auto &config) { return (config.type() == ProjectType::Fuzz); });
  if (fuzzConfig == configs.end())
    return;

  Project fuzzProject=Project::create(*fuzzConfig,options);
  for (const auto& project : fuzzProject.splitToFiles({ L"main.cc" }))
    projects.push_back(project);
}

void Projects::createUtilitiesProjects(const Options &options,vector<Config> &configs,vector<Project> &projects)
{
  auto utilitiesConfig=find_if(configs.begin(),configs.end(),[&](const auto &config) { return (config.name() == L"utilities"); });
  if (utilitiesConfig == configs.end())
    return;

  Project utilitiesProject=Project::create(*utilitiesConfig,options);

  vector<wstring> aliases = { L"compare", L"composite", L"conjure", L"identify", L"mogrify", L"montage", L"stream" };

  for (const auto& alias : aliases)
  {
    if (options.isImageMagick7)
      createUtilityProject(utilitiesProject,alias,L"magick",projects);
    else
      createUtilityProject(utilitiesProject,alias,alias,projects);
  }

  if (options.isImageMagick7)
    createUtilityProject(utilitiesProject,L"magick",L"magick",projects);
  else
    createUtilityProject(utilitiesProject,L"convert",L"convert",projects);
}

void Projects::createUtilityProject(const Project &utilitiesProject,wstring name,wstring fileName,vector<Project> &projects)
{
  Project utilityProject(utilitiesProject);
  utilityProject.rename(name);
  utilityProject.setFiles({ fileName + L".c" });

  projects.push_back(utilityProject);
}

void Projects::write(const vector<Project> &projects)
{
  for (const auto& project : projects)
  {
    project.write(projects);
    project.writeFilters();
    project.writeLicense();
    project.writeMagickBaseconfigDefine();
  }
}
