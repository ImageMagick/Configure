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

Project::Project(const Config &config,const Options &options)
  : _config(config),
    _options(options)
{  
}

const wstring Project::characterSet() const
{
  return(_config.useUnicode() ? L"Unicode" : L"MultiByte");
}

const Compiler Project::compiler() const
{
  return(_config.isMagickProject() && _options.visualStudioVersion >= VisualStudioVersion::VS2022
    ? Compiler::CPP
    : Compiler::Default);
}

const wstring Project::configurationType() const
{
  if (isApplication())
    return(L"Application");

  if (_options.isStaticBuild || _config.type() == ProjectType::StaticLibrary)
    return(L"StaticLibrary");

  return(L"DynamicLibrary");
}

const wstring Project::defines() const
{
  wstring defines=L"_WIN32_WINNT=0x0601";
  if (_options.isStaticBuild || _config.type() == ProjectType::StaticLibrary)
  {
    defines+=L";_LIB";
    for (auto& define : _config.staticDefines())
      defines+=L";" + define;
  }
  else
  {
    for (auto& define : _config.dynamicDefines())
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

const wstring Project::includeDirectories() const
{
  wstring directories;

  if (_config.includes().empty())
    directories=L"$(SolutionDir)" + _config.directory() + L";";

  for (const auto& include : _config.includes())
  {
    wstring includeDirectory=include;
    if (include[0] == L'\\')
      includeDirectory=include.substr(1);
    else
      includeDirectory=_config.directory() + include;

    directories+=L"$(SolutionDir)" + includeDirectory + L";";
  }

  for (const auto& reference : _config.references())
    directories+=L"$(SolutionDir)Artifacts\\include\\" + reference + L";";

  if (_options.useOpenCL && _config.useOpenCL())
    directories+=L"$(SolutionDir)Build\\OpenCL";

  return(directories);
}

const bool Project::isApplication() const
{
  switch(_config.type())
  {
    case ProjectType::Application:
    case ProjectType::Demo:
    case ProjectType::Fuzz:
      return(true);
    default:
      return(false);
  }
}

const wstring Project::nasmOptions() const
{
  wstring
    options=L"";

  if (_options.architecture == Architecture::Arm64)
    throwException(L"NASM is not supported for Arm64 architecture");

  if (_options.architecture == Architecture::x64)
    options+=L" -fwin64 -DWIN64 -D__x86_64__";
  else
    options+=L" -fwin32 -DWIN32";

  for (const auto& include : _config.nasmIncludes(_options.architecture))
    options+=L" -i\"$(SolutionDir)" + _config.directory() + include + L"\"";

  options+=L" -o \"$(IntDir)%(Filename).obj\" \"%(FullPath)\"";
  return(options);
}

const wstring Project::openMPSupport() const
{
  return(_options.useOpenMP ? L"true" : L"false");
}

const wstring Project::outputDirectory() const
{
  switch(_config.type())
  {
    case ProjectType::Application: return(L"bin");
    case ProjectType::Coder:
    case ProjectType::DynamicLibrary:
    case ProjectType::Filter:
      return(_options.isStaticBuild ? L"lib" : L"bin");
    case ProjectType::Demo: return(L"demo");
    case ProjectType::Fuzz: return(L"fuzz");
    case ProjectType::StaticLibrary: return(L"lib");
    default: throwException(L"Unsupported project type");
  }
}

const wstring Project::platformToolset() const
{
  switch (_options.visualStudioVersion)
  {
    case VisualStudioVersion::VS2022: return(L"v143");
    case VisualStudioVersion::VS2019: return(L"v142");
    case VisualStudioVersion::VS2017: return(L"v141");
    default: throwException(L"Unknown architecture");
  }
}

const wstring Project::prefix() const
{
  switch(_config.type())
  {
  case ProjectType::Application: return(L"APP");
  case ProjectType::Coder:
      return(_options.isStaticBuild ? L"CORE" : L"IM_MOD");
  case ProjectType::DynamicLibrary: return(L"CORE");
  case ProjectType::Demo: return(L"DEMO");
  case ProjectType::Filter:
      return(_options.isStaticBuild ? L"CORE" :L"FILTER");
  case ProjectType::Fuzz: return(L"FUZZ");
  case ProjectType::StaticLibrary: return(L"CORE");
  default: throwException(L"Unsupported project type");
  }
}

const wstring Project::warningLevel() const
{
  if (_options.isImageMagick7 && _config.isMagickProject())
    return(L"Level4");
  else
    return(L"TurnOffAllWarnings");
}

const wstring Project::additionalDependencies(const bool debug) const
{
  wstring dependencies;

  for (auto& reference : _config.references())
    dependencies+=(debug ? L"CORE_DB_" : L"CORE_RL_") + reference + L"_.lib;";

  if (!_options.isStaticBuild)
  {
    for (auto& reference : _config.coderReferences())
      dependencies+=(debug ? L"IM_MOD_DB_" : L"IM_MOD_RL_") + reference + L"_.lib;";
  }

  return(dependencies);
}

void Project::copyConfigInfo(const Config& config)
{
  _config=_config.copyInfo(config);
}

Project Project::create(const Config &config,const Options &options)
{
  Project project(config,options);
  project.loadFiles();

  return(project);
}

bool Project::isExcluded(const wstring fileName,set<wstring> &excludes,multiset<wstring> &foundExcludes) const
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

void Project::loadFiles()
{
  multiset<wstring> foundExcludes;
  set<wstring> excludes(_config.excludes(_options.architecture));

  loadFiles(L"",excludes,foundExcludes);

  for (const auto& exclude : excludes)
  {
    if (foundExcludes.find(exclude) == foundExcludes.end())
      throwException(L"Invalid exclude path " + exclude + L" in " + name());
  }
}

void Project::loadFiles(const wstring directory,set<wstring> &excludes,multiset<wstring> &foundExcludes)
{
  if (isExcluded(directory + L"\\",excludes,foundExcludes))
    return;

  for (const auto& file : filesystem::directory_iterator(_options.rootDirectory + _config.directory() + directory))
  {
    wstring name=directory;
    if (!directory.empty())
      name+=L"\\";
    name+=file.path().filename().wstring();

    if (file.is_directory())
    {
      loadFiles(name,excludes,foundExcludes);
      continue;
    }
  
    static const set<wstring>
      validExtensions = { L".asm", L".c", L".cc", L".cpp", L".h" };

    if (validExtensions.find(file.path().extension().wstring()) == validExtensions.end())
      continue;

    if (!isExcluded(name,excludes,foundExcludes))
      _files.insert(name);
  }
}

void Project::rename(const wstring& name)
{
  _config.rename(name);
}

const wstring Project::runtimeLibrary(bool debug) const
{
  wstring prefix=debug ? L"MultiThreadedDebug" : L"MultiThreaded";
  return(prefix + (_options.linkRuntime ? L"" : L"DLL"));
}

void Project::setFiles(const vector<wstring> files)
{
  _files.clear();

  for (const auto& file : files)
    _files.insert(file);
}

vector<Project> Project::splitToFiles(const vector<wstring> additionalFiles) const
{
  vector<Project> projects;
  for (const auto& file : _files)
  {
    if (!endsWith(file,L".c") && !endsWith(file,L".cc") && !endsWith(file,L".cpp"))
      continue;

    Config config(_config);
    config.rename(file.substr(0,file.find_last_of(L".")));

    Project project=create(config,_options);
    project._files.clear();
    project._files.insert(file);

    const wstring headerFile = file.substr(0,file.find_last_of(L".")) + L".h";
    if (filesystem::exists(_options.rootDirectory + _config.directory() + headerFile))
      project._files.insert(headerFile);

    for (const auto& additionalFile : additionalFiles)
    {
      project._files.insert(additionalFile);
    }

    projects.push_back(project);
  }
  return(projects);
}

const wstring Project::targetName(bool debug) const
{
  if (_config.type() == ProjectType::Application)
    return(name());

  return(prefix() + L"_" + (debug ? L"DB_" : L"RL_") + name() + L"_");
}

void Project::write(const vector<Project> &allProjects) const
{
  const wstring vcxprojFileName=_options.rootDirectory + fileName();
  filesystem::create_directories(filesystem::path(vcxprojFileName).parent_path());

  wofstream file(vcxprojFileName);
  if (!file)
    throwException(L"Failed to open file: " + vcxprojFileName);

  const bool includeMasm=hasAsmfiles() && !_config.useNasm() && _options.architecture != Architecture::Arm64;

  file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
  file << "<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">" << endl;
  writeConfiguration(file);
  writeProperties(file);
  writeOutputProperties(file);
  writeCompilationConfiguration(file);
  writePropsImports(file,includeMasm);
  if ((_config.type() == ProjectType::StaticLibrary) ||
      (_options.isStaticBuild && (_config.type() == ProjectType::DynamicLibrary || _config.type() == ProjectType::Coder)))
    writeLibProperties(file);
  else
    writeLinkProperties(file);
  writeFiles(file);
  writeReferences(file,allProjects);
  writeTargetsImports(file,includeMasm);
  writeCopyIncludes(file);
  file << "</Project>" << endl;
}
void Project::writeCompilationConfiguration(wofstream &file) const
{
  file << "  <ItemDefinitionGroup>" << endl;
  file << "    <ClCompile>" << endl;
  file << "      <AdditionalOptions>/source-charset:utf-8 %(AdditionalOptions)</AdditionalOptions>" << endl;
  file << "      <AdditionalIncludeDirectories>" << includeDirectories() << "%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>" << endl;
  file << "      <FunctionLevelLinking>true</FunctionLevelLinking>" << endl;
  file << "      <LanguageStandard>stdcpp17</LanguageStandard>" << endl;
  file << "      <LanguageStandard_C>stdc17</LanguageStandard_C>" << endl;
  file << "      <MultiProcessorCompilation>true</MultiProcessorCompilation>" << endl;
  file << "      <StringPooling>true</StringPooling>" << endl;
  file << "      <SuppressStartupBanner>true</SuppressStartupBanner>" << endl;
  file << "      <OpenMPSupport>" << openMPSupport() << "</OpenMPSupport>" << endl;
  file << "      <WarningLevel>" << warningLevel() << "</WarningLevel>" << endl;
  file << "      <DebugInformationFormat Condition=\"'$(Configuration)'=='Debug'\">ProgramDatabase</DebugInformationFormat>" << endl;
  file << "      <DebugInformationFormat Condition=\"'$(Configuration)'=='Release'\">None</DebugInformationFormat>" << endl;
  file << "      <BasicRuntimeChecks Condition=\"'$(Configuration)'=='Debug'\">EnableFastChecks</BasicRuntimeChecks>" << endl;
  file << "      <BasicRuntimeChecks Condition=\"'$(Configuration)'=='Release'\">Default</BasicRuntimeChecks>" << endl;
  file << "      <InlineFunctionExpansion Condition=\"'$(Configuration)'=='Debug'\">Disabled</InlineFunctionExpansion>" << endl;
  file << "      <InlineFunctionExpansion Condition=\"'$(Configuration)'=='Release'\">AnySuitable</InlineFunctionExpansion>" << endl;
  file << "      <OmitFramePointers Condition=\"'$(Configuration)'=='Debug'\">false</OmitFramePointers>" << endl;
  file << "      <OmitFramePointers Condition=\"'$(Configuration)'=='Release'\">true</OmitFramePointers>" << endl;
  file << "      <Optimization Condition=\"'$(Configuration)'=='Debug'\">Disabled</Optimization>" << endl;
  file << "      <Optimization Condition=\"'$(Configuration)'=='Release'\">MaxSpeed</Optimization>" << endl;
  file << "      <PreprocessorDefinitions>" << defines() << ";%(PreprocessorDefinitions)</PreprocessorDefinitions>" << endl;
  file << "      <PreprocessorDefinitions Condition=\"'$(Configuration)'=='Debug'\">_DEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>" << endl;
  file << "      <PreprocessorDefinitions Condition=\"'$(Configuration)'=='Release'\">NDEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>" << endl;
  file << "      <RuntimeLibrary Condition=\"'$(Configuration)'=='Debug'\">" << runtimeLibrary(true) << "</RuntimeLibrary>" << endl;
  file << "      <RuntimeLibrary Condition=\"'$(Configuration)'=='Release'\">" << runtimeLibrary(false) << "</RuntimeLibrary>" << endl;
  if (compiler() == Compiler::CPP)
    file << "      <CompileAs>CompileAsCpp</CompileAs>" << endl;
  if (_config.isMagickProject() && _options.isImageMagick7)
    file << "      <TreatWarningAsError>true</TreatWarningAsError>" << endl;
  file << "    </ClCompile>" << endl;
  file << "  </ItemDefinitionGroup>" << endl;
}

void Project::writeConfiguration(wofstream &file) const
{
  file << "  <ItemGroup Label=\"ProjectConfigurations\">" << endl;
  file << "    <ProjectConfiguration Include=\"Debug|" << _options.platform() << "\">" << endl;
  file << "      <Configuration>Debug</Configuration>" << endl;
  file << "      <Platform>" << _options.platform() << "</Platform>" << endl;
  file << "    </ProjectConfiguration>" << endl;
  file << "    <ProjectConfiguration Include=\"Release|" << _options.platform() << "\">" << endl;
  file << "      <Configuration>Release</Configuration>" << endl;
  file << "      <Platform>" << _options.platform() << "</Platform>" << endl;
  file << "    </ProjectConfiguration>" << endl;
  file << "  </ItemGroup>" << endl;
}

void Project::writeCopyIncludes(wofstream &file) const
{
  if (_config.includeArtifacts().empty())
    return;

  file << "  <Target Name=\"CopyIncludes\" AfterTargets=\"Build\">" << endl;
  file << "    <RemoveDir Directories=\"$(SolutionDir)Artifacts\\include\\" << name() << "\" Condition=\"Exists('$(SolutionDir)Artifacts\\include\\" << name() << "')\" />" << endl;
  file << "    <ItemGroup>" << endl;
  size_t index=0;
  for (const auto& include : _config.includeArtifacts())
  {
    if (endsWith(include.first,L".h"))
      file << "      <HeaderFiles" << index++ << " Include=\"$(SolutionDir)" << include.first << "\" />" << endl;
    else
      file << "      <HeaderFiles" << index++ << " Include=\"$(SolutionDir)" << include.first << "\\*.h\" />" << endl;
  }
  file << "    </ItemGroup>" << endl;
   index=0;
  for (const auto& include : _config.includeArtifacts())
  {
    file << "    <Error Condition=\"'@(HeaderFiles" << index << ")' == ''\" Text=\"No header files found in: " << include.first << "\" />" << endl;
    file << "    <Copy SourceFiles=\"@(HeaderFiles" << index++ << ")\" DestinationFolder=\"$(SolutionDir)Artifacts\\include\\" << name() << "\\" << include.second << "\" SkipUnchangedFiles=\"true\" />" << endl;
  }
  file << "  </Target>" << endl;
}

void Project::writeFiles(wofstream &file) const
{
  unordered_map<wstring, int> fileNameCount;
  file << "  <ItemGroup>" << endl;
  for (auto& fileName : _files)
  {
    wstring objectName=fileName.substr(fileName.find_last_of(L"\\") + 1);

    if (endsWith(fileName,L".h"))
      file << "    <ClInclude Include=\"$(SolutionDir)" << _config.directory() << fileName << "\" />" << endl;
    else if (endsWith(fileName,L".asm"))
    {
      if (_config.useNasm())
      {
        file << "    <CustomBuild Include=\"$(SolutionDir)" << _config.directory() << fileName << "\">" << endl;
        file << "      <Command>$(SolutionDir)Build\\nasm" << nasmOptions() << "</Command>" << endl;
        if (fileNameCount[objectName]++ == 0)
          file << "      <Outputs>$(IntDir)%(Filename).obj;%(Outputs)</Outputs>" << endl;
        else
          file << "      <Outputs>$(IntDir)%(Filename)." << fileNameCount[objectName] << ".obj;%(Outputs)</Outputs>" << endl;
        file << "    </CustomBuild>" << endl;
      }
      else if (_options.architecture == Architecture::Arm64)
      {
        file << "    <CustomBuild Include=\"$(SolutionDir)" << _config.directory() << fileName << "\">" << endl;
        file << "      <Command>armasm64 \"%(FullPath)\" -o \"$(IntDir)%(Filename).obj\"</Command>" << endl;
        if (fileNameCount[objectName]++ == 0)
          file << "      <Outputs>$(IntDir)%(Filename).obj;%(Outputs)</Outputs>" << endl;
        else
          file << "      <Outputs>$(IntDir)%(Filename)." << fileNameCount[objectName] << ".obj;%(Outputs)</Outputs>" << endl;
        file << "    </CustomBuild>" << endl;
      }
      else
      {
        file << "    <MASM Include=\"$(SolutionDir)" << _config.directory() << fileName << "\">" << endl;
        file << "      <FileType>Document</FileType>" << endl;
        if (_options.architecture == Architecture::x86)
          file << "      <UseSafeExceptionHandlers>true</UseSafeExceptionHandlers>" << endl;
        file << "    </MASM>" << endl;
      }
    }
    else
    {
      if (fileNameCount[objectName]++ == 0)
        file << "    <ClCompile Include=\"$(SolutionDir)" << _config.directory() << fileName << "\" />" << endl;
      else
      {
        file << "    <ClCompile Include=\"$(SolutionDir)" << _config.directory() << fileName << "\">" << endl;
        file << "      <ObjectFileName>$(IntDir)" << objectName << L"." << fileNameCount[objectName] << ".obj</ObjectFileName>" << endl;
        file << "    </ClCompile>" << endl;
      }
    }
  }

  if (!_config.resourceFileName().empty())
    file << "    <ResourceCompile Include=\"$(SolutionDir)" << _config.resourceFileName().substr(_options.rootDirectory.length()) << "\" />" << endl;

  file << "  </ItemGroup>" << endl;
}

void Project::writeFilters() const
{
  wstring filterFileName=_options.rootDirectory + fileName() + L".filters";
  wofstream file(filterFileName);
  if (!file)
    throwException(L"Failed to open file: " + filterFileName);
  
  set<wstring> directories;
  file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
  file << "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">" << endl;
  file << "  <ItemGroup>" << endl;

  for (const auto& fileName : _files)
  {
    size_t slashIndex=fileName.find_last_of(L"\\");
    if (slashIndex == wstring::npos)
      continue;

    wstring directory=fileName.substr(0,slashIndex);
    directories.insert(directory);
    wstring parentDirectory=directory;
    while (slashIndex != wstring::npos)
    {
      parentDirectory=parentDirectory.substr(0,slashIndex);
      directories.insert(parentDirectory);
      slashIndex=parentDirectory.find_last_of(L"\\");
    }

    wstring tag=L"ClCompile";

    if (endsWith(fileName, L".h"))
    {
      tag = L"ClInclude";
    }
    else if (endsWith(fileName, L".asm"))
    {
      if (_config.useNasm())
        tag = L"CustomBuild";
      else
        tag = L"MASM";
    }
    
    file << "    <" << tag << " Include=\"$(SolutionDir)" << _config.directory() << fileName << "\">" << endl;
    file << "      <Filter>" << directory << "</Filter>" << endl;
    file << "    </" << tag << ">" << endl;
  }

  for (const auto& directory : directories)
  {
    file << "    <Filter Include=\"" << directory << "\">" << endl;
    file << "      <UniqueIdentifier>{" << createGuid(directory) << "}</UniqueIdentifier>" << endl;
    file << "    </Filter>" << endl;
  }

  file << "  </ItemGroup>" << endl;
  file << "</Project>" << endl;
}

void Project::writeLibProperties(wofstream& file) const
{
  file << "  <ItemDefinitionGroup>" << endl;
  file << "    <Lib>" << endl;
  file << "      <TreatLibWarningAsErrors>true</TreatLibWarningAsErrors>" << endl;
  file << "    </Lib>" << endl;
  file << "  </ItemDefinitionGroup>" << endl;
}

void Project::writeLicense() const
{
  if (_config.licenses().empty())
    return;

  wstring targetDirectory=_options.rootDirectory + L"Artifacts\\license\\";
  filesystem::create_directories(targetDirectory);

  wofstream licenseFile(targetDirectory + name() + L".txt");
  for (const auto& license : _config.licenses())
  {
    wstring sourceFileName=_options.rootDirectory + _config.directory() + license;
    wifstream sourceLicenseFile(sourceFileName);
    if (!sourceLicenseFile)
      throwException(L"Failed to open license file: " + sourceFileName);

    wstring versionFileName=_options.rootDirectory + _config.directory() + L".ImageMagick\\ImageMagick.version.h";
    wstring projectName=name();
    if (!filesystem::exists(versionFileName))
    {
      wstring configFolder=sourceFileName.substr(0,sourceFileName.find_last_of(L"\\"));
      versionFileName=configFolder + L"\\.ImageMagick\\ImageMagick.version.h";
      projectName=configFolder.substr(configFolder.find_last_of(L"\\") + 1);
    }

    wifstream versionFile(versionFileName);
    if (versionFile)
    {
      wstring
        line;

      getline(versionFile,line);
      getline(versionFile,line);
      if (!startsWith(line,L"#define DELEGATE_VERSION_STRING "))
        throwException(L"Invalid version file: " + versionFileName);
      line=line.substr(33,line.length() - 34);
      licenseFile << L"[ " << projectName << L" " << line << L" ]" << endl << endl;
    }
    else
    {
      licenseFile << L"[ " << projectName << L" ]" << endl << endl;
    }
    licenseFile << sourceLicenseFile.rdbuf() << endl;
  }
}

void Project::writeLinkProperties(wofstream& file) const
{
  wstring preBuildLibs;

  if (_options.isStaticBuild && isApplication())
  {
    for (const auto& library : _options.preBuildLibs())
      preBuildLibs += library + L";";
  }

  file << "  <ItemDefinitionGroup>" << endl;
  file << "    <Link>" << endl;
  file << "      <AdditionalLibraryDirectories>$(SolutionDir)Artifacts\\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>" << endl;
  file << "      <TreatLinkerWarningAsErrors>true</TreatLinkerWarningAsErrors>" << endl;
  file << "      <AdditionalDependencies Condition=\"'$(Configuration)'=='Debug'\">" << additionalDependencies(true) << "%(AdditionalDependencies)</AdditionalDependencies>" << endl;
  file << "      <AdditionalDependencies Condition=\"'$(Configuration)'=='Release'\">" << preBuildLibs << additionalDependencies(false) << "%(AdditionalDependencies)</AdditionalDependencies>" << endl;
  file << "      <ImportLibrary Condition=\"'$(Configuration)'=='Debug'\">$(SolutionDir)Artifacts\\lib\\" << targetName(true) <<".lib</ImportLibrary>" << endl;
  file << "      <ImportLibrary Condition=\"'$(Configuration)'=='Release'\">$(SolutionDir)Artifacts\\lib\\" << targetName(false) <<".lib</ImportLibrary>" << endl;
  if (_config.useUnicode())
    file << "      <EntryPointSymbol>wWinMainCRTStartup</EntryPointSymbol>" << endl;
  if (!_config.moduleDefinitionFile().empty())
    file << "      <ModuleDefinitionFile>$(SolutionDir)" <<  _config.directory() << _config.moduleDefinitionFile() << "</ModuleDefinitionFile>" << endl;
  file << "    </Link>" << endl;
  file << "  </ItemDefinitionGroup>" << endl;
}

void Project::writeMagickBaseconfigDefine() const
{
  if (_config.magickBaseconfigDefine().empty())
    return;

  wstring targetDirectory=_options.rootDirectory + L"Artifacts\\config\\";
  filesystem::create_directories(targetDirectory);

  wstring configFileName=targetDirectory + name() + L".h";
  wofstream configFile(configFileName);
    if (!configFile)
      throwException(L"Failed to open license file: " + configFileName);

  configFile << _config.magickBaseconfigDefine();
}

void Project::writeOutputProperties(wofstream &file) const
{
  file << "  <PropertyGroup>" << endl;
  file << "    <LinkIncremental>false</LinkIncremental>" << endl;
  file << "    <OutDir>$(SolutionDir)Artifacts\\" << outputDirectory() << "\\</OutDir>" << endl;
  file << "    <TargetName Condition=\"'$(Configuration)'=='Debug'\">" << targetName(true) << "</TargetName>" << endl;
  file << "    <TargetName Condition=\"'$(Configuration)'=='Release'\">" << targetName(false) << "</TargetName>" << endl;
  if (_options.visualStudioVersion >= VisualStudioVersion::VS2019)
    file << "    <UseDebugLibraries Condition=\"'$(Configuration)'=='Debug'\">true</UseDebugLibraries>" << endl;
  file << "  </PropertyGroup>" << endl;
}

void Project::writeProperties(wofstream &file) const
{
  file << "  <PropertyGroup Label=\"Globals\">" << endl;
  file << "    <ProjectName>" << fullName() << "</ProjectName>" << endl;
  file << "    <ProjectGuid>{" << guid() << "}</ProjectGuid>" << endl;
  file << "    <Keyword>" << _options.platform() << "Proj</Keyword>" << endl;
  file << "  </PropertyGroup>" << endl;
  file << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />" << endl;
  file << "  <PropertyGroup Label=\"Configuration\">" << endl;
  file << "    <CharacterSet>" << characterSet() << "</CharacterSet>" << endl;
  file << "    <ConfigurationType>" << configurationType() << "</ConfigurationType>" << endl;
  file << "    <PlatformToolset>" << platformToolset() << "</PlatformToolset>" << endl;
  file << "    <UseOfMfc>false</UseOfMfc>" << endl;
  file << "  </PropertyGroup>" << endl;
  file << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />" << endl;
}

void Project::writePropsImports(wofstream &file,bool includeMasm) const
{
  if (includeMasm)
  {
    file << "  <ImportGroup Label=\"ExtensionSettings\">" << endl;
    file << "    <Import Project=\"$(VCTargetsPath)\\BuildCustomizations\\masm.props\" />" << endl;
    file << "  </ImportGroup>" << endl;
  }
}

void Project::writeReference(wofstream &file,const Project &project) const
{
    file << "    <ProjectReference Include=\"$(SolutionDir)" << project.fileName() << "\">" << endl;
    file << "      <Project>{" << project.guid() << "}</Project>" << endl;
    file << "      <Name>" << project.fullName() << "</Name>" << endl;
    file << "    </ProjectReference>" << endl;
}

void Project::writeReferences(wofstream &file,const vector<Project> &allProjects) const
{
  if (_config.references().empty())
    return;

  file << "  <ItemGroup>" << endl;

  for (const auto& reference : _config.references())
  {
    auto project = find_if(allProjects.begin(), allProjects.end(),[&reference](const Project& p) { return(p.isLibrary() && p.name() == reference); });
    if (project == allProjects.end())
      continue;

    writeReference(file,project[0]);
  }

  for (const auto& reference : _config.coderReferences())
  {
    auto project = find_if(allProjects.begin(), allProjects.end(),[&reference](const Project& p) { return(p.type() == ProjectType::Coder && p.name() == reference); });
    if (project == allProjects.end())
      continue;

    writeReference(file,project[0]);
  }

  if (isApplication())
  {
    for (const auto& project : allProjects)
    {
      if (project.type() == ProjectType::Coder || project.type() == ProjectType::Filter)
        writeReference(file,project);
    }
  }

  file << "  </ItemGroup>" << endl;
}

void Project::writeTargetsImports(wofstream &file,bool includeMasm) const
{
  file << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />" << endl;
  if (includeMasm)
  {
    file << "  <ImportGroup Label=\"ExtensionTargets\">" << endl;
    file << "    <Import Project=\"$(VCTargetsPath)\\BuildCustomizations\\masm.targets\" />" << endl;
    file << "  </ImportGroup>" << endl;
  }
}
