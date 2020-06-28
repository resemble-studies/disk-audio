/* License: MIT. See LICENSE in root directory. */

#include <ensemble/disk-audio.hpp>

using Ensemble::DiskAudio;

int main()
{
    auto pDiskAudio = new DiskAudio;

    pDiskAudio->Eject();
}
