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
#include "ConfigureWizard.h"
#include "CommandLineInfo.h"
#include "InstallerConfig.h"
#include "MagickBaseConfig.h"
#include "Notice.h"
#include "Options.h"
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
  tryAttachConsole();

  try
  {
    Options options(getRootDirectory());
    options.checkImageMagickVersion();

    CommandLineInfo info=CommandLineInfo(options);
    ParseCommandLine(info);

    if (info.showWizard)
    {
      ConfigureWizard
        wizard;

      wizard.setOptions(options);

      if (wizard.DoModal() != ID_WIZFINISH)
        return(FALSE);
    }
    
    WaitDialog waitDialog;
    waitDialog.setSteps(6);

    cleanupFolders(options,waitDialog);
    return(createFiles(options,waitDialog));
  }
  catch (exception ex)
  {
    cerr << "Exception caught: " << ex.what() << endl;
    return(FALSE);
  } 
}

void ConfigureApp::cleanupFolders(Options &options,WaitDialog &waitDialog) const
{
  waitDialog.nextStep(L"Cleaning up folders...");
  filesystem::remove_all(options.rootDirectory + L"Artifacts\\demo");
  filesystem::remove_all(options.rootDirectory + L"Artifacts\\fuzz");
  
#ifdef _DEBUG
  filesystem::remove_all(options.rootDirectory + L"Artifacts\\config");
  filesystem::remove_all(options.rootDirectory + L"Artifacts\\include");
  filesystem::remove_all(options.rootDirectory + L"Artifacts\\license");
#endif
}

BOOL ConfigureApp::createFiles(Options &options,WaitDialog &waitDialog) const
{
  waitDialog.nextStep(L"Loading configuration files...");
  vector<Config> configs=Configs::load(options);

  waitDialog.nextStep(L"Creating projects...");
  vector<Project> projects=Projects::create(options,configs);

  waitDialog.nextStep(L"Writing project files...");
  Projects::write(projects);

  waitDialog.nextStep(L"Writing solution files...");
  Solution::write(options,projects);

  waitDialog.nextStep(L"Loading version information...");
  optional<VersionInfo> versionInfo=VersionInfo::load(options);
  if (versionInfo)
    writeImageMagickFiles(options, *versionInfo,waitDialog);

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

void ConfigureApp::tryAttachConsole()
{
  if (!AttachConsole(ATTACH_PARENT_PROCESS))
    return;

  freopen_s((FILE**)stdout,"CONOUT$","w",stdout);
  freopen_s((FILE**)stderr,"CONOUT$","w",stderr);
  ios::sync_with_stdio();

  cout << endl << "Console attached successfully." << endl;
}

void ConfigureApp::writeImageMagickFiles(const Options &options,const VersionInfo &versionInfo,WaitDialog &waitDialog) const
{
  waitDialog.setSteps(7);

  waitDialog.nextStep(L"Writing version information...");
  versionInfo.write();

  waitDialog.nextStep(L"Writing magick-base-config.h...");
  MagickBaseConfig::write(options);

  waitDialog.nextStep(L"Writing threshold map...");
  ThresholdMap::write(options);

  waitDialog.nextStep(L"Writing xml config files...");
  XmlConfigFiles::write(options);

  waitDialog.nextStep(L"Writing PerlMagick configuration...");
  PerlMagick::configure(options);

  waitDialog.nextStep(L"Writing notice...");
  Notice::write(options,versionInfo);

  waitDialog.nextStep(L"Writing installer configuration...");
  InstallerConfig::write(options,versionInfo);
}
