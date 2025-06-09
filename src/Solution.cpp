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

const wstring Solution::solutionFolderGuid(const Project &project)
{
  if (project.type() == ProjectType::Application)
    return(createGuid(L"Applications"));

  if (project.type() == ProjectType::Fuzz)
    return(createGuid(L"Fuzz"));

  if (project.type() == ProjectType::Coder)
    return(createGuid(L"Coders"));

  return(createGuid(project.directory().substr(0,project.directory().find(L"\\"))));
}

const wstring Solution::solutionName(const ConfigureOptions &options)
{
  wstring
    name(L"IM");

  name+=(options.version == ImageMagickVersion::ImageMagick7 ? L"7." : L"6.");
  name+=(options.buildType == BuildType::Dynamic ? L"Dynamic." : L"Static.");
  name+=options.architectureName();
  name+=L".sln";

  return(name);
}

void Solution::write(const ConfigureOptions &options,const vector<Project> &projects)
{
  wstring solutionFileName = options.rootDirectory + solutionName(options);
  wofstream file(solutionFileName);
  if (!file)
    throwException(L"Failed to open file: " + solutionFileName);

  file << L"Microsoft Visual Studio Solution File, Format Version 12.00" << endl;
  writeVisualStudioVersion(file,options);
  writeProjects(file,projects);
  writeConfigFolder(file, options); 
  writeProjectFolder(file,L"Applications");
  writeProjectFolder(file,L"Coders");
  writeProjectFolder(file,L"Dependencies");
  writeProjectFolder(file,L"Fuzz");
  writeProjectFolder(file,L"OptionalDependencies");
  writeProjectFolder(file,L"ImageMagick");
  file << L"Global" << endl;
  file << L"  GlobalSection(SolutionConfigurationPlatforms) = preSolution" << endl;
  file << L"    Debug|" << options.architectureName() << " = Debug|" << options.architectureName() << endl;
  file << L"    Release|" << options.architectureName() << " = Release|" << options.architectureName() << endl;
  file << L"  EndGlobalSection" << endl;
  writeProjectsConfiguration(file,projects);
  writeProjectsNesting(file,projects);
  file << L"EndGlobal" << endl;
}

void Solution::writeConfigFolder(wofstream &file,const ConfigureOptions& options)
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

void Solution::writeProjectFolder(wofstream &file,const wstring &name)
{
  file << "Project(\"{2150E333-8FDC-42A3-9474-1A3956D46DE8}\") = \"" << name << "\", \"" << name << "\", \"{" << createGuid(name) << "}\"" << endl;
  file << "EndProject" << endl;
}

void Solution::writeProjects(wofstream& file,const vector<Project>& projects)
{
  for (const auto& project : projects)
  {
    file << "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"" << project.fullName() << "\", \"" << project.fileName() << "\", \"{" << project.guid() << "}\"" << endl;
    file << "EndProject" << endl;
  }
}

void Solution::writeProjectsConfiguration(wofstream& file,const vector<Project>& projects)
{  
  file << "  GlobalSection(ProjectConfigurationPlatforms) = postSolution" << endl;
  for (const auto& project : projects)
  {
    file << "    {" << project.guid() << L"}.Debug|x64.ActiveCfg = Debug|x64" << endl;
    file << "    {" << project.guid() << L"}.Debug|x64.Build.0 = Debug|x64" << endl;
    file << "    {" << project.guid() << L"}.Release|x64.ActiveCfg = Release|x64" << endl;
    file << "    {" << project.guid() << L"}.Release|x64.Build.0 = Release|x64" << endl;
  }
  file << "  EndGlobalSection" << endl;
}

void Solution::writeProjectsNesting(wofstream& file,const vector<Project>& projects)
{
  file << L"  GlobalSection(NestedProjects) = preSolution" << endl;
  for (const auto& project : projects)
  {
    file << L"    {" << project.guid() << L"} = {" << solutionFolderGuid(project) << L"}" << endl;
  }
  file << L"  EndGlobalSection" << endl;
}

void Solution::writeVisualStudioVersion(wofstream& file,const ConfigureOptions &options)
{
  switch(options.visualStudioVersion)
  {
    case VisualStudioVersion::VS2022:
      file << "# Visual Studio 17" << endl;
      file << "VisualStudioVersion = 17.0.31903.59" << endl;
      break;
    case VisualStudioVersion::VS2019:
      file << "# Visual Studio 16" << endl;
      file << "VisualStudioVersion = 16.0.28701.123" << endl;
      break;
    case VisualStudioVersion::VS2017:
      file << "# Visual Studio 15" << endl;
      file << "VisualStudioVersion = 15.0.26124.0" << endl;
      break;
  }
  file << "MinimumVisualStudioVersion = 10.0.40219.1" << endl;
}
