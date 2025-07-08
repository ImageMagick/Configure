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
#include "MagickBaseConfig.h"

void MagickBaseConfig::write(const Options &options)
{
  wifstream configIn(options.rootDirectory + L"ProjectConfigs\\MagickCore\\magick-baseconfig.h.in");
  if (!configIn)
    throwException(L"Unable to open magick-baseconfig.h.in");

  wstring directory=options.rootDirectory + L"\\ImageMagick\\" + options.magickCoreName();
  wofstream configOut(directory + L"\\magick-baseconfig.h");
  if (!configOut)
    throwException(L"Unable to open magick-baseconfig.h");

  wstring line;
  while (getline(configIn,line))
  {
    if (trim(line).compare(L"$$CONFIG$$") != 0)
    {
      configOut << line << endl;
      continue;
    }

    configOut << "/*" << endl;
    configOut << "  Define to build a ImageMagick which uses registry settings or" << endl;
    configOut << "  hard-coded paths to locate installed components.  This supports" << endl;
    configOut << "  using the \"setup.exe\" style installer, or using hard-coded path" << endl;
    configOut << "  definitions (see below).  If you want to be able to simply copy" << endl;
    configOut << "  the built ImageMagick to any directory on any directory on any machine," << endl;
    configOut << "  then do not use this setting." << endl;
    configOut << "*/" << endl;
    if (options.installedSupport)
      configOut << "#define MAGICKCORE_INSTALLED_SUPPORT" << endl;
    else
      configOut << "#undef MAGICKCORE_INSTALLED_SUPPORT" << endl;
    configOut << endl;

    configOut << "/*" << endl;
    configOut << "  Specify size of PixelPacket color Quantums (8, 16, or 32)." << endl;
    configOut << "  A value of 8 uses half the memory than 16 and typically runs 30% faster," << endl;
    configOut << "  but provides 256 times less color resolution than a value of 16." << endl;
    configOut << "*/" << endl;
    if (options.quantumDepth == QuantumDepth::Q8)
      configOut << "#define MAGICKCORE_QUANTUM_DEPTH 8" << endl;
    else if (options.quantumDepth == QuantumDepth::Q16)
      configOut << "#define MAGICKCORE_QUANTUM_DEPTH 16" << endl;
    else if (options.quantumDepth == QuantumDepth::Q32)
      configOut << "#define MAGICKCORE_QUANTUM_DEPTH 32" << endl;
    else if (options.quantumDepth == QuantumDepth::Q64)
      configOut << "#define MAGICKCORE_QUANTUM_DEPTH 64" << endl;
    configOut << endl;

    if (options.channelMaskDepth() != L"")
    {
      configOut << "/*" << endl;
      configOut << "  Channel mask depth" << endl;
      configOut << "*/" << endl;
      configOut << "#define MAGICKCORE_CHANNEL_MASK_DEPTH " << options.channelMaskDepth() << endl;
      configOut << endl;
    }

    configOut << "/*" << endl;
    configOut << "  Define to enable high dynamic range imagery (HDRI)" << endl;
    configOut << "*/" << endl;
    if (options.useHDRI)
      configOut << "#define MAGICKCORE_HDRI_ENABLE 1" << endl;
    else
      configOut << "#define MAGICKCORE_HDRI_ENABLE 0" << endl;
    configOut << endl;

    configOut << "/*" << endl;
    configOut << "  Define to enable OpenCL" << endl;
    configOut << "*/" << endl;
    if (options.useOpenCL)
      configOut << "#define MAGICKCORE_HAVE_CL_CL_H" << endl;
    else
      configOut << "#undef MAGICKCORE_HAVE_CL_CL_H" << endl;
    configOut << endl;

    configOut << "/*" << endl;
    configOut << "  Define to enable Distributed Pixel Cache" << endl;
    configOut << "*/" << endl;
    if (options.enableDpc)
      configOut << "#define MAGICKCORE_DPC_SUPPORT" << endl;
    else
      configOut << "#undef MAGICKCORE_DPC_SUPPORT" << endl;
    configOut << endl;

    configOut << "/*" << endl;
    configOut << "  Exclude deprecated methods in MagickCore API" << endl;
    configOut << "*/" << endl;
    if (options.excludeDeprecated)
      configOut << "#define MAGICKCORE_EXCLUDE_DEPRECATED" << endl;
    else
      configOut << "#undef MAGICKCORE_EXCLUDE_DEPRECATED" << endl;
    configOut << endl;

    configOut << "/*" << endl;
    configOut << "  Define to only use the built-in (in-memory) settings." << endl;
    configOut << "*/" << endl;
    if (options.zeroConfigurationSupport)
      configOut << "#define MAGICKCORE_ZERO_CONFIGURATION_SUPPORT 1" << endl;
    else
      configOut << "#define MAGICKCORE_ZERO_CONFIGURATION_SUPPORT 0" << endl;
    configOut << endl;

    for (const auto& entry : filesystem::directory_iterator(options.rootDirectory + L"Artifacts\\config"))
    {
      if (!entry.is_regular_file() || !endsWith(entry.path().filename(),L".h"))
        continue;

      wstring fileName=entry.path().wstring();
      wifstream versionFile(fileName);
      if (!versionFile)
        throwException(L"Unable to open: " + fileName);
   
      configOut << versionFile.rdbuf() << endl;
    }
  }
}
