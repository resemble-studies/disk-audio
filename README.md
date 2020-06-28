# Ensemble Disk Audio

A simple [Win32 MCICDA](https://docs.microsoft.com/en-us/windows/win32/multimedia) encapsulation, reversed from the Genie engine.

## Usage

```c++
#include <ensemble/disk-audio.hpp>

using Ensemble::DiskAudio;
```

```c++
auto* d = new DiskAudio;

d->Eject();
```
