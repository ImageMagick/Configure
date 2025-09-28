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
#include "Version.h"

Version::Version(const wstring& major,const wstring& minor,const wstring& patch,const wstring& metadata)
{
  _major=major;
  _minor=minor;
  _patch=patch;
  _metadata=metadata;
}

Version Version::empty()
{
  return(Version(L"",L"",L"",L""));
}

Version Version::parse(const wstring& value)
{
  auto parts=split(value,L"+");

  wstring metadata;
  if (parts.size() > 1)
    metadata=parts[1];

  parts=split(parts[0],L".");
  if (parts.size() != 3)
    throwException(L"Invalid version: " + value);

  return(Version(parts[0],parts[1],parts[2],metadata));
}
