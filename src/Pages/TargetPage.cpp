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
#include "TargetPage.h"

IMPLEMENT_DYNCREATE(TargetPage, CPropertyPage)

TargetPage::TargetPage() : CPropertyPage(IDD_TARGET_PAGE)
{
}

void TargetPage::setOptions(Options &options)
{
  _options=&options;
}

void TargetPage::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);

  DDX_CBIndex(pDX,IDC_QUANTUM_DEPTH,(int&) _options->quantumDepth);
  DDX_CBIndex(pDX,IDC_PLATFORM,(int&) _options->architecture);
  DDX_CBIndex(pDX,IDC_VISUALSTUDIO,(int&) _options->visualStudioVersion);
  DDX_CBIndex(pDX,IDC_POLICYCONFIG,(int&) _options->policyConfig);
  DDX_Radio(pDX,IDC_PROJECT_DYNAMIC, _options->isStaticBuild);
  DDX_Check(pDX,IDC_LINK_RUNTIME,_options->linkRuntime);
  DDX_Check(pDX,IDC_HDRI,_options->useHDRI);
  DDX_Check(pDX,IDC_OPEN_MP,_options->useOpenMP);
  DDX_Check(pDX,IDC_OPEN_CL,_options->useOpenCL);
  DDX_Check(pDX,IDC_ENABLE_DPC,_options->enableDpc);
  DDX_Check(pDX,IDC_INCLUDE_INCOMPATIBLE,_options->includeIncompatibleLicense);
  DDX_Check(pDX,IDC_INCLUDE_OPTIONAL,_options->includeOptional);
  DDX_Check(pDX,IDC_EXCLUDE_DEPRECATED,_options->excludeDeprecated);
  DDX_Check(pDX,IDC_INSTALLED_SUPPORT,_options->installedSupport);
  DDX_Check(pDX,IDC_ZERO_CONFIGURATION_SUPPORT,_options->zeroConfigurationSupport);
}

BOOL TargetPage::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  UpdateData(FALSE);

  return(TRUE);
}

BEGIN_MESSAGE_MAP(TargetPage, CPropertyPage)
END_MESSAGE_MAP()
