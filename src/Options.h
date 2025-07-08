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
#pragma once
#include "stdafx.h"

class Options
{
public:
  Options(const wstring &rootDirectory);

  Architecture architecture;
  BOOL enableDpc;
  BOOL excludeDeprecated;
  BOOL includeIncompatibleLicense;
  BOOL includeOptional;
  BOOL installedSupport;
  BOOL isStaticBuild;
  BOOL linkRuntime;
  PolicyConfig policyConfig;
  QuantumDepth quantumDepth;
  wstring rootDirectory;
  BOOL useHDRI;
  BOOL useOpenCL;
  BOOL useOpenMP;
  bool isImageMagick7;
  VisualStudioVersion visualStudioVersion;
  BOOL zeroConfigurationSupport;

  const wstring architectureName() const;

  const set<wstring>& preBuildLibs() const { return(_preBuildLibs); };

  const wstring channelMaskDepth() const;

  const wstring magickCoreName() const { return(isImageMagick7 ? L"MagickCore" : L"magick"); };

  const wstring platform() const;

  const wstring projectsDirectory() const;

  void checkImageMagickVersion();

private:
  static wstring getEnvironmentVariable(const wchar_t *name);

  static VisualStudioVersion getVisualStudioVersion();
  
  static bool hasVisualStudioFolder(const wchar_t *name);

  set<wstring> _preBuildLibs;
};
