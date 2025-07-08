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
#include "Solution.h"

const wstring Solution::solutionFolder(const Project &project)
{
  switch (project.type())
  {
    case ProjectType::Application: return(L"Applications");
    case ProjectType::Coder: return(L"Coders");
    case ProjectType::Demo: return(L"Demos");
    case ProjectType::Filter: return(L"Filters");
    case ProjectType::Fuzz: return(L"Fuzz");
    default: return(project.directory().substr(0,project.directory().find(L"\\")));
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
  wstring solutionFileName = options.rootDirectory + solutionName(options);
  wofstream file(solutionFileName);
  if (!file)
    throwException(L"Failed to open file: " + solutionFileName);

  set<wstring> solutionFolders;
  file << L"Microsoft Visual Studio Solution File, Format Version 12.00" << endl;
  writeVisualStudioVersion(file,options);
  writeProjects(file,projects);
  writeConfigFolder(file,options);
  writeProjectFolders(file,projects);
  file << L"Global" << endl;
  file << L"\tGlobalSection(SolutionConfigurationPlatforms) = preSolution" << endl;
  file << L"\t\tDebug|" << options.architectureName() << " = Debug|" << options.architectureName() << endl;
  file << L"\t\tRelease|" << options.architectureName() << " = Release|" << options.architectureName() << endl;
  file << L"\tEndGlobalSection" << endl;
  writeProjectsConfiguration(file,options,projects);
  writeProjectsNesting(file,projects);
  file << L"EndGlobal" << endl;
}

void Solution::writeConfigFolder(wofstream &file,const Options& options)
{
  file << "Project(\"{2150E333-8FDC-42A3-9474-1A3956D46DE8}\") = \"Config\", \"Config\", \"{" << createGuid(L"Config") << "}\"" << endl;
  file << "\tProjectSection(SolutionItems) = preProject" << endl;
  for (const auto& entry : filesystem::directory_iterator(options.rootDirectory + L"Artifacts\\bin"))
  {
    if (!entry.is_regular_file())
      continue;

    wstring fileName=entry.path().filename();
    if (!endsWith(fileName, L".xml"))
      continue;

    file << "\t\tArtifacts\\bin\\" << fileName << " = Artifacts\\bin\\" << fileName << endl;
  }
  file << "\tEndProjectSection" << endl;
  file << "EndProject" << endl;
}

void Solution::writeProjectFolders(wofstream &file,const vector<Project>& projects)
{
  set<wstring> solutionFolders;
  for (const auto& project : projects)
    solutionFolders.insert(solutionFolder(project));

  for (const auto& solutionFolder : solutionFolders)
  {
    file << "Project(\"{2150E333-8FDC-42A3-9474-1A3956D46DE8}\") = \"" << solutionFolder << "\", \"" << solutionFolder << "\", \"{" << createGuid(solutionFolder) << "}\"" << endl;
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
    file << L"\t\t{" << project.guid() << L"} = {" << createGuid(solutionFolder(project)) << L"}" << endl;
  }
  file << L"\tEndGlobalSection" << endl;
}

void Solution::writeVisualStudioVersion(wofstream& file,const Options &options)
{
  switch(options.visualStudioVersion)
  {
    case VisualStudioVersion::VS2022:
      file << "# Visual Studio Version 17" << endl;
      file << "VisualStudioVersion = 17.0.31903.59" << endl;
      break;
    case VisualStudioVersion::VS2019:
      file << "# Visual Studio Version 16" << endl;
      file << "VisualStudioVersion = 16.0.28701.123" << endl;
      break;
    case VisualStudioVersion::VS2017:
      file << "# Visual Studio Version 15" << endl;
      file << "VisualStudioVersion = 15.0.26124.0" << endl;
      break;
  }
  file << "MinimumVisualStudioVersion = 10.0.40219.1" << endl;
}
