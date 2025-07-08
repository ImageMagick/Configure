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

#include "Notice.h"

void Notice::write(const Options &options,const VersionInfo &versionInfo)
{
  wofstream notice(options.rootDirectory + L"Artifacts\\NOTICE.txt");
  if (!notice)
    throwException(L"Unable to open NOTICE.txt");

  notice << "[ Imagemagick " << versionInfo.version() << versionInfo.libAddendum() << " (" << versionInfo.releaseDate() << ") ]" << endl << endl;
  notice << readLicense(options.rootDirectory + L"ImageMagick\\LICENSE") << endl << endl;

  wstring licensesDirectory=options.rootDirectory + L"Artifacts\\license\\";
  for (const auto& entry : filesystem::directory_iterator(licensesDirectory))
  {
    if (!entry.is_regular_file())
      continue;

    notice << readLicense(entry.path().wstring()) << endl << endl;
  }
}

const wstring Notice::readLicense(const wstring &fileName)
{
  wifstream file(fileName);
  if (!file)
    throwException(L"Unable to open license file: " + fileName);

  return(trim(wstring((istreambuf_iterator<wchar_t>(file)),istreambuf_iterator<wchar_t>())));
}
