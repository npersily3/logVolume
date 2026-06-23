#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <conio.h> // Windows specific
#include <iostream>

int main() {
    // 1. Initialize COM library
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) return 1;

    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioEndpointVolume* pAudioEndpointVolume = NULL;

    // 2. Create device enumerator
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);

    // 3. Get current active default audio output
    if (SUCCEEDED(hr)) {
        hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
    }

    // 4. Activate the endpoint volume control
    if (SUCCEEDED(hr)) {
        hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndpointVolume);
    }

    // 5. Set volume scalar (0.0 to 1.0)
    if (SUCCEEDED(hr)) {

        float newVolume = 0.50f; // 50%
        float increment = newVolume/2;
        hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);

        std::cout << "Press a to log decrease, d to log increase, s reset, q to quit (everytime the volume has changed, play a video/audio in a new tabe) right now the audio is at 50 percent)" << std::endl;

        while (true) {


            if (_kbhit()) { // Check if a key has been pressed without blocking
                char ch = _getch(); // Read the character directly
                std::cout << "Key pressed: " << ch << "\n";
                if (ch == 'q') break;

                if (ch == 'a') {

                    newVolume -= increment;
                    increment /= 2;
                    std::cout << "decreased:" << newVolume << "percent of total\n";
                } else if (ch == 'd') {
                    newVolume += increment;
                    increment /= 2;
                    std::cout << "increased:" << newVolume << "percent of total\n";
                } else if (ch == 's') {
                    std::cout << "reset to :" << newVolume << "percent of total\n";
                    newVolume = 0.5f;
                    increment = newVolume/2;
                }
                hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
            }

        }

    }

    // Clean up resources
    if (pAudioEndpointVolume) pAudioEndpointVolume->Release();
    if (pDevice) pDevice->Release();
    if (pEnumerator) pEnumerator->Release();
    CoUninitialize();

    return 0;
}