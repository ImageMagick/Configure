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
#include "Project.h"
#include "Projects.h"
#include "Solution.h"

BEGIN_MESSAGE_MAP(ConfigureApp, CWinApp)
  ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

ConfigureApp theApp;

ConfigureApp::ConfigureApp()
{
}

BOOL ConfigureApp::InitInstance()
{
  bool
    attachedConsole;

  ConfigureOptions
    options(std::filesystem::current_path().parent_path().wstring() + L"\\");

  CommandLineInfo info=CommandLineInfo(options);
  ParseCommandLine(info);

  attachedConsole=attachConsole();

  if (info.showWizard && !attachedConsole)
  {
    ConfigureWizard
      wizard;

    wizard.setOptions(options);

    if (wizard.DoModal() != ID_WIZFINISH)
      return(FALSE);
  }

  if (!attachedConsole)
    return(createFiles(options));

  try
  {
    return(createFiles(options));
  }
  catch (std::exception ex)
  {
    std::cerr << "Exception caught: " << ex.what() << std::endl;
    return(FALSE);
  } 
}

bool ConfigureApp::attachConsole()
{
  FILE
    *cerr,
    *cout;

  if (!AttachConsole(ATTACH_PARENT_PROCESS)) 
    return(false);

  freopen_s(&cout,"CONOUT$","w",stdout);
  freopen_s(&cerr,"CONOUT$","w",stderr);

  std::cout << std::endl << "Console attached successfully." << std::endl;
  return(true);
}

BOOL ConfigureApp::createFiles(ConfigureOptions &options)
{
  std::vector<Config> configs=Configs::load(options);
  std::vector<Project> projects=Projects::create(configs,options);
  Projects::write(projects);
  Solution::write(projects,options);

  //
  //
  //
  //
  // WRITE THE NOTICE FILE!
  //
  //
  //
  //

  //

  /*Solution
    solution(wizard);

  WaitDialog
    waitDialog;

  CommandLineInfo info=CommandLineInfo(wizard);
  ParseCommandLine(info);

  wizard.parseCommandLineInfo(info);

  solution.loadProjects();*/

  //response=ID_WIZFINISH;
  //if (info.noWizard() == FALSE)
  //  response=wizard.DoModal();

  //solution.write(waitDialog);

  //std::vector<ConfigFile> configFiles = ConfigFiles::load(L"D:/ImageMagick/ImageMagick-Windows",options);

  return(TRUE);
}