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
#pragma once
#include "stdafx.h"

class GitRepositoryInfo
{
public:
  GitRepositoryInfo() = default;

  static const GitRepositoryInfo load(const wstring &directory);

  const wstring commit() const { return(_commit); }

  const wstring link() const
  {
    if (_remoteUrl.empty() || _commit.empty())
      return(L"");

    return(_remoteUrl + L"/tree/" + _commit);
  }

  const wstring remoteUrl() const { return(_remoteUrl); }

private:
  wstring _commit;
  wstring _remoteUrl;
};
