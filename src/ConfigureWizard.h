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

class ConfigureWizard : public CPropertySheet
{
  DECLARE_DYNAMIC(ConfigureWizard)

public:
  ConfigureWizard(CWnd* pWndParent = (CWnd *) NULL);

  virtual ~ConfigureWizard();

  void setOptions(Options& options)
  {
    _targetPage.setOptions(options);
  }

protected:
  DECLARE_MESSAGE_MAP()

private:
  FinishedPage _finishedPage;
  TargetPage _targetPage;
  WelcomePage _welcomePage;
};
