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
#include "Solution.h"

const wstring Solution::solutionDirectory(const Project &project)
{
  switch (project.type())
  {
    case ProjectType::Application:
      return(L"Applications");
    case ProjectType::Coder:
      return(L"Coders");
    case ProjectType::Demo:
      return(L"Demos");
    case ProjectType::Filter:
      return(L"Filters");
    case ProjectType::Fuzz:
      return(L"Fuzz");
    case ProjectType::DynamicLibrary:
    case ProjectType::StaticLibrary:
    case ProjectType::Undefined:
    default:
      return(project.directory().substr(0,project.directory().find(L"\\")));
  }
}

const wstring Solution::solutionName(const Options &options)
{
  wstring
    name(L"IM");

  name+=(options.isImageMagick7 ? L"7." : L"6.");
  name+=(options.isStaticBuild ? L"Static." : L"Dynamic.");
  name+=options.architectureName();
  name+=L".sln";

  return(name);
}

void Solution::write(const Options &options,const vector<Project> &projects)
{
  const auto solutionFileName=options.rootDirectory + solutionName(options);
  wofstream file(solutionFileName);
  if (!file)
    throwException(L"Failed to open file: " + solutionFileName);

  file << L"Microsoft Visual Studio Solution File, Format Version 12.00" << endl;
  writeVisualStudioVersion(file,options);
  writeProjects(file,projects);
  writeConfigDirectory(file,options);
  writeProjectDirectories(file,projects);
  file << L"Global" << endl;
  file << L"\tGlobalSection(SolutionConfigurationPlatforms) = preSolution" << endl;
  file << L"\t\tDebug|" << options.architectureName() << " = Debug|" << options.architectureName() << endl;
  file << L"\t\tRelease|" << options.architectureName() << " = Release|" << options.architectureName() << endl;
  file << L"\tEndGlobalSection" << endl;
  writeProjectsConfiguration(file,options,projects);
  writeProjectsNesting(file,projects);
  file << L"EndGlobal" << endl;
}

void Solution::writeConfigDirectory(wofstream &file,const Options& options)
{
  const auto binDirectory=options.binArtifactsDirectory();
  if (!filesystem::exists(binDirectory))
    return;

  file << "Project(\"{2150E333-8FDC-42A3-9474-1A3956D46DE8}\") = \"Config\", \"Config\", \"{" << createGuid(L"Config") << "}\"" << endl;
  file << "\tProjectSection(SolutionItems) = preProject" << endl;
  for (const auto& entry : filesystem::directory_iterator(binDirectory))
  {
    if (!entry.is_regular_file())
      continue;

    const auto fileName=entry.path().filename().wstring();
    if (!endsWith(fileName, L".xml"))
      continue;

    file << "\t\tArtifacts\\bin\\" << fileName << " = Artifacts\\bin\\" << fileName << endl;
  }
  file << "\tEndProjectSection" << endl;
  file << "EndProject" << endl;
}

void Solution::writeProjectDirectories(wofstream &file,const vector<Project>& projects)
{
  set<wstring> solutionDirectories;
  for (const auto& project : projects)
    solutionDirectories.insert(solutionDirectory(project));

  for (const auto& solutionDirectory : solutionDirectories)
  {
    file << "Project(\"{2150E333-8FDC-42A3-9474-1A3956D46DE8}\") = \"" << solutionDirectory << "\", \"" << solutionDirectory << "\", \"{" << createGuid(solutionDirectory) << "}\"" << endl;
    file << "EndProject" << endl;
  }
}

void Solution::writeProjects(wofstream& file,const vector<Project>& projects)
{
  for (const auto& project : projects)
  {
    file << "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"" << project.fullName() << "\", \"" << project.fileName() << "\", \"{" << project.guid() << "}\"" << endl;
    file << "EndProject" << endl;
  }
}

void Solution::writeProjectsConfiguration(wofstream& file,const Options& options,const vector<Project>& projects)
{
  file << "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution" << endl;
  for (const auto& project : projects)
  {
    file << "\t\t{" << project.guid() << L"}.Debug|" << options.architectureName() << L".ActiveCfg = Debug|" << options.platform() << endl;
    file << "\t\t{" << project.guid() << L"}.Debug|" << options.architectureName() << L".Build.0 = Debug|" << options.platform() << endl;
    file << "\t\t{" << project.guid() << L"}.Release|" << options.architectureName() << L".ActiveCfg = Release|" << options.platform() << endl;
    file << "\t\t{" << project.guid() << L"}.Release|" << options.architectureName() << L".Build.0 = Release|" << options.platform() << endl;
  }
  file << "\tEndGlobalSection" << endl;
}

void Solution::writeProjectsNesting(wofstream& file,const vector<Project>& projects)
{
  file << L"\tGlobalSection(NestedProjects) = preSolution" << endl;
  for (const auto& project : projects)
  {
    file << L"\t\t{" << project.guid() << L"} = {" << createGuid(solutionDirectory(project)) << L"}" << endl;
  }
  file << L"\tEndGlobalSection" << endl;
}

void Solution::writeVisualStudioVersion(wofstream& file,const Options &options)
{
  switch(options.visualStudioVersion)
  {
    case VisualStudioVersion::VS2026:
      file << "# Visual Studio Version 18" << endl;
      file << "VisualStudioVersion = 18.0.11205.157 d18.0" << endl;
      break;
    case VisualStudioVersion::VS2022:
      file << "# Visual Studio Version 17" << endl;
      file << "VisualStudioVersion = 17.0.31903.59" << endl;
      break;
    case VisualStudioVersion::VS2019:
      file << "# Visual Studio Version 16" << endl;
      file << "VisualStudioVersion = 16.0.28701.123" << endl;
      break;
  }
  file << "MinimumVisualStudioVersion = 10.0.40219.1" << endl;
}
