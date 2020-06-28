# Ensemble Disk Audio

A simple [Win32 MCICDA](https://docs.microsoft.com/en-us/windows/win32/multimedia) encapsulation, reversed from the Genie engine.

## Usage

```c++
#include <ensemble/disk-audio.hpp>

using Ensemble::DiskAudio;
```

```c++
auto pDiskAudio = new DiskAudio;

pDiskAudio->Eject();
```

## References

 - Jeff Prosise, "Programming Utilities: CDPLAYER Makes Your CD-ROM Drive Sing," PC Mag, vol. 11, no. 20, pp. 433–442, Nov. 24, 1992. ISSN: 0888-8507.
 - Giancarlo Iovino, "MCI Sample," MFC Programmer's Sourcebook, Code Guru, 1998.
 - James Boer, "Redbook Audio Playback for Windows," Game Audio Programming, pp. 574-586, 2002. ISBN: 1-58450-245-2.
 - Zhang Jing, Liang Shu, "Chapter 6 Multimedia Audio," Windows Multimedia Programming Basics, pp. 154-166, 2005.
