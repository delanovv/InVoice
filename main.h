#ifndef _MAIN_H_
#define _MAIN_H_

#include <WinSock2.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")
#include <Windows.h>
#include <memory>
#include <kthook/kthook.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <string>
#include "sampapi/CChat.h"
#include "sampapi/CInput.h"
#include "sampapi/CGame.h"
#include "sampapi/CLocalPlayer.h"
#include "sampapi/CNetGame.h"
#include <stdio.h>
#include <vector>
#pragma comment(lib,"winmm.lib")
#include <mmsystem.h>
#include <Mmreg.h>
#include "image/ByteMicActive.h"
#include "image/micon.h"
#include "image/pngwin.h"
#include "image/monserratbold.h"
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <functiondiscoverykeys_devpkey.h>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <iostream>
#include <sstream>
#include <cstring>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <boost/asio.hpp>
#include "audio/bass.h"
using CTimer__UpdateSignature = void(__cdecl*)();
using PresentSignature = HRESULT(__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
using ResetSignature = HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
using WndProcSignature = HRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM);
using CHud__DrawRadar = void(__cdecl*)();
namespace samp = sampapi::v037r3;

IDirect3DTexture9* micOn;
IDirect3DTexture9* micActive;
IDirect3DTexture9* micOff;

kthook::kthook_simple<CTimer__UpdateSignature> CTimerHook{};
kthook::kthook_signal<PresentSignature> PresentHook{};
kthook::kthook_signal<ResetSignature> ResetHook{};
kthook::kthook_simple<WndProcSignature> WndProcHook{};
kthook::kthook_signal<CHud__DrawRadar> samp_init_hook{ reinterpret_cast<void*>(0x58A330) };

ImFont* fontVoice{};
bool voiceSet = false;
bool ImGuiEnable = true;
char ImGuiInputBuffer[256];
char messageBuffer[256] = "";
bool voiceMoving = false;
int selectedMic = 0;
int selectedMi = 0;
std::vector<const char*> microphones;
std::vector<const char*> myMic;
float volumeMic = 1;
const char* items = u8"Микрофон 1\0Микрофон 2\0Микрофон 3\0";
void sampAddChatMessage(const char* message) {
    samp::RefChat()->AddMessage(0xFFFFFFFF, message);
}

std::string convert1251toUTF8(unsigned char ch) {
    std::string utf8str;
    if (ch < 0x80) {
        // символ ASCII, кодируется одним байтом в UTF-8
        utf8str = std::string(1, ch);
    }
    else {
        switch (ch) {
        case 0xA3: utf8str = u8"Ґ"; break;
        case 0xA6: utf8str = u8"Ё"; break;
        case 0xA8: utf8str = u8"Є"; break;
        case 0xAD: utf8str = u8"Ї"; break;
        case 0xB0: utf8str = u8"І"; break;
        case 0xB1: utf8str = u8"і"; break;
        case 0xB2: utf8str = u8"ґ"; break;
        case 0xB6: utf8str = u8"ё"; break;
        case 0xB8: utf8str = u8"є"; break;
        case 0xBD: utf8str = u8"ї"; break;
        case 0xC0: utf8str = u8"А"; break;
        case 0xC1: utf8str = u8"Б"; break;
        case 0xC2: utf8str = u8"В"; break;
        case 0xC3: utf8str = u8"Г"; break;
        case 0xC4: utf8str = u8"Д"; break;
        case 0xC5: utf8str = u8"Е"; break;
        case 0xC6: utf8str = u8"Ж"; break;
        case 0xC7: utf8str = u8"З"; break;
        case 0xC8: utf8str = u8"И"; break;
        case 0xC9: utf8str = u8"Й"; break;
        case 0xCA: utf8str = u8"К"; break;
        case 0xCB: utf8str = u8"Л"; break;
        case 0xCC: utf8str = u8"М"; break;
        case 0xCD: utf8str = u8"Н"; break;
        case 0xCE: utf8str = u8"О"; break;
        case 0xCF: utf8str = u8"П"; break;
        case 0xD0: utf8str = u8"Р"; break;
        case 0xD1: utf8str = u8"С"; break;
        case 0xD2: utf8str = u8"Т"; break;
        case 0xD3: utf8str = u8"У"; break;
        case 0xD4: utf8str = u8"Ф"; break;
        case 0xD5: utf8str = u8"Х"; break;
        case 0xD6: utf8str = u8"Ц"; break;
        case 0xD7: utf8str = u8"Ч"; break;
        case 0xD8: utf8str = u8"Ш"; break;
        case 0xD9: utf8str = u8"Щ"; break;
        case 0xDA: utf8str = u8"Ъ"; break;
        case 0xDB: utf8str = u8"Ы"; break;
        case 0xDC: utf8str = u8"Ь"; break;
        case 0xDD: utf8str = u8"Э"; break;
        case 0xDE: utf8str = u8"Ю"; break;
        case 0xDF: utf8str = u8"Я"; break;
        case 0xE0: utf8str = u8"а"; break;
        case 0xE1: utf8str = u8"б"; break;
        case 0xE2: utf8str = u8"в"; break;
        case 0xE3: utf8str = u8"г"; break;
        case 0xE4: utf8str = u8"д"; break;
        case 0xE5: utf8str = u8"е"; break;
        case 0xE6: utf8str = u8"ж"; break;
        case 0xE7: utf8str = u8"з"; break;
        case 0xE8: utf8str = u8"и"; break;
        case 0xE9: utf8str = u8"й"; break;
        case 0xEA: utf8str = u8"к"; break;
        case 0xEB: utf8str = u8"л"; break;
        case 0xEC: utf8str = u8"м"; break;
        case 0xED: utf8str = u8"н"; break;
        case 0xEE: utf8str = u8"о"; break;
        case 0xEF: utf8str = u8"п"; break;
        case 0xF0: utf8str = u8"р"; break;
        case 0xF1: utf8str = u8"с"; break;
        case 0xF2: utf8str = u8"т"; break;
        case 0xF3: utf8str = u8"у"; break;
        case 0xF4: utf8str = u8"ф"; break;
        case 0xF5: utf8str = u8"х"; break;
        case 0xF6: utf8str = u8"ц"; break;
        case 0xF7: utf8str = u8"ч"; break;
        case 0xF8: utf8str = u8"ш"; break;
        case 0xF9: utf8str = u8"щ"; break;
        case 0xFA: utf8str = u8"ъ"; break;
        case 0xFB: utf8str = u8"ы"; break;
        case 0xFC: utf8str = u8"ь"; break;
        case 0xFD: utf8str = u8"э"; break;
        case 0xFE: utf8str = u8"ю"; break;
        case 0xFF: utf8str = u8"я"; break;
        default: utf8str = ""; break;
        }
    }
    return utf8str;
}


#endif // !_MAIN_H_