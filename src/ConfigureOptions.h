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

class ConfigureOptions
{
public:
  ConfigureOptions(const std::wstring &rootDirectory);

  const std::wstring architectureName() const;

  const std::wstring platform() const;

  const std::wstring projectsDirectory() const;

  Architecture architecture;
  BuildType buildType;
  BOOL enableDpc;
  BOOL excludeAliases;
  BOOL excludeDeprecated;
  BOOL includeIncompatibleLicense;
  BOOL includeOptional;
  BOOL installedSupport;
  BOOL linkRuntime;
  PolicyConfig policyConfig;
  QuantumDepth quantumDepth;
  std::wstring rootDirectory;
  BOOL useHDRI;
  BOOL useOpenCL;
  BOOL useOpenMP;
  ImageMagickVersion version;
  VisualStudioVersion visualStudioVersion;
  BOOL zeroConfigurationSupport;

  void setImageMagickVersion6();

private:
  const std::wstring buildTypeName() const;

  static std::wstring getEnvironmentVariable(const wchar_t *name);

  static VisualStudioVersion getVisualStudioVersion();
  
  static bool hasVisualStudioFolder(const wchar_t *name);
};

