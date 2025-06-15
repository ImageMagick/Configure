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
#include "ConfigureApp.h"

#include "Configs.h"
#include "ConfigureOptions.h"
#include "ConfigureWizard.h"
#include "CommandLineInfo.h"
#include "InstallerConfig.h"
#include "MagickBaseConfig.h"
#include "Notice.h"
#include "PerlMagick.h"
#include "Project.h"
#include "Projects.h"
#include "Solution.h"
#include "ThresholdMap.h"
#include "XmlConfigFiles.h"

BEGIN_MESSAGE_MAP(ConfigureApp, CWinApp)
  ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

ConfigureApp theApp;

ConfigureApp::ConfigureApp()
{
}

BOOL ConfigureApp::InitInstance()
{
  bool attachedConsole=attachConsole();

  try
  {
    ConfigureOptions options(getRootDirectory());
    options.checkImageMagickVersion();

    CommandLineInfo info=CommandLineInfo(options);
    ParseCommandLine(info);

    if (info.showWizard && !attachedConsole)
    {
      ConfigureWizard
        wizard;

      wizard.setOptions(options);

      if (wizard.DoModal() != ID_WIZFINISH)
        return(FALSE);
    }

    cleanupFolders(options);
    return(createFiles(options));
  }
  catch (exception ex)
  {
    if (attachedConsole)
      cerr << "Exception caught: " << ex.what() << endl;
    return(FALSE);
  } 
}

bool ConfigureApp::attachConsole()
{
  FILE
    *cerr_file,
    *cout_file;

  if (!AttachConsole(ATTACH_PARENT_PROCESS)) 
    return(false);

  freopen_s(&cout_file,"CONOUT$","w",stdout);
  freopen_s(&cerr_file,"CONOUT$","w",stderr);

  cout << endl << "Console attached successfully." << endl;
  return(true);
}

void ConfigureApp::cleanupFolders(ConfigureOptions &options) const
{
  filesystem::remove_all(options.rootDirectory + L"Artifacts\\config");
  filesystem::remove_all(options.rootDirectory + L"Artifacts\\demo");
  filesystem::remove_all(options.rootDirectory + L"Artifacts\\fuzz");
  filesystem::remove_all(options.rootDirectory + L"Artifacts\\include");
  filesystem::remove_all(options.rootDirectory + L"Artifacts\\license");
}

BOOL ConfigureApp::createFiles(ConfigureOptions &options) const
{
  vector<Config> configs=Configs::load(options);
  vector<Project> projects=Projects::create(options,configs);
  Projects::write(projects);
  Solution::write(options,projects);

  optional<VersionInfo> versionInfo=VersionInfo::load(options);
  if (versionInfo)
    writeImageMagickFiles(options, *versionInfo);

  return(TRUE);
}

const wstring ConfigureApp::getRootDirectory() const
{
  filesystem::path directory=filesystem::current_path();
  while (directory.has_parent_path())
  {
    if (directory.filename() == L"Configure")
      return directory.parent_path().wstring() + L"\\";

    directory=directory.parent_path();
  }

  throwException(L"Cannot find root directory for ConfigureApp.");
}

void ConfigureApp::writeImageMagickFiles(const ConfigureOptions &options,const VersionInfo &versionInfo) const
{
  versionInfo.write(options);
  MagickBaseConfig::write(options);
  ThresholdMap::write(options);
  XmlConfigFiles::write(options);
  PerlMagick::configure(options);
  Notice::write(options,versionInfo);
  InstallerConfig::write(options,versionInfo);
}
