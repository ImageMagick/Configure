/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%  Copyright @ 1999 ImageMagick Studio LLC, a non-profit organization         %
%  dedicated to making software imaging solutions freely available.           %
%                                                                             %
%  You may not use this file except in compliance with the License.  You may  %
%  obtain a copy of the License at                                            %
%                                                                             %
%    https://www.imagemagick.org/license/                                     %
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

  ConfigureApp(const ConfigureApp&) = delete;

  ConfigureApp& operator=(const ConfigureApp&) = delete;

  virtual BOOL InitInstance();

  DECLARE_MESSAGE_MAP()

private:
  static void cleanupDirectories(Options &options);

  static void copyFiles(Options &options);

  static void copyFiles(const wstring &sourceDirectory,const wstring &targetDirectory);

  BOOL createFiles(Options &options,WaitDialog &waitDialog) const;

  const wstring getRootDirectory() const;

  void writeImageMagickFiles(const Options &options,const VersionInfo &versionInfo,WaitDialog &waitDialog) const;
};
