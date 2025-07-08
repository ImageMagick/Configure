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
#include "XmlConfigFiles.h"

const wstring XmlConfigFiles::getPolicyFileName(const Options &options)
{
  switch(options.policyConfig)
  {
    case PolicyConfig::Limited: return L"policy-limited.xml";
    case PolicyConfig::Open: return L"policy-open.xml";
    case PolicyConfig::Secure: return L"policy-secure.xml";
    case PolicyConfig::WebSafe: return L"policy-websafe.xml";
    default: throwException(L"Unknown policy configuration type.");
  }
}

void XmlConfigFiles::write(const Options &options)
{
  const wstring configFolder=options.rootDirectory + L"ImageMagick\\config\\";
  const wstring targetFolder=options.rootDirectory + L"Artifacts\\bin\\";

  filesystem::copy_file(configFolder + getPolicyFileName(options),targetFolder + L"policy.xml",filesystem::copy_options::overwrite_existing);

  vector<wstring> xmlFiles = { L"colors.xml", L"english.xml", L"locale.xml", L"log.xml", L"mime.xml", L"thresholds.xml" };
  for (auto& xmlFile : xmlFiles)
    filesystem::copy_file(configFolder + xmlFile,targetFolder + xmlFile,filesystem::copy_options::overwrite_existing);
}
