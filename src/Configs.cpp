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
#include "Configs.h"

void Configs::addConfig(Config &config,const Options &options,vector<Config> &configs)
{
  if (config.isOptional() && !options.includeOptional)
    return;

  if (config.hasIncompatibleLicense() && !options.includeIncompatibleLicense)
    return;

  if (config.disabledForArm64() && options.architecture == Architecture::Arm64)
    return;

  if (config.isImageMagick7Only() && !options.isImageMagick7)
    return;

  configs.push_back(config);
}

void Configs::correctDirectories(vector<Config> &configs)
{
  for(auto& config : configs)
    config.correctDirectory();
}

vector<Config> Configs::load(const Options &options)
{
  vector<Config>
    configs;

  loadDependencies(options,configs);
  loadImageMagick(options,configs);
  removeInvalidReferences(options,configs);
  validate(options,configs);

  return(configs);
}

void Configs::loadCoders(const Options &options,vector<Config> &configs)
{
  vector<Config>
    coders;

  const auto coderDirectory=L"ImageMagick\\coders\\";
  if (!filesystem::exists(options.rootDirectory + coderDirectory))
    throwException(L"Cannot find coders directory");

  const auto coderProjectsDirectory=options.rootDirectory + L"Configure\\Configs\\coders\\";
  for (const auto& entry : filesystem::directory_iterator(coderProjectsDirectory))
  {
    if (!entry.is_regular_file() || !endsWith(entry.path().filename(),L".txt"))
      continue;

    auto name=entry.path().stem().wstring().substr(6);
    if (name.empty())
      name=L"coders";
    else
      name=name.substr(1);

    Config config=Config::load(name,coderDirectory,entry.path().wstring());
    addConfig(config,options,configs);
  }
}

Config Configs::loadConfig(const Options &options,const wstring &name,const wstring &directory)
{
  const auto projectDirectory=options.rootDirectory + L"Configure\\Configs\\" + name;
  if (!filesystem::exists(projectDirectory))
    throwException(L"Cannot find project directory");

  return(Config::load(name,directory + L"\\",projectDirectory + L"\\Config.txt"));
}

void Configs::loadConfig(const Options &options,const wstring &name,const wstring &directory,vector<Config> &configs)
{
  Config config=loadConfig(options,name,directory);
  addConfig(config,options,configs);
}

void Configs::loadDependencies(const Options &options,vector<Config> &configs)
{
  if (!filesystem::exists(options.rootDirectory + L"Dependencies"))
    return;

  if (filesystem::exists(options.rootDirectory + L"Dependencies\\Dependencies"))
  {
    loadDirectory(options,L"Dependencies\\Dependencies",configs);
    loadDirectory(options,L"Dependencies\\OptionalDependencies",configs);
    correctDirectories(configs);
  }
  else
  {
    loadDirectory(options,L"Dependencies",configs);
    loadDirectory(options,L"OptionalDependencies",configs);
  }
}

void Configs::loadDirectory(const Options &options,const wstring directory,vector<Config> &configs)
{
  const auto fullProjectDirectory=options.rootDirectory + L"\\" + directory;
  if (!filesystem::exists(fullProjectDirectory))
    return;

  for (const auto& entry : filesystem::directory_iterator(fullProjectDirectory))
  {
    if (entry.is_directory())
    {
      const auto name=entry.path().filename().wstring();
      const auto projectDirectory=directory + L"\\" + entry.path().filename().wstring() + L"\\";
      const auto configFile=options.rootDirectory + L"\\" + projectDirectory + L".ImageMagick\\Config.txt";

      auto config=Config::load(name,projectDirectory,configFile);
      addConfig(config,options,configs);
    }
  }
}

void Configs::loadImageMagick(const Options &options,vector<Config> &configs)
{
  if (!filesystem::exists(options.rootDirectory + L"ImageMagick"))
    return;

  if (options.isImageMagick7)
  {
    loadConfig(options,L"MagickCore",L"ImageMagick\\MagickCore",configs);
    loadConfig(options,L"MagickWand",L"ImageMagick\\MagickWand",configs);
    loadConfig(options,L"oss-fuzz",L"ImageMagick\\oss-fuzz",configs);
  }
  else
  {
    loadConfig(options,L"MagickCore",L"ImageMagick\\magick",configs);
    loadConfig(options,L"wand",L"ImageMagick\\wand",configs);
  }

  loadCoders(options,configs);
  loadConfig(options,L"Magick++",L"ImageMagick\\Magick++",configs);
  loadConfig(options,L"demos",L"ImageMagick\\Magick++\\demo",configs);
  loadConfig(options,L"filters",L"ImageMagick\\filters",configs);
  loadConfig(options,L"utilities",L"ImageMagick\\utilities",configs);
  loadDirectory(options,L"OptionalApplications",configs);

  if (!options.isImageMagick7)
  {
    for (auto& config : configs)
      config.updateForImageMagick6();
  }
}

void Configs::removeInvalidReferences(const Options &options,vector<Config> &configs)
{
  for (auto& config : configs)
  {
    set<wstring> invalidReferences;
    for (auto& reference : config.references())
    {
      auto found = false;
      for (const auto& otherConfig : configs)
      {
        if (&otherConfig == &config)
          continue;

        if (otherConfig.isLibrary() && reference == otherConfig.name())
        {
          found = true;
          break;
        }
      }

      if (!found && !filesystem::exists(options.includeArtifactsDirectory() + reference))
        invalidReferences.insert(reference);
    }

    for (const auto& invalidReference : invalidReferences)
      config.removeReference(invalidReference);
  }
}

void Configs::validate(const Options &options,const vector<Config> &configs)
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
