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

void Configs::addConfig(Config &config,const ConfigureOptions &options,vector<Config> &configs)
{
  if (options.version == ImageMagickVersion::ImageMagick6 && config.isMagickProject())
    return;

  if (!options.includeOptional && config.isOptional())
    return;

  if (!options.includeIncompatibleLicense && config.hasIncompatibleLicense())
    return;

  configs.push_back(config);
}

vector<Config> Configs::load(const ConfigureOptions &options)
{
  vector<Config>
    configs;

  loadDirectory(options,L"Dependencies",configs);
  loadDirectory(options,L"OptionalDependencies",configs);
  if (options.version == ImageMagickVersion::ImageMagick7)
  {
    loadProject(options,L"MagickCore",L"ImageMagick\\MagickCore",configs);
    loadProject(options,L"MagickWand",L"ImageMagick\\MagickWand",configs);
    loadProject(options,L"oss-fuzz",L"ImageMagick\\oss-fuzz",configs);
  }
  else
  {
    loadProject(options,L"magick",L"ImageMagick\\magick",configs);
    loadProject(options,L"wand",L"ImageMagick\\wand",configs);
  }
  loadProject(options,L"Magick++",L"ImageMagick\\Magick++",configs);
  loadCoders(options,configs);
  //loadProject(options,L"demos",L"ImageMagick\\Magick++\\demo",configs);
  loadProject(options,L"utilities",L"ImageMagick\\utilities",configs);
  
  validate(options,configs);

  return(configs);
}

void Configs::loadCoders(const ConfigureOptions &options,vector<Config> &configs) 
{
  vector<Config>
    coders;

  wstring coderDirectory=L"ImageMagick\\coders\\";
  if (!filesystem::exists(options.rootDirectory + coderDirectory))
    return;

  wstring coderProjectsDirectory=options.rootDirectory + L"Projects\\coders\\";
  for (const auto& entry : filesystem::directory_iterator(coderProjectsDirectory))
  {
    if (!entry.is_regular_file() || !endsWith(entry.path().filename(),L".txt"))
      continue;
    
    wstring name=entry.path().stem().wstring().substr(6);
    if (name.empty())
      name=L"coders";
    else
      name=name.substr(1);

    Config config=Config::load(name,coderDirectory,entry.path().wstring());
    addConfig(config,options,configs);
  }
}

void Configs::loadDirectory(const ConfigureOptions &options,const wstring directory,vector<Config> &configs) 
{
  wstring fullProjectDirectory=options.rootDirectory + L"\\" + directory;
  if (!filesystem::exists(fullProjectDirectory))
    return;

  for (const auto& entry : filesystem::directory_iterator(fullProjectDirectory))
  {
    if (entry.is_directory())
    {
      wstring name=entry.path().filename().wstring();
      wstring projectDirectory=directory + L"\\" + entry.path().filename().wstring() + L"\\";
      wstring configFile=options.rootDirectory + L"\\" + projectDirectory + L".ImageMagick\\Config.txt";

      Config config=Config::load(name,projectDirectory,configFile);
      addConfig(config,options,configs);
    }
  }
}

void Configs::loadProject(const ConfigureOptions &options,const wstring &name,const wstring &directory,vector<Config> &configs)
{
  optional<Config> config=loadProjectConfig(options,name,directory);

  if (!config)
    return;

  addConfig(*config,options,configs);
}

optional<Config> Configs::loadProjectConfig(const ConfigureOptions &options,const wstring &name,const wstring &directory)
{
  wstring projectDirectory=options.rootDirectory + L"Projects\\" + name;
  if (!filesystem::exists(projectDirectory))
    return(nullopt);

  return(Config::load(name,directory + L"\\",projectDirectory + L"\\Config.txt"));
}

void Configs::validate(const ConfigureOptions &options,const vector<Config> &configs)
{
  for (const auto& config : configs)
  {
    for (const auto& include : config.includes())
    {
      wstring includeDirectory = options.rootDirectory;
      if (include[0] == L'\\')
        includeDirectory+=include.substr(1); 
      else
        includeDirectory+=config.directory() + include;

      if (!filesystem::exists(includeDirectory))
        throwException(L"Include directory does not exist: " + includeDirectory);
    }

    if (!config.references().empty() && config.type() == ProjectType::StaticLibrary)
      throwException(L"A static library should have no references");
  }
}
