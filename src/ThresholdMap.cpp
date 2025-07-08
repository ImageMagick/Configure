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
#include "ThresholdMap.h"

void ThresholdMap::write(const Options &options)
{
  if (!options.zeroConfigurationSupport)
    return;

  wifstream thresholds(options.rootDirectory + L"Artifacts\\bin\\thresholds.xml");
  if (!thresholds)
    throwException(L"Unable to open thresholds.xml");

  wofstream thresholdMap(options.rootDirectory + L"ImageMagick\\" + options.magickCoreName() + L"\\threshold-map.h");
  if (!thresholdMap)
    throwException(L"Unable to open threshold-map.h");

  thresholdMap << "static const char *const BuiltinMap=" << endl;

  wstring line;
  while (getline(thresholds,line))
  {
    if (line.length() == 0)
      continue;

    line=replace(line,L"\"",L"\\\"");
    thresholdMap << "\"" << line << "\"" << endl;
  }

  thresholdMap << ";";
}
