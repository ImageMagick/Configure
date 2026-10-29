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
%  Unless required by applicable law or agreed to in writing, software        %
%  distributed under the License is distributed on an "AS IS" BASIS,          %
%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
%  See the License for the specific language governing permissions and        %
%  limitations under the License.                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/
#include "GitRepositoryInfo.h"
#include "CommandRunner.h"

const GitRepositoryInfo GitRepositoryInfo::load(const wstring &directory)
{
  GitRepositoryInfo repositoryInfo;
  const CommandRunner commandRunner(directory);
  auto remoteUrl=trim(commandRunner.execute(L"git remote get-url origin"));
  if (startsWith(remoteUrl,L"git@github.com:"))
    remoteUrl=L"https://github.com/" + remoteUrl.substr(15);
  else if (startsWith(remoteUrl,L"ssh://git@github.com/"))
    remoteUrl=L"https://github.com/" + remoteUrl.substr(21);

  const auto scheme=remoteUrl.find(L"://");
  if (scheme != wstring::npos)
  {
    const auto authorityStart=scheme + 3;
    const auto authorityEnd=remoteUrl.find(L"/",authorityStart);
    const auto credentialsEnd=remoteUrl.find(L"@",authorityStart);
    if (credentialsEnd != wstring::npos && (authorityEnd == wstring::npos || credentialsEnd < authorityEnd))
      remoteUrl.erase(authorityStart,credentialsEnd - authorityStart + 1);
  }

  while (endsWith(remoteUrl,L"/"))
    remoteUrl.pop_back();
  if (endsWith(remoteUrl,L".git"))
    remoteUrl.erase(remoteUrl.length() - 4);

  repositoryInfo._remoteUrl=remoteUrl;
  repositoryInfo._commit=trim(commandRunner.execute(L"git rev-parse HEAD"));
  return(repositoryInfo);
}
