/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%  Copyright @ 1999 ImageMagick Studio LLC, a non-profit organization         %
%  dedicated to making software imaging solutions freely available.           %
%                                                                             %
%  You may not use this file except in compliance with the License.  You may  %
%  obtain a copy of the License at                                            %
%                                                                             %
%    https://imagemagick.org/license/                                         %
%                                                                             %
%  distributed under the License is distributed on an "AS IS" BASIS,          %
%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
%  See the License for the specific language governing permissions and        %
%  limitations under the License.                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/
#include "CommandRunner.h"

CommandRunner::CommandRunner(const wstring &workingDirectory)
  : _workingDirectory(filesystem::path(workingDirectory).lexically_normal().wstring())
{
}

const wstring CommandRunner::execute(const wstring &command) const
{
  HANDLE hRead, hWrite;
  SECURITY_ATTRIBUTES sa={ sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

  if (!CreatePipe(&hRead,&hWrite,&sa,0))
    return(L"");

  SetHandleInformation(hRead,HANDLE_FLAG_INHERIT,0);

  PROCESS_INFORMATION pi = {};
  STARTUPINFOW si = {};
  si.cb=sizeof(si);
  si.dwFlags=STARTF_USESTDHANDLES;
  si.hStdOutput=hWrite;
  si.hStdError=hWrite;
  si.hStdInput=NULL;

  auto cmd=L"cmd.exe /C " + command;
  if (!CreateProcessW(NULL,cmd.data(),NULL,NULL,TRUE,CREATE_NO_WINDOW,NULL,_workingDirectory.c_str(),&si,&pi))
  {
    CloseHandle(hWrite);
    CloseHandle(hRead);
    return(L"");
  }
  CloseHandle(hWrite);

  wstring result;
  char buffer[4096] = { 0 };
  DWORD bytesRead;
  while (ReadFile(hRead,buffer,sizeof(buffer) - 1,&bytesRead,NULL) && bytesRead)
  {
      buffer[bytesRead] = '\0';
      int length=MultiByteToWideChar(CP_UTF8,0,buffer,-1,NULL,0);
      wstring data((unsigned int) (length - 1),0);
      MultiByteToWideChar(CP_UTF8,0,buffer,-1,&data[0],length);
      result+=data;
  }
  CloseHandle(hRead);

  DWORD exitCode;
  WaitForSingleObject(pi.hProcess,INFINITE);
  GetExitCodeProcess(pi.hProcess,&exitCode);

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  if (exitCode != 0)
    return L"";

  result=replace(result,L"\n",L"");
  return result;
}
