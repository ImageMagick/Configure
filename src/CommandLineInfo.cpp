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
#include "CommandLineInfo.h"

CommandLineInfo::CommandLineInfo(Options &options)
{
  _options=&options;
  showWizard=true;
}

void CommandLineInfo::ParseParam(const wchar_t* pszParam, BOOL bFlag, BOOL bLast)
{
  if (!bFlag)
    return;

  if (_wcsicmp(pszParam, L"arm64") == 0)
    _options->architecture=Architecture::Arm64;
  else if (_wcsicmp(pszParam, L"deprecated") == 0)
    _options->excludeDeprecated=FALSE;
  else if (_wcsicmp(pszParam, L"dynamic") == 0)
    _options->isStaticBuild=FALSE;
  else if (_wcsicmp(pszParam, L"hdri") == 0)
    _options->useHDRI=TRUE;
  else if (_wcsicmp(pszParam, L"incompatibleLicense") == 0)
     _options->includeIncompatibleLicense=TRUE;
  else if (_wcsicmp(pszParam, L"includeOptional") == 0)
    _options->includeOptional=TRUE;
  else if (_wcsicmp(pszParam, L"installedSupport") == 0)
    _options->installedSupport=TRUE;
  else if (_wcsicmp(pszParam, L"noDpc") == 0)
    _options->enableDpc=FALSE;
  else if (_wcsicmp(pszParam, L"noHdri") == 0)
    _options->useHDRI=FALSE;
  else if (_wcsicmp(pszParam, L"noOpenMP") == 0)
    _options->useOpenMP=FALSE;
  else if (_wcsicmp(pszParam, L"noWizard") == 0)
    showWizard=false;
  else if (_wcsicmp(pszParam, L"limitedPolicy") == 0)
    _options->policyConfig=PolicyConfig::Limited;
  else if (_wcsicmp(pszParam, L"linkRuntime") == 0)
    _options->linkRuntime=TRUE;
  else if (_wcsicmp(pszParam, L"openCL") == 0)
    _options->useOpenCL=TRUE;
  else if (_wcsicmp(pszParam, L"openPolicy") == 0)
    _options->policyConfig=PolicyConfig::Open;
  else if (_wcsicmp(pszParam, L"Q8") == 0)
    _options->quantumDepth=QuantumDepth::Q8;
  else if (_wcsicmp(pszParam, L"Q16") == 0)
    _options->quantumDepth=QuantumDepth::Q16;
  else if (_wcsicmp(pszParam, L"Q32") == 0)
    _options->quantumDepth=QuantumDepth::Q32;
  else if (_wcsicmp(pszParam, L"Q64") == 0)
    _options->quantumDepth=QuantumDepth::Q64;
  else if (_wcsicmp(pszParam, L"securePolicy") == 0)
    _options->policyConfig=PolicyConfig::Secure;
  else if (_wcsicmp(pszParam, L"static") == 0)
    _options->isStaticBuild=TRUE;
  else if (_wcsicmp(pszParam, L"x86") == 0)
    _options->architecture=Architecture::x86;
  else if (_wcsicmp(pszParam, L"x64") == 0)
    _options->architecture=Architecture::x64;
  else if (_wcsicmp(pszParam, L"VS2017") == 0)
    _options->visualStudioVersion=VisualStudioVersion::VS2017;
  else if (_wcsicmp(pszParam, L"VS2019") == 0)
    _options->visualStudioVersion=VisualStudioVersion::VS2019;
  else if (_wcsicmp(pszParam, L"VS2022") == 0)
    _options->visualStudioVersion=VisualStudioVersion::VS2022;
  else if (_wcsicmp(pszParam, L"webSafePolicy") == 0)
    _options->policyConfig=PolicyConfig::WebSafe;
  else if (_wcsicmp(pszParam, L"zeroConfigurationSupport") == 0)
    _options->zeroConfigurationSupport=TRUE;
}
