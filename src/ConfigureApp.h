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

#include "Options.h"
#include "VersionInfo.h"
#include "WaitDialog.h"

class ConfigureApp : public CWinApp
{
public:
  ConfigureApp();

  virtual BOOL InitInstance();

  DECLARE_MESSAGE_MAP()

private:

  void cleanupFolders(Options &options,WaitDialog &waitDialog) const;

  BOOL createFiles(Options &options,WaitDialog &waitDialog) const;

  const wstring getRootDirectory() const;

  void tryAttachConsole();

  void writeImageMagickFiles(const Options &options,const VersionInfo &versionInfo,WaitDialog &waitDialog) const;
};
