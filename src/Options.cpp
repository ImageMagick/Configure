#include "Options.h"

Options::Options(const wstring &rootDirectory)
  : rootDirectory(rootDirectory)
{
#if _M_IX86
  architecture=Architecture::x86;
#elif _M_ARM64
  architecture=Architecture::Arm64;
#else
  architecture=Architecture::x64;
#endif
  enableDpc=TRUE;
  excludeDeprecated=TRUE;
#ifdef DEBUG
  includeIncompatibleLicense=TRUE;
  includeOptional=TRUE;
#else
  includeIncompatibleLicense=FALSE;
  includeOptional=FALSE;
#endif
  installedSupport=FALSE;
  isImageMagick7=TRUE;
  isStaticBuild=FALSE;
  linkRuntime=FALSE;
  policyConfig=PolicyConfig::Open;
  quantumDepth=QuantumDepth::Q16;
  useHDRI=TRUE;
  useOpenCL=TRUE;
  useOpenMP=TRUE;
  visualStudioVersion=getVisualStudioVersion();
  zeroConfigurationSupport=FALSE;

  wifstream preBuildFile(rootDirectory + L"Artifacts\\pre-build-libs.txt");
  if (preBuildFile)
  {
    wstring
      line;

    while (getline(preBuildFile,line))
    {
      line=trim(line);
      if (!line.empty())
        _preBuildLibs.insert(line);
    }
  }
}

const wstring Options::architectureName() const
{
  switch (architecture)
  {
  case Architecture::x86: return(L"x86");
  case Architecture::x64: return(L"x64");
  case Architecture::Arm64: return(L"arm64");
  default: throwException(L"Unknown architecture");
  }
}

const wstring Options::channelMaskDepth() const
{
  if (!isImageMagick7)
    return(L"");

  if ((visualStudioVersion >= VisualStudioVersion::VS2022) && (architecture != Architecture::x86))
    return(L"64");
  else
    return(L"32");
}

const wstring Options::platform() const
{
  switch (architecture)
  {
  case Architecture::x86: return(L"Win32");
  case Architecture::x64: return(L"x64");
  case Architecture::Arm64: return(L"ARM64");
  default: throwException(L"Unknown architecture");
  }
}

const wstring Options::projectsDirectory() const
{
  return(L"ProjectFiles\\" + architectureName() + L"\\");
}

void Options::checkImageMagickVersion()
{
  if (filesystem::exists(rootDirectory + L"\\ImageMagick\\magick"))
  {
    isImageMagick7=FALSE;
    useHDRI=FALSE;
  }
}

wstring Options::getEnvironmentVariable(const wchar_t *name)
{
  wchar_t
    *buffer;

  size_t
    length;

  wstring
    value;

  if (_wdupenv_s(&buffer,&length,name) == 0)
  {
    if ((buffer != (wchar_t *) NULL) && (length > 0))
    {
      value=wstring(buffer);
      free(buffer);
      return(value);
    }
  }

  return(value);
}

VisualStudioVersion Options::getVisualStudioVersion()
{
  if (hasVisualStudioFolder(L"2022"))
    return(VisualStudioVersion::VS2022);
  else if (hasVisualStudioFolder(L"2019"))
    return(VisualStudioVersion::VS2019);
  else if (hasVisualStudioFolder(L"2017"))
    return(VisualStudioVersion::VS2017);
  else
    return(VSLATEST);
}

bool Options::hasVisualStudioFolder(const wchar_t *name)
{
  wstring
    path;

  path=getEnvironmentVariable(L"ProgramW6432") + L"\\Microsoft Visual Studio\\" + name;
  if (filesystem::exists(path))
    return(true);
  path=getEnvironmentVariable(L"ProgramFiles(x86)") + L"\\Microsoft Visual Studio\\" + name;
  return(filesystem::exists(path) ? true : false);
}
