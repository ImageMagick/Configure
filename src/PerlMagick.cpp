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
#include "PerlMagick.h"

void PerlMagick::configure(const Options &options)
{
  filesystem::copy_file(options.rootDirectory + L"Build\\PerlMagick\\Zip.ps1", options.rootDirectory + L"ImageMagick\\PerlMagick\\Zip.ps1",filesystem::copy_options::overwrite_existing);

  wifstream makeFileIn(options.rootDirectory + L"Build\\PerlMagick\\Makefile.PL.in");
  if (!makeFileIn)
    throwException(L"Unable to open Makefile.PL.in for reading.");

  wofstream makeFile(options.rootDirectory + L"ImageMagick\\PerlMagick\\Makefile.PL");
  if (!makeFile)
    throwException(L"Unable to open Makefile.PL for writing.");

  wstring libName=magickCoreLibraryName(options);

  wstring line;
  while (getline(makeFileIn,line))
  {
    line=replace(line,L"$$LIB_NAME$$",libName);
    line=replace(line,L"$$PLATFORM$$",options.architectureName());
    makeFile << line << endl;
  }
}

wstring PerlMagick::magickCoreLibraryName(const Options &options)
{
  return(L"CORE_RL_" + options.magickCoreName() + L"_.a");
}
