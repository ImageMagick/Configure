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

#include "Pages\WelcomePage.h"
#include "Pages\TargetPage.h"
#include "Pages\FinishedPage.h"
//#include "Shared.h"
////
////class CommandLineInfo;
////
class ConfigureWizard : public CPropertySheet
{
  DECLARE_DYNAMIC(ConfigureWizard)

public:
  ConfigureWizard(CWnd* pWndParent = (CWnd *) NULL);

  virtual ~ConfigureWizard();

  void setOptions(ConfigureOptions& options)
  {
    _targetPage.setOptions(options);
  }

////
////  const wstring binDirectory() const;
////
////  const wstring channelMaskDepth() const;
////
////  bool enableDpc() const;
////
////  bool excludeAliases() const;
////
////  bool excludeDeprecated() const;
////
////  bool includeIncompatibleLicense() const;
////
////  bool includeOptional() const;
////
////  bool installedSupport() const;
////
////  bool isImageMagick7() const;
////
////  const wstring machineName() const;
////
////  const wstring magickCoreProjectName() const;
////
////  Platform platform() const;
////
////  const wstring platformName() const;
////
////  const wstring platformAlias() const;
////
////  PolicyConfig policyConfig() const;
////
////  QuantumDepth quantumDepth() const;
////
////  const wstring quantumDepthBits() const;
////
////  const wstring solutionName() const;
////
////  SolutionType solutionType() const;
////
////  bool useHDRI() const;
////
////  bool useOpenCL() const;
////
////  bool useOpenMP() const;
////
////  void updateProjectNames(wstring &value) const;
////
////  VisualStudioVersion visualStudioVersion() const;
////
////  const wstring visualStudioVersionName() const;
////
////  bool zeroConfigurationSupport() const;
////
////  void parseCommandLineInfo(const CommandLineInfo &info);
////
protected:

  DECLARE_MESSAGE_MAP()

private:
  FinishedPage _finishedPage;
  TargetPage _targetPage;
  WelcomePage _welcomePage;
};

