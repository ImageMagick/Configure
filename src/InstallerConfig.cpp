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

#include "InstallerConfig.h"

void InstallerConfig::write(const Options &options,const VersionInfo &versionInfo)
{
  if (!filesystem::exists(options.rootDirectory + L"Installer"))
    return;

  wstring configFileName=L"Installer\\Inno\\config.isx";
  versionInfo.write(L"Installer\\Inno\\config.isx.in",configFileName);

  wofstream configFile(options.rootDirectory + configFileName,ios::app);

  if (options.isStaticBuild)
  {
    configFile << L"#define public MagickStaticPackage 1" << endl;
  }
  else
  {
    configFile << L"#define public MagickDynamicPackage 1" << endl;
    if (options.architecture != Architecture::Arm64)
      configFile << L"#define public MagickPerlMagick 1" << endl;
  }

  switch (options.architecture)
  {
    case Architecture::Arm64:
      configFile << L"#define public MagickArm64Architecture 1" << endl;
      break;
    case Architecture::x64:
      configFile << L"#define public Magick64BitArchitecture 1" << endl;
      break;
  }

  if (options.useHDRI)
    configFile << L"#define public MagickHDRI 1" << endl;

  if (options.isImageMagick7)
    configFile << L"#define public MagickVersion7 1" << endl;
}
