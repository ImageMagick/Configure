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
#include "Project.h"

Project::Project(const std::wstring guid,const Config &config,const ConfigureOptions &options)
  : 
    _guid(guid),
    _config(&config),
    _options(&options)
{  
  _type=_config->type();
  if (_type == ProjectType::DynamicLibrary && options.buildType == BuildType::Static)
    _type=ProjectType::StaticLibrary;
}

const std::wstring Project::additionalDependencies() const
{
  std::wstring dependencies;
  for (auto& reference : _config->references())
    dependencies+=libraryName(reference,true) + L".lib;";
  return(dependencies);
}

const std::wstring Project::characterSet() const
{
  return(_config->useUnicode() ? L"Unicode" : L"MultiByte");
}

const std::wstring Project::configurationType() const
{
  switch(_type)
  {
  case ProjectType::DynamicLibrary: return(L"DynamicLibrary");
  case ProjectType::StaticLibrary: return(L"StaticLibrary");
  default: throwException(L"Unsupported project type");
  }
}

const std::wstring Project::defines() const
{
  std::wstring defines=L"_WIN32_WINNT=0x0601";
  for (auto& define : _config->defines(_type))
  {
    defines+=L";" + define;
  }

  return(defines);
}

const bool Project::hasAsmfiles() const
{
  for (const auto& file : _files)
  {
    if (endsWith(file, L".asm"))
      return(true);
  }

  return(false);
}

const std::wstring Project::includeDirectories() const
{
  std::wstring directories;

  if (_config->includes().empty())
    directories=L"$(SolutionDir)" + _config->directory() + L";";

  for (const auto& include : _config->includes())
    directories+=L"$(SolutionDir)" + _config->directory() + include + L";";

  for (const auto& reference : _config->references())
    directories+=L"$(SolutionDir)Artifacts\\include\\" + reference + L";";

  return(directories);
}

const std::wstring Project::openMPSupport() const
{
  return(_options->useOpenMP ? L"true" : L"false");
}

const std::wstring Project::outputDirectory() const
{
  switch(_type)
  {
  case ProjectType::DynamicLibrary: return(L"bin");
  case ProjectType::StaticLibrary: return(L"lib");
  default: throwException(L"Unsupported project type");
  }
}

const std::wstring Project::platformToolset() const
{
  switch (_options->visualStudioVersion)
  {
  case VisualStudioVersion::VS2022: return(L"v143");
  case VisualStudioVersion::VS2019: return(L"v142");
  case VisualStudioVersion::VS2017: return(L"v141");
  default: throwException(L"Unknown architecture");
  }
}

const std::wstring Project::prefix() const
{
  switch(_type)
  {
  case ProjectType::DynamicLibrary:
  case ProjectType::StaticLibrary: 
    return(L"CORE");
  default:
    throwException(L"Unsupported project type");
  }
}

const std::wstring Project::warningLevel() const
{
  if (_options->version == ImageMagickVersion::ImageMagick7 && _config->isMagickProject())
    return(L"Level4");
  else
    return(L"TurnOffAllWarnings");
}

Project Project::create(const Config &config,const ConfigureOptions &options)
{
  const std::wstring guid=createGuid(config.name());

  Project project(guid,config,options);
  project.loadFiles();

  return(project);
}

bool Project::isExcluded(const std::wstring fileName,std::set<std::wstring> &excludes,std::multiset<std::wstring> &foundExcludes) const
{
  if (startsWith(fileName,L".git\\") || startsWith(fileName,L".github\\") || startsWith(fileName,L".ImageMagick\\"))
    return(true);

  if (endsWith(fileName,L".h"))
  {
    size_t lastDot=fileName.find_last_of(L'.');

    if (isExcluded(fileName.substr(0,lastDot) + L".c",excludes,foundExcludes))
      return(true);

    if (isExcluded(fileName.substr(0,lastDot) + L".cc",excludes,foundExcludes))
      return(true);

    if (isExcluded(fileName.substr(0,lastDot) + L".cpp",excludes,foundExcludes))
      return(true);

    return(false);
  }

  for (const auto& exclude : excludes)
  {
    if (startsWith(fileName,exclude))
    {
      foundExcludes.insert(exclude);
      return(true);
    }
  }

  return(false);
}

const std::wstring Project::libraryName(const std::wstring &project,bool debug)
{
  return((debug ? L"DB_" : L"RL_") + project);
}

void Project::loadFiles()
{
  std::multiset<std::wstring> foundExcludes;
  std::set<std::wstring> excludes(_config->excludes(_options->architecture));

  loadFiles(L"",excludes,foundExcludes);

  for (const auto& exclude : excludes)
  {
    if (foundExcludes.find(exclude) == foundExcludes.end())
      throwException(L"Invalid exclude path " + exclude + L" in " + name());
  }
}

void Project::loadFiles(const std::wstring directory,std::set<std::wstring> &excludes,std::multiset<std::wstring> &foundExcludes)
{
  if (isExcluded(directory + L"\\",excludes,foundExcludes))
    return;

  for (const auto& file : std::filesystem::directory_iterator(_options->rootDirectory + _config->directory() + directory))
  {
    std::wstring name=directory;
    if (!directory.empty())
      name+=L"\\";
    name+=file.path().filename().wstring();

    if (file.is_directory())
    {
      loadFiles(name,excludes,foundExcludes);
      continue;
    }
  
    static const std::set<std::wstring>
      validExtensions = { L".asm", L".c", L".cc", L".cpp", L".h" };

    if (validExtensions.find(file.path().extension().wstring()) == validExtensions.end())
      continue;

    if (!isExcluded(name,excludes,foundExcludes))
      _files.insert(name);
  }
}

const std::wstring Project::nasmOptions() const
{
  std::wstring
    options=L"";

  if (_options->architecture == Architecture::Arm64)
    throwException(L"NASM is not supported for Arm64 architecture");

  if (_options->architecture == Architecture::x64)
    options+=L" -fwin64 -DWIN64 -D__x86_64__";
  else
    options+=L" -fwin32 -DWIN32";

  for (const auto& include : _config->nasmIncludes(_options->architecture))
    options+=L" -i\"$(SolutionDir)" + _config->directory() + include + L"\"";

  options+=L" -o \"$(IntDir)%(Filename).obj\" \"%(FullPath)\"";
  return(options);
}

const std::wstring Project::runtimeLibrary(bool debug) const
{
  std::wstring prefix=debug ? L"MultiThreadedDebug" : L"MultiThreaded";
  return(prefix + (_options->buildType == BuildType::Dynamic ? L"DLL" : L""));
}

const std::wstring Project::targetName(bool debug) const
{
  switch(_type)
  {
  case ProjectType::DynamicLibrary:
    return(prefix() + L"_" + (debug ? L"DB_" : L"RL_") + name() + L"_");
  case ProjectType::StaticLibrary: 
    return(libraryName(name(),debug));
  default:
    throwException(L"Unsupported project type");
  }
}

void Project::write(const std::vector<Project> &allProjects) const
{
  const std::wstring vcxprojFileName=_options->rootDirectory + fileName();
  std::filesystem::create_directories(std::filesystem::path(vcxprojFileName).parent_path());

  std::wofstream file(vcxprojFileName);
  if (!file)
    throwException(L"Failed to open file: " + vcxprojFileName);

  const bool includeMasm=hasAsmfiles() && !_config->useNasm();

  file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
  file << "<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">" << std::endl;
  writeConfiguration(file);
  writeProperties(file);
  writeOutputProperties(file);
  writeCompilationConfiguration(file);
  writePropsImports(file,includeMasm);
  if (_type == ProjectType::StaticLibrary)
    writeLibProperties(file);
  else
    writeLinkProperties(file);
  writeFiles(file);
  writeReferences(file,allProjects);
  writeTargetsImports(file,includeMasm);
  writeCopyIncludes(file);
  file << "</Project>" << std::endl;


  // ADD IMAGEMAGICK RESOURCE FILE!
 /*   <ResourceCompile>
      <PreprocessorDefinitions>NDEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <Culture>0x0409</Culture>
    </ResourceCompile>*/




//  file << "    <ResourceCompile>" << std::endl;
//  file << "      <PreprocessorDefinitions Condition=\"'$(Configuration)'=='Debug'\">_DEBUG;%(PreprocessorDefinitions</PreprocessorDefinitions>" << std::endl;
//  file << "      <PreprocessorDefinitions Condition=\"'$(Configuration)'=='Release'\">NDEBUG;%(PreprocessorDefinitionsfault</PreprocessorDefinitions>" << std::endl;
//  file << "      <Culture>0x0409</Culture>" << std::endl;
//
  file.close();
}

void Project::writeCompilationConfiguration(std::wofstream &file) const
{
  file << "  <ItemDefinitionGroup>" << std::endl;
  file << "    <ClCompile>" << std::endl;
  file << "      <AdditionalOptions>/source-charset:utf-8 %(AdditionalOptions)</AdditionalOptions>" << std::endl;
  file << "      <AdditionalIncludeDirectories>" << includeDirectories() << "%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>" << std::endl;
  file << "      <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>" << std::endl;
  file << "      <FunctionLevelLinking>true</FunctionLevelLinking>" << std::endl;
  file << "      <LanguageStandard>stdcpp17</LanguageStandard>" << std::endl;
  file << "      <LanguageStandard_C>stdc17</LanguageStandard_C>" << std::endl;
  file << "      <MultiProcessorCompilation>true</MultiProcessorCompilation>" << std::endl;
  file << "      <StringPooling>true</StringPooling>" << std::endl;
  file << "      <SuppressStartupBanner>true</SuppressStartupBanner>" << std::endl;
  file << "      <OpenMPSupport>" << openMPSupport() << "</OpenMPSupport>" << std::endl;
  file << "      <WarningLevel>" << warningLevel() << "</WarningLevel>" << std::endl;
  file << "      <BasicRuntimeChecks Condition=\"'$(Configuration)'=='Debug'\">EnableFastChecks</BasicRuntimeChecks>" << std::endl;
  file << "      <BasicRuntimeChecks Condition=\"'$(Configuration)'=='Release'\">Default</BasicRuntimeChecks>" << std::endl;
  file << "      <InlineFunctionExpansion Condition=\"'$(Configuration)'=='Debug'\">Disabled</InlineFunctionExpansion>" << std::endl;
  file << "      <InlineFunctionExpansion Condition=\"'$(Configuration)'=='Release'\">AnySuitable</InlineFunctionExpansion>" << std::endl;
  file << "      <OmitFramePointers Condition=\"'$(Configuration)'=='Debug'\">false</OmitFramePointers>" << std::endl;
  file << "      <OmitFramePointers Condition=\"'$(Configuration)'=='Release'\">true</OmitFramePointers>" << std::endl;
  file << "      <Optimization Condition=\"'$(Configuration)'=='Debug'\">Disabled</Optimization>" << std::endl;
  file << "      <Optimization Condition=\"'$(Configuration)'=='Release'\">MaxSpeed</Optimization>" << std::endl;
  file << "      <PreprocessorDefinitions>" << defines() << ";%(PreprocessorDefinitions)</PreprocessorDefinitions>" << std::endl;
  file << "      <PreprocessorDefinitions Condition=\"'$(Configuration)'=='Debug'\">_DEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>" << std::endl;
  file << "      <PreprocessorDefinitions Condition=\"'$(Configuration)'=='Release'\">NDEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>" << std::endl;
  file << "      <RuntimeLibrary Condition=\"'$(Configuration)'=='Debug'\">" << runtimeLibrary(true) << "</RuntimeLibrary>" << std::endl;
  file << "      <RuntimeLibrary Condition=\"'$(Configuration)'=='Release'\">" << runtimeLibrary(false) << "</RuntimeLibrary>" << std::endl;
  file << "    </ClCompile>" << std::endl;
  file << "  </ItemDefinitionGroup>" << std::endl;
}

void Project::writeConfiguration(std::wofstream &file) const
{
  file << "  <ItemGroup Label=\"ProjectConfigurations\">" << std::endl;
  file << "    <ProjectConfiguration Include=\"Debug|" << _options->architectureName() << "\">" << std::endl;
  file << "      <Configuration>Debug</Configuration>" << std::endl;
  file << "      <Platform>" << _options->platform() << "</Platform>" << std::endl;
  file << "    </ProjectConfiguration>" << std::endl;
  file << "    <ProjectConfiguration Include=\"Release|" << _options->architectureName() << "\">" << std::endl;
  file << "      <Configuration>Release</Configuration>" << std::endl;
  file << "      <Platform>" << _options->platform() << "</Platform>" << std::endl;
  file << "    </ProjectConfiguration>" << std::endl;
  file << "  </ItemGroup>" << std::endl;
}

void Project::writeCopyIncludes(std::wofstream &file) const
{
  if (_config->includeArtifacts().empty())
    return;

  file << "  <Target Name=\"CopyIncludes\" AfterTargets=\"Build\">" << std::endl;
  file << "    <RemoveDir Directories=\"$(SolutionDir)Artifacts\\include\\" << name() << "\" Condition=\"Exists('$(SolutionDir)Artifacts\\include\\" << name() << "')\" />" << std::endl;
  file << "    <ItemGroup>" << std::endl;
  size_t index=0;
  for (const auto& include : _config->includeArtifacts())
  {
    if (endsWith(include.first,L".h"))
      file << "      <HeaderFiles" << index++ << " Include=\"$(SolutionDir)" << include.first << "\" />" << std::endl;
    else
      file << "      <HeaderFiles" << index++ << " Include=\"$(SolutionDir)" << include.first << "\\*.h\" />" << std::endl;
  }
  file << "    </ItemGroup>" << std::endl;
   index=0;
  for (const auto& include : _config->includeArtifacts())
  {
    file << "    <Error Condition=\"'@(HeaderFiles0)' == ''\" Text=\"No header files found in: " << include.first << "\" />" << std::endl;
    file << "    <Copy SourceFiles=\"@(HeaderFiles" << index++ << ")\" DestinationFolder=\"$(SolutionDir)Artifacts\\include\\" << name() << "\\" << include.second << "\" SkipUnchangedFiles=\"true\" />" << std::endl;
  }
  file << "  </Target>" << std::endl;
}

void Project::writeFiles(std::wofstream &file) const
{
  file << "  <ItemGroup>" << std::endl;
  for (auto& fileName : _files)
  {
    if (endsWith(fileName,L".h"))
      file << "    <ClInclude Include=\"$(SolutionDir)" << _config->directory() << fileName << "\" />" << std::endl;
    else if (endsWith(fileName,L".asm"))
    {
      if (_config->useNasm())
      {
        file << "    <CustomBuild Include=\"$(SolutionDir)" << _config->directory() << fileName << "\">" << std::endl;
        file << "      <Command>$(SolutionDir)Build\\nasm" << nasmOptions() << "</Command>" << std::endl;
        file << "      <Outputs>$(IntDir)%(Filename).obj;%(Outputs)</Outputs>" << std::endl;
        file << "    </CustomBuild>" << std::endl;
      }
      else
      {
        file << "    <MASM Include=\"$(SolutionDir)" << _config->directory() << fileName << "\">" << std::endl;
        file << "      <FileType>Document</FileType>" << std::endl;
        file << "    </MASM>" << std::endl;
      }
    }
    else
      file << "    <ClCompile Include=\"$(SolutionDir)" << _config->directory() << fileName << "\" />" << std::endl;
  }
  file << "  </ItemGroup>" << std::endl;
}

void Project::writeFilters() const
{
  std::wstring filterFileName=_options->rootDirectory + fileName() + L".filters";
  std::wofstream file(filterFileName);
  if (!file)
    throwException(L"Failed to open file: " + filterFileName);
  
  std::set<std::wstring> directories;
  file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
  file << "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">" << std::endl;
  file << "  <ItemGroup>" << std::endl;

  for (const auto& fileName : _files)
  {
    size_t slashIndex=fileName.find_last_of(L"\\");
    if (slashIndex == std::wstring::npos)
      continue;

    std::wstring directory=fileName.substr(0,slashIndex);
    directories.insert(directory);
    std::wstring parentDirectory=directory;
    while (slashIndex != std::wstring::npos)
    {
      parentDirectory=parentDirectory.substr(0,slashIndex);
      directories.insert(parentDirectory);
      slashIndex=parentDirectory.find_last_of(L"\\");
    }

    std::wstring tag=L"ClCompile";

    if (endsWith(fileName, L".h"))
    {
      tag = L"ClInclude";
    }
    else if (endsWith(fileName, L".asm"))
    {
      if (_config->useNasm())
        tag = L"CustomBuild";
      else
        tag = L"MASM";
    }
    
    file << "    <" << tag << " Include=\"$(SolutionDir)" << _config->directory() << fileName << "\">" << std::endl;
    file << "      <Filter>" << directory << "</Filter>" << std::endl;
    file << "    </" << tag << ">" << std::endl;
  }

  for (const auto& directory : directories)
  {
    file << "    <Filter Include=\"" << directory << "\">" << std::endl;
    file << "      <UniqueIdentifier>{" << createGuid(directory) << "}</UniqueIdentifier>" << std::endl;
    file << "    </Filter>" << std::endl;
  }

  file << "  </ItemGroup>" << std::endl;
  file << "</Project>" << std::endl;

  file.close();
}

void Project::writeLibProperties(std::wofstream& file) const
{
  file << "  <ItemDefinitionGroup>" << std::endl;
  file << "    <Lib>" << std::endl;
  file << "      <AdditionalLibraryDirectories>$(SolutionDir)Artifacts\\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>" << std::endl;
  file << "      <AdditionalDependencies>" << additionalDependencies() << ";%(AdditionalDependencies)</AdditionalDependencies>" << std::endl;
  file << "      <AdditionalOptions>/WX %(AdditionalOptions)</AdditionalOptions>" << std::endl;
  file << "    </Lib>" << std::endl;
  file << "  </ItemDefinitionGroup>" << std::endl;
}

void Project::writeLinkProperties(std::wofstream& file) const
{
  file << "  <ItemDefinitionGroup>" << std::endl;
  file << "    <Link>" << std::endl;
  file << "      <AdditionalLibraryDirectories>$(SolutionDir)Artifacts\\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>" << std::endl;
  file << "      <AdditionalDependencies>" << additionalDependencies() << ";%(AdditionalDependencies)</AdditionalDependencies>" << std::endl;
  file << "      <AdditionalOptions>/WX %(AdditionalOptions)</AdditionalOptions>" << std::endl;
  file << "      <ImportLibrary Condition=\"'$(Configuration)'=='Debug'\">$(SolutionDir)\\Artifacts\\lib\\" << libraryName(name(),true) <<".lib</ImportLibrary>" << std::endl;
  file << "      <ImportLibrary Condition=\"'$(Configuration)'=='Release'\">$(SolutionDir)\\Artifacts\\lib\\" << libraryName(name(),false) <<".lib</ImportLibrary>" << std::endl;
  if (_config->useUnicode())
    file << "    <EntryPointSymbol>wWinMainCRTStartup</EntryPointSymbol>" << std::endl;
  if (!_config->moduleDefinitionFile().empty())
    file << "      <ModuleDefinitionFile>$(SolutionDir)" <<  _config->directory() << _config->moduleDefinitionFile() << "</ModuleDefinitionFile>" << std::endl;
  file << "    </Link>" << std::endl;
  file << "  </ItemDefinitionGroup>" << std::endl;
}

void Project::writeOutputProperties(std::wofstream &file) const
{
    file << "  <PropertyGroup>" << std::endl;
  file << "    <LinkIncremental>false</LinkIncremental>" << std::endl;
  file << "    <OutDir>$(SolutionDir)Artifacts\\" << outputDirectory() << "\\</OutDir>" << std::endl;
  file << "    <TargetName Condition=\"'$(Configuration)'=='Debug'\">" << targetName(true) << "</TargetName>" << std::endl;
  file << "    <TargetName Condition=\"'$(Configuration)'=='Release'\">" << targetName(false) << "</TargetName>" << std::endl;
  if (_options->visualStudioVersion >= VisualStudioVersion::VS2019)
    file << "    <UseDebugLibraries Condition=\"'$(Configuration)'=='Debug'\">true</UseDebugLibraries>" << std::endl;
  file << "  </PropertyGroup>" << std::endl;
}

void Project::writeProperties(std::wofstream &file) const
{
  file << "  <PropertyGroup Label=\"Globals\">" << std::endl;
  file << "    <ProjectName>" << fullName() << "</ProjectName>" << std::endl;
  file << "    <ProjectGuid>{" << _guid << "}</ProjectGuid>" << std::endl;
  file << "    <Keyword>" << _options->platform() << "Proj</Keyword>" << std::endl;
  file << "  </PropertyGroup>" << std::endl;
  file << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />" << std::endl;
  file << "  <PropertyGroup Label=\"Configuration\">" << std::endl;
  file << "    <ConfigurationType>" << configurationType() << "</ConfigurationType>" << std::endl;
  file << "    <PlatformToolset>" << platformToolset() << "</PlatformToolset>" << std::endl;
  file << "    <UseOfMfc>false</UseOfMfc>" << std::endl;
  file << "    <CharacterSet>" << characterSet() << "</CharacterSet>" << std::endl;
  file << "  </PropertyGroup>" << std::endl;
}

void Project::writePropsImports(std::wofstream &file,bool includeMasm) const
{
  file << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />" << std::endl;
  if (includeMasm)
  {
    file << "  <ImportGroup Label=\"ExtensionSettings\">" << std::endl;
    file << "    <Import Project=\"$(VCTargetsPath)\\BuildCustomizations\\masm.props\" />" << std::endl;
    file << "  </ImportGroup>" << std::endl;
  }
}

void Project::writeReferences(std::wofstream &file,const std::vector<Project> &allProjects) const
{
  if (_config->references().empty())
    return;

  file << "  <ItemGroup>" << std::endl;
  for (const auto& reference : _config->references())
  {
    auto project = std::find_if(allProjects.begin(), allProjects.end(),[&reference](const Project& p) { return(p.name() == reference); });
    if (project == allProjects.end())
      continue;
    
    file << "    <ProjectReference Include=\"$(SolutionDir)" << project->fileName() << "\">" << std::endl;
    file << "      <Project>{" << project->guid() << "}</Project>" << std::endl;
    file << "      <Name>" << project->fullName() << "</Name>" << std::endl;
    file << "    </ProjectReference>" << std::endl;
  }

  file << "  </ItemGroup>" << std::endl;
}

void Project::writeTargetsImports(std::wofstream &file,bool includeMasm) const
{
  file << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />" << std::endl;
  if (includeMasm)
  {
    file << "  <ImportGroup Label=\"ExtensionTargets\">" << std::endl;
    file << "    <Import Project=\"$(VCTargetsPath)\\BuildCustomizations\\masm.targets\" />" << std::endl;
    file << "  </ImportGroup>" << std::endl;
  }
}



//void Config::loadFiles(const std::set<std::wstring> directories,const std::set<std::wstring> excludes)
//{
//  
//}


//
//Compiler Project::compiler() const
//{
//  return(_magickProject && _wizard.visualStudioVersion() >= VisualStudioVersion::VS2022
//    ? Compiler::CPP
//    : Compiler::Default);
//}
//
//const wstring Project::configDefine() const
//{
//  return(_configDefine);
//}
//
//const vector<wstring> &Project::defines()
//{
//  return(_defines);
//}
//
//const vector<wstring> &Project::definesDll()
//{
//  return(_definesDll);
//}
//
//const vector<wstring> &Project::definesLib()
//{
//  return(_definesLib);
//}
//
//const vector<wstring> &Project::dependencies()
//{
//  return(_dependencies);
//}
//
//const vector<wstring> &Project::directories()
//{
//  return(_directories);
//}
//
//const vector<wstring> &Project::excludes()
//{
//  return(_excludes);
//}
//
//const vector<ProjectFile*> &Project::files() const
//{
//  return(_files);
//}
//
//const vector<wstring> &Project::includes()
//{
//  return(_includes);
//}
//
//const vector<wstring> &Project::includesNasm()
//{
//  return(_includesNasm);
//}
//
//const vector<wstring> &Project::platformExcludes(Platform platform)
//{
//  switch (platform)
//  {
//    case Platform::X86: return(_excludesX86);
//    case Platform::X64: return(_excludesX64);
//    case Platform::ARM64: return(_excludesARM64);
//    default: throw;
//  }
//}
//
//const wstring Project::configPath(const wstring &subPath) const
//{
//  return(_configFolder + L"\\" + subPath);
//}
//
//const wstring Project::filePath(const wstring &subPath) const
//{
//  wstring
//    path = _filesFolder + L"\\";
//
//  path += _path != L"" ? _path : _name;
//
//  if (subPath == L".")
//    return(path);
//
//  if (subPath == L"..")
//    return( _filesFolder + L"\\");
//
//  return(path + L"\\" + subPath);
//}
//
//bool Project::isConsole() const
//{
//  if (!isExe())
//    return(false);
//  return(_type != ProjectType::APPTYPE);
//}
//
//bool Project::isDll() const
//{
//  return((_type == ProjectType::DLLTYPE) || (_type == ProjectType::DLLMODULETYPE));
//}
//
//bool Project::isExe() const
//{
//  return((_type == ProjectType::EXETYPE) || (_type == ProjectType::EXEMODULETYPE) || (_type == ProjectType::APPTYPE));
//}
//
//bool Project::isFuzz() const
//{
//  return(_modulePrefix == L"FUZZ");
//}
//
//bool Project::isLib() const
//{
//  return((_type == ProjectType::STATICTYPE));
//}
//
//bool Project::isModule() const
//{
//  return((_type == ProjectType::DLLMODULETYPE) || (_type == ProjectType::EXEMODULETYPE));
//}
//
//bool Project::isOptimizationDisable() const
//{
//  return(_disableOptimization);
//}
//
//bool Project::isSupported(const VisualStudioVersion visualStudioVersion) const
//{
//  return(visualStudioVersion >= _minimumVisualStudioVersion);
//}
//
//const vector<wstring> &Project::libraries()
//{
//  return(_libraries);
//}
//
//const wstring Project::moduleDefinitionFile() const
//{
//  return(_moduleDefinitionFile);
//}
//
//const wstring Project::name() const
//{
//  return(_name);
//}
//
//const wstring Project::notice() const
//{
//  return(_notice);
//}
//
//const vector<wstring> &Project::references()
//{
//  return(_references);
//}
//
//bool Project::treatWarningAsError() const
//{
//  return(_magickProject && _wizard.isImageMagick7());
//}
//
//bool Project::useNasm() const
//{
//  return(_useNasm);
//};
//
//bool Project::useOpenCL() const
//{
//  return(_useOpenCL);
//}
//
//bool Project::useUnicode() const
//{
//  return(_useUnicode);
//}
//
//const wstring Project::version() const
//{
//  return _versions[0];
//}
//
//int Project::warningLevel() const
//{
//  return(_magickProject && _wizard.isImageMagick7() ? 4 : 0);
//}
//
//void Project::checkFiles(const VisualStudioVersion visualStudioVersion)
//{
//   std::vector<ProjectFile*>
//     newFiles(_files.size());
//
//  auto filter=[visualStudioVersion](ProjectFile* p){ return p->isSupported(visualStudioVersion); };
//  auto it=std::copy_if(_files.begin(),_files.end(),newFiles.begin(),filter);
//  newFiles.resize(std::distance(newFiles.begin(),it));
//  _files=newFiles;
//}
//
//void Project::mergeProjectFiles()
//{
//  ProjectFile
//    *projectFile;
//
//  if ((_type != ProjectType::DLLMODULETYPE) || (_wizard.solutionType() == SolutionType::DYNAMIC_MT))
//    return;
//
//  projectFile=new ProjectFile(&_wizard,this,L"CORE",_name);
//  for (auto& file : _files)
//  {
//    projectFile->merge(file);
//  }
//  _files.clear();
//  _files.push_back(projectFile);
//}
//
//Project* Project::create(const ConfigureWizard &wizard,const wstring &configFolder, const wstring &filesFolder, const wstring &name)
//{
//  wifstream
//    config;
//
//  wstring
//    configPath;
//
//  configPath=configFolder + L"\\" + name + L"\\.ImageMagick";
//  config.open(pathFromRoot(configPath + L"\\Config.txt"));
//  if (!config)
//  {
//    configPath=configFolder + L"\\" + name;
//    config.open(pathFromRoot(configPath +L"\\Config.txt"));
//  }
//
//  if (!config)
//    return((Project *) NULL);
//
//  Project* project = new Project(wizard,configPath,filesFolder,name);
//  project->loadConfig(config);
//  config.close();
//
//  if (project->_onlyImageMagick7 && !wizard.isImageMagick7())
//    return((Project *) NULL);
//
//  project->setNoticeAndVersion();
//
//  return(project);
//}
//
//bool Project::loadFiles()
//{
//  ProjectFile
//    *projectFile;
//
//  _files.clear();
//
//  if (shouldSkip())
//    return(false);
//
//  switch(_type)
//  {
//    case ProjectType::DLLMODULETYPE:
//    {
//      loadModules();
//      break;
//    }
//    case ProjectType::DLLTYPE:
//    {
//      projectFile=new ProjectFile(&_wizard,this,L"CORE",_name);
//      _files.push_back(projectFile);
//      break;
//    }
//    case ProjectType::APPTYPE:
//    case ProjectType::EXETYPE:
//    {
//      projectFile=new ProjectFile(&_wizard,this,L"UTIL",_name);
//      _files.push_back(projectFile);
//      break;
//    }
//    case ProjectType::EXEMODULETYPE:
//    {
//      loadModules();
//      break;
//    }
//    case ProjectType::STATICTYPE:
//    {
//      projectFile=new ProjectFile(&_wizard,this,L"CORE",_name);
//      _files.push_back(projectFile);
//      break;
//    }
//  }
//
//  return(true);
//}
//
//bool Project::shouldSkip() const
//{
//  if (_disabledARM64 && _wizard.platform() == Platform::ARM64)
//    return(true);
//
//  if (_hasIncompatibleLicense && !_wizard.includeIncompatibleLicense())
//    return(true);
//
//  if (_isOptional && !_wizard.includeOptional())
//    return(true);
//
//  return(false);
//}
//
//void Project::updateProjectNames()
//{
//  _wizard.updateProjectNames(_name);
//
//  updateProjectNames(_dependencies);
//  updateProjectNames(_directories);
//  updateProjectNames(_includes);
//  updateProjectNames(_references);
//}
//
//void Project::updateProjectNames(vector<wstring> &vector)
//{
//  for (auto& value : vector)
//    _wizard.updateProjectNames(value);
//}
//
//Project::Project(const ConfigureWizard &wizard,const wstring &configFolder,const wstring &filesFolder,const wstring &name)
//  : _wizard(wizard)
//{
//  _configFolder=configFolder;
//  _filesFolder=filesFolder;
//  _name=name;
//
//  _disabledARM64=false;
//  _disableOptimization=false;
//  _hasIncompatibleLicense=false;
//  _isOptional=false;
//  _magickProject=false;
//  _minimumVisualStudioVersion=VSEARLIEST;
//  _onlyImageMagick7=false;
//  _type=ProjectType::UNDEFINEDTYPE;
//  _useNasm=false;
//  _useOpenCL=false;
//  _useUnicode=false;
//}
//
//void Project::addLines(wifstream &config,wstring &value)
//{
//  wstring
//    line;
//
//  while (!config.eof())
//  {
//    getline(config,line);
//    if (trim(line).empty())
//      return;
//
//    value+=line+L"\n";
//  }
//}
//
//void Project::addLines(wifstream &config,vector<wstring> &container)
//{
//  wstring
//    line;
//
//  while (!config.eof())
//  {
//    line=readLine(config);
//    if (line.empty())
//      return;
//
//    container.push_back(line);
//  }
//}
//
//void Project::loadConfig(wifstream &config)
//{
//  wstring
//    line;
//
//  while (!config.eof())
//  {
//    line=readLine(config);
//    if (line == L"[APP]")
//      _type=ProjectType::APPTYPE;
//    else if (line == L"[CONFIG_DEFINE]")
//      addLines(config,_configDefine);
//    else if (line == L"[DEFINES_DLL]")
//      addLines(config,_definesDll);
//    else if (line == L"[DEFINES_LIB]")
//      addLines(config,_definesLib);
//    else if (line == L"[DEFINES]")
//      addLines(config,_defines);
//    else if (line == L"[DEPENDENCIES]")
//      addLines(config,_dependencies);
//    else if (line == L"[DIRECTORIES]")
//      addLines(config,_directories);
//    else if (line == L"[DISABLED_ARM64]")
//      _disabledARM64=true;
//    else if (line == L"[DISABLE_OPTIMIZATION]")
//      _disableOptimization=true;
//    else if (line == L"[DLL]")
//      _type=ProjectType::DLLTYPE;
//    else if (line == L"[DLLMODULE]")
//      _type=ProjectType::DLLMODULETYPE;
//    else if (line == L"[EXE]")
//      _type=ProjectType::EXETYPE;
//    else if (line == L"[EXEMODULE]")
//      _type=ProjectType::EXEMODULETYPE;
//    else if (line == L"[EXCLUDES]")
//      addLines(config,_excludes);
//    else if (line == L"[EXCLUDES_X86]")
//      addLines(config,_excludesX86);
//    else if (line == L"[EXCLUDES_X64]")
//      addLines(config,_excludesX64);
//    else if (line == L"[EXCLUDES_ARM64]")
//      addLines(config,_excludesARM64);
//    else if (line == L"[INCLUDES]")
//      addLines(config,_includes);
//    else if (line == L"[INCLUDES_NASM]")
//      addLines(config,_includesNasm);
//    else if (line == L"[INCOMPATIBLE_LICENSE]")
//      _hasIncompatibleLicense=true;
//    else if (line == L"[STATIC]")
//      _type=ProjectType::STATICTYPE;
//    else if (line == L"[LIBRARIES]")
//      addLines(config,_libraries);
//    else if (line == L"[MODULE_DEFINITION_FILE]")
//      _moduleDefinitionFile=readLine(config);
//    else if (line == L"[MODULE_PREFIX]")
//      _modulePrefix=readLine(config);
//    else if (line == L"[NASM]")
//      _useNasm=true;
//    else if (line == L"[ONLY_IMAGEMAGICK7]")
//      _onlyImageMagick7=true;
//    else if (line == L"[OPTIONAL]")
//      _isOptional=true;
//    else if (line == L"[PATH]")
//      _path=readLine(config);
//    else if (line == L"[REFERENCES]")
//      addLines(config,_references);
//    else if (line == L"[UNICODE]")
//      _useUnicode=true;
//    else if (line == L"[OPENCL]")
//      _useOpenCL=true;
//    else if (line == L"[VISUAL_STUDIO]")
//      _minimumVisualStudioVersion=parseVisualStudioVersion(readLine(config));
//    else if (line == L"[MAGICK_PROJECT]")
//      _magickProject=true;
//    else if (line == L"[LICENSE]")
//      _licenseFileNames=readLicenseFilenames(readLine(config));
//  }
//}
//
//void Project::loadModules()
//{
//  ProjectFile
//    *projectAlias,
//    *projectFile;
//
//  for (auto& dir :_directories)
//  {
//    const wstring
//      path(pathFromRoot(filePath(dir)));
//
//    if (!directoryExists(path))
//      throwException(L"Invalid folder specified: " + path);
//    
//    for (const auto& entry : filesystem::directory_iterator(path))
//    {
//      wstring
//        fileName,
//        name;
//
//      if (!entry.is_regular_file())
//        continue;
//
//      fileName=entry.path().filename();
//      if (contains(_excludes,fileName) || startsWith(fileName,L"main.") || !isValidSrcFile(fileName))
//        continue;
//
//      name=fileName;
//      name=name.substr(0,name.find_last_of(L"."));
//      projectFile=new ProjectFile(&_wizard,this,_modulePrefix,name);
//      _files.push_back(projectFile);
//
//      for (auto& alias : projectFile->aliases())
//      {
//        projectAlias=new ProjectFile(&_wizard,this,_modulePrefix,alias,name);
//        _files.push_back(projectAlias);
//      }
//    }
//  }
//}
//
//const vector<wstring> Project::readLicenseFilenames(const wstring &line) const
//{
//  wstring
//    fileName;
//
//  vector<wstring>
//    fileNames;
//
//  wstringstream
//    wss(line);
//
//  while(getline(wss, fileName, L';'))
//  {
//    wstring
//      filePath(pathFromRoot(filePath(fileName)));
//
//    filesystem::path
//      file(filePath);
//
//    if (!filesystem::exists(file))
//      throwException(L"Unable to open license file: " + fileName);
//
//    fileNames.push_back(filePath);
//  }
//
//  return (fileNames);
//}
//
//void Project::setNoticeAndVersion()
//{
//  _notice=L"";
//  for (auto& licenseFileName : _licenseFileNames)
//  {
//    filesystem::path
//      folder,
//      versionFile;
//
//    wifstream
//      version;
//
//    wstring
//      versionFileName;
//
//    folder=filesystem::path(licenseFileName).parent_path();
//    versionFileName=folder.wstring()+L"\\.ImageMagick\\ImageMagick.version.h";
//    versionFile=filesystem::path(versionFileName).wstring();
//    if (!filesystem::exists(versionFile))
//      {
//        folder=folder.parent_path();
//        versionFileName=folder.wstring()+L"\\.ImageMagick\\ImageMagick.version.h";
//        versionFile=filesystem::path(versionFileName).wstring();
//        if (!filesystem::exists(versionFile))
//          throwException(L"Unable to find version file for: " + _name);
//      }
//
//    version.open(versionFileName);
//    while (!version.eof())
//    {
//      wstring
//        line;
//
//      line=readLine(version);
//      if (!startsWith(line,L"DELEGATE_VERSION_NUM"))
//        continue;
//
//      line=line.substr(line.find_last_of(L" "));
//      line=replace(line,L",",L".");
//      _versions.push_back(line);
//      break;
//    }
//    version.close();
//
//    _notice+=L"[ "+folder.stem().wstring()+_versions.back()+L" ] copyright:\r\n";
//    _notice+=readLicense(licenseFileName)+L"\r\n";
//  }
//}
