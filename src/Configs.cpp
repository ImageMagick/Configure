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
#include "Configs.h"

void Configs::addConfig(const Config &config,const ConfigureOptions &options,std::vector<Config> &configs)
{
  if (options.version == ImageMagickVersion::ImageMagick6 && config.isMagickProject())
    return;

  if (!options.includeOptional && config.isOptional())
    return;

  configs.push_back(config);
}

std::vector<Config> Configs::load(const ConfigureOptions &options)
{
  std::vector<Config>
    configs;

  loadDirectory(options, L"Dependencies",configs);
 /* loadDirectory(options.rootDirectory + L"OptionalDependencies",configs);
  loadCoders(options,configs);
  loadProject(options,L"demos",L"ImageMagick\\Magick++\\demo",configs);
  loadProject(options,L"Magick++",L"ImageMagick\\Magick++",configs);
  if (options.version == ImageMagickVersion::ImageMagick7)
  {
    loadProject(options,L"MagickCore",L"ImageMagick\\MagickCore",configs);
    loadProject(options,L"MagickWand",L"ImageMagick\\MagickWand",configs);
    loadProject(options,L"oss-fuzz",L"ImageMagick\\oss-fuzz",configs);
  }
  else
  {
    loadProject(options,L"MagickCore",L"ImageMagick\\magick",configs);
    loadProject(options,L"MagickWand",L"ImageMagick\\wand",configs);
  }
  loadProject(options,L"utilities",L"ImageMagick\\utilities",configs);*/
  
  validate(configs,options);

  return(configs);
}

void Configs::loadCoders(const ConfigureOptions &options,std::vector<Config> &configs) 
{
  //auto
  //  coderConfig=loadProjectConfig(options,L"\\coders",L"\\ImageMagick\\coders");

  //if (!coderConfig)
  //  return;

  //Config config=*coderConfig;

  //if (options.buildType == BuildType::Static)
  //{
  //  configs.push_back(config);
  //  return;
  //}

  //for (const auto& entry : std::filesystem::directory_iterator(config.directory()))
  //{
  //  if (!entry.is_regular_file() || entry.path().extension() != ".c")
  //    continue;
  //  
  //  std::wstring name=entry.path().filename().wstring();
  //  std::wstring configFile=config.configDirectory() + L"\\Config." + name + L".txt";
  //  
  //  std::optional<Config> moduleConfig=std::nullopt;
  //  if (std::filesystem::exists(configFile))
  //    moduleConfig=Config::load(config.directory(),configFile);

  //  throwException(L"CREATE MODULE CONFIG");
  //}
}

void Configs::loadDirectory(const ConfigureOptions &options,const std::wstring directory,std::vector<Config> &configs) 
{
  std::wstring fullProjectDirectory=options.rootDirectory + L"\\" + directory;
  if (!std::filesystem::exists(fullProjectDirectory))
    return;

  for (const auto& entry : std::filesystem::directory_iterator(fullProjectDirectory))
  {
    if (entry.is_directory())
    { 
      std::wstring name=entry.path().filename().wstring();
      std::wstring projectDirectory=directory + L"\\" + entry.path().filename().wstring() + L"\\";
      std::wstring configFile=options.rootDirectory + L"\\" + projectDirectory + L".ImageMagick\\Config.txt";

      Config config=Config::load(name,projectDirectory,configFile);
      addConfig(config,options,configs);
    }
  }
}

void Configs::loadProject(const ConfigureOptions &options,const std::wstring &name,const std::wstring &directory,std::vector<Config> &configs)
{
 /* auto config=loadProjectConfig(options,name,directory);

  if (!config)
    return;

  configs.push_back(*config);*/
}

std::optional<Config> Configs::loadProjectConfig(const ConfigureOptions &options,const std::wstring &name,const std::wstring &directory)
{
  std::wstring
    projectDirectory=options.rootDirectory + L"Projects\\" + name;

  if (!std::filesystem::exists(projectDirectory))
    return(std::nullopt);

  return(Config::load(name,options.rootDirectory + directory,projectDirectory + L"\\Config.txt"));
}

void Configs::validate(const std::vector<Config> &configs,const ConfigureOptions &options)
{
  //
  //
  //
  //
  // CHECK IF THE DEFINES CAN BE FOUND IN FILES
  //
  //
  //
  for (const auto& config : configs)
  {
    for (const auto& include : config.includes())
    {
      std::wstring includeDirectory = options.rootDirectory + config.directory() + include;
      if (!std::filesystem::exists(includeDirectory))
        throwException(L"Include directory does not exist: " + includeDirectory);
    }

    if (!config.references().empty() && config.type() == ProjectType::StaticLibrary)
      throwException(L"A static library should have no references");
  }
}