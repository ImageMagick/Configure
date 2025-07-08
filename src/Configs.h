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

#include "Config.h"
#include "Options.h"

class Configs
{
public:
  static vector<Config> load(const Options &options);

private:
  static void addConfig(Config &config,const Options &options,vector<Config> &configs);

  static void loadCoders(const Options &options,vector<Config> &configs);

  static void loadDirectory(const Options &options,const wstring directory,vector<Config> &configs);

  static void loadProject(const Options &options,const wstring &name,const wstring &directory,vector<Config> &configs);

  static Config loadProjectConfig(const Options &options,const wstring &name,const wstring &directory);

  static void removeInvalidReferences(const Options &options,vector<Config> &configs);

  static void validate(const Options &options,const vector<Config> &configs);
};
