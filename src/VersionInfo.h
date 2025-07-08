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

class VersionInfo
{
public:
  const wstring version() const;

  const wstring libAddendum() const;

  const wstring releaseDate() const { return _releaseDate; }

  static optional<VersionInfo> load(const Options& options);

  void write() const;

  void write(wstring inputFile,wstring outputFile) const;

private:
  VersionInfo(const Options& options);

  const wstring fullVersion() const;

  const wstring interfaceVersion() const;

  const wstring libVersionNumber() const;

  const wstring ppInterfaceVersion() const;

  const wstring ppLibVersionNumber() const;

  const wstring quantumDepthBits() const;

  const wstring versionNumber() const;

  const wstring visualStudioVersionName() const;

  const wstring executeCommand(const wstring &command) const;
  
  const wstring getFileModificationDate(const wstring &fileName,const wstring &format) const;
  
  void load(const wstring fileName);

  void loadValue(const wstring &line,const wstring &keyword,wstring *value) const;

  wstring replaceVariable(const wstring &line,const size_t offset) const;

  wstring replaceVariable(const wstring& line,const size_t offset,const size_t length,const wstring &newValue) const;

  void setGitRevision();

  void setReleaseDate();

  wstring _gitRevision;
  wstring _isBeta;
  wstring _libraryCurrent;
  wstring _libraryRevision;
  wstring _libraryAge;
  wstring _libVersion;
  wstring _major;
  wstring _minor;
  wstring _micro;
  const Options _options;
  wstring _patchlevel;
  wstring _ppLibraryCurrent;
  wstring _ppLibraryRevision;
  wstring _ppLibraryAge;
  wstring _releaseDate;
};
