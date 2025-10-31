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

#include "License.h"

void License::write(const Options &options,const Config &config)
{
  auto& version=config.version();
  if (version.isEmpty())
    throwException(L"Missing version information for: " + config.name());

  const auto targetDirectory=options.licenseArtifactsDirectory();
  filesystem::create_directories(targetDirectory);

  const auto sourceFileName=options.rootDirectory + config.directory() + config.licenseFile();
  wifstream sourceLicenseFile(sourceFileName);
  if (!sourceLicenseFile)
    throwException(L"Failed to open license file: " + sourceFileName);

  wstring content((std::istreambuf_iterator<wchar_t>(sourceLicenseFile)),std::istreambuf_iterator<wchar_t>());

  wofstream licenseFile(targetDirectory + config.name() + L".txt");
  licenseFile << L"[ " << config.productName() << L" " << version.fullVersion() << L" (" << config.releaseDate() << L") ]" << endl << endl;
  licenseFile << trim(content) << endl;
}

void License::writeNonWindowsLicenses(const Options &options)
{
  auto directory=L"Dependencies\\NonWindowsDependencies\\";
  if (!filesystem::exists(options.rootDirectory + directory))
    directory=L"NonWindowsDependencies\\";

  if (!filesystem::exists(options.rootDirectory + directory))
    return;

  for (const auto& entry : filesystem::directory_iterator(options.rootDirectory + directory))
  {
    if (!entry.is_directory())
      continue;

    auto name=entry.path().filename().wstring();
    auto projectDirectory=directory + name + L"\\";
    auto configFile=options.rootDirectory + projectDirectory + L".ImageMagick\\Config.txt";
    auto config=Config::load(name,projectDirectory,configFile);
    
    License::write(options,config);
  }
}
