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
#include "ResourceFiles.h"

void ResourceFiles::write(const Options& options, vector<Config>& configs)
{
  const auto targetDirectory=options.resourceArtifactsDirectory();
  filesystem::create_directories(targetDirectory);

  for (const auto& config : configs)
  {
    auto& version = config.version();
    if (version.isEmpty())
      continue;

    const auto resourceFileName=targetDirectory + config.name() + L".rc";
    wofstream resourceFile(resourceFileName);

    resourceFile << L"#include \"winver.h\"" << endl;
    resourceFile << L"#define DELEGATE_VERSION_NUM " << version.major() << L"," << version.minor() << L"," << version.patch() << endl;
    resourceFile << L"#define DELEGATE_VERSION_STRING \"" << version.major() << L"." << version.minor() << L"." << version.patch() << L" (" << config.releaseDate() << L")\"" << endl;
    resourceFile << L"VS_VERSION_INFO VERSIONINFO" << endl;
    resourceFile << L" FILEVERSION DELEGATE_VERSION_NUM" << endl;
    resourceFile << L" PRODUCTVERSION DELEGATE_VERSION_NUM" << endl;
    resourceFile << L" FILEFLAGSMASK 0x3fL" << endl;
    resourceFile << L"#ifdef _DEBUG" << endl;
    resourceFile << L" FILEFLAGS 0x1L" << endl;
    resourceFile << L"#else" << endl;
    resourceFile << L" FILEFLAGS 0x0L" << endl;
    resourceFile << L"#endif" << endl;
    resourceFile << L" FILEOS 0x40004L" << endl;
    resourceFile << L" FILETYPE VFT_DLL" << endl;
    resourceFile << L" FILESUBTYPE 0x0L" << endl;
    resourceFile << L"BEGIN" << endl;
    resourceFile << L"  BLOCK \"StringFileInfo\"" << endl;
    resourceFile << L"  BEGIN" << endl;
    resourceFile << L"    BLOCK \"040904b0\"" << endl;
    resourceFile << L"    BEGIN" << endl;
    resourceFile << L"      VALUE \"ProductName\", \"" << config.productName() << "\\0\"" << endl;
    resourceFile << L"      VALUE \"FileDescription\", \"ImageMagick Studio library\\0\"" << endl;
    resourceFile << L"      VALUE \"OriginalFilename\", \"" << config.productName() << "\\0\"" << endl;
    resourceFile << L"      VALUE \"InternalName\", \"ImageMagick Studio\\0\"" << endl;
    resourceFile << L"      VALUE \"FileVersion\", DELEGATE_VERSION_STRING \"\\0\"" << endl;
    resourceFile << L"      VALUE \"ProductVersion\", DELEGATE_VERSION_STRING \"\\0\"" << endl;
    resourceFile << L"      VALUE \"CompanyName\", \"" << config.companyName() << "\\0\"" << endl;
    resourceFile << L"      VALUE \"LegalCopyright\", \"" << config.copyright() << "\\0\"" << endl;
    resourceFile << L"      VALUE \"Comments\", \"" << config.url() << "\\0\"" << endl;
    resourceFile << L"    END" << endl;
    resourceFile << L"  END" << endl;
    resourceFile << L"  BLOCK \"VarFileInfo\"" << endl;
    resourceFile << L"  BEGIN" << endl;
    resourceFile << L"    VALUE \"Translation\", 0x409, 1200" << endl;
    resourceFile << L"  END" << endl;
    resourceFile << L"END" << endl;
  }
}
