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

class Version
{
public:
  const wstring fullVersion() const { return(_major + L"." + _minor + L"." + _patch + (_metadata.empty() ? L"" : L"+" + _metadata)); }

  bool isEmpty() const { return(_major.empty() && _minor.empty() && _patch.empty() && _metadata.empty()); }

  const wstring numericVersion() const { return(_major + L"," + _minor + L"," + _patch); }

  static Version empty();

  static Version parse(const wstring& value);

private:
  Version(const wstring& major,const wstring& minor,const wstring& patch,const wstring& metadata);

  wstring _major;
  wstring _minor;
  wstring _patch;
  wstring _metadata;
};

