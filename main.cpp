#include "main.h"
#include "voice.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HRESULT WndProc(const decltype(WndProcHook)& hook, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYUP) {
        if (wParam == VK_F9) {
            voiceSet = { !voiceSet };
            if (voiceSet == true) {
                samp::RefGame()->SetCursorMode(2, false);
            }
            else {
                samp::RefGame()->SetCursorMode(0, false);
            }
        }
    }

    if (uMsg == WM_KEYDOWN) {
        if (wParam == 'X') { // Проверяем, была ли нажата клавиша X
            voiceMoving = true; // Если да, устанавливаем переменную voiceMoving в true
        }
    }
    else if (uMsg == WM_KEYUP) {
        if (wParam == 'X') { // Проверяем, была ли отпущена клавиша X
            voiceMoving = false; // Если да, устанавливаем переменную voiceMoving в false
        }
    }

    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
        return 1;
    }
    hook.get_trampoline()(hwnd, uMsg, wParam, lParam);
}


void apply_custom_style() {
    ImGuiStyle* style = &ImGui::GetStyle();

    style->WindowPadding = ImVec2(15, 15);
    style->WindowRounding = 5.0f;
    style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    //style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    //style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
}

std::optional<HRESULT> D3D9Present(const decltype(PresentHook)& hook, IDirect3DDevice9* pDevice, CONST RECT* pSrcRect, CONST RECT* pDestRect, HWND hDestWindow, CONST RGNDATA* pDirtyRegion) {
    static bool ImGuiInit{};
    if (!ImGuiInit) {
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(**reinterpret_cast<HWND**>(0xC17054));
        ImGui_ImplDX9_Init(pDevice);
        ImGui::GetIO().IniFilename = nullptr;
#pragma warning(push)
#pragma warning(disable: 4996)
        std::string font{ getenv("WINDIR") }; font += "\\Fonts\\Arialbd.TTF";

#pragma warning(pop)
        ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16.5f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

        fontVoice = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(monserratbold, sizeof(monserratbold), 22.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

        apply_custom_style();

        auto latest_wndproc_ptr = GetWindowLongPtrW(**reinterpret_cast<HWND**>(0xC17054), GWLP_WNDPROC);
        WndProcHook.set_dest(latest_wndproc_ptr);
        WndProcHook.set_cb(&WndProc);
        WndProcHook.install();

        if (micOn == nullptr || micOff == nullptr) {
            D3DXCreateTextureFromFileInMemory(pDevice, ByteMicActive, sizeof(ByteMicActive), &micActive);
            D3DXCreateTextureFromFileInMemory(pDevice, ByteMicOf, sizeof(ByteMicOf), &micOff);
            D3DXCreateTextureFromFileInMemory(pDevice, ByteMicOn, sizeof(ByteMicOn), &micOn);
        }

        ImGuiInit = { true };
    }


    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (voiceSet) {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 3, ImGui::GetIO().DisplaySize.y / 3), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(750.f, 400.f), ImGuiCond_Always);
        ImGui::Begin(u8"Голосовой Чат VoiceCC", &voiceSet, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

        if (ImGui::CollapsingHeader(u8"Звук")) {
            ImGui::Combo(u8"Выбор микрофона", &selectedMic, items);
            ImGui::SliderFloat(u8"Громкость микрофона", &volumeMic, 0.f, 1.f);
        }

        ImGui::End();
    }

    if (ImGuiEnable) {
        ImGui::SetNextWindowPos(ImVec2(0.f, 760.f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(350.f, 140.f), ImGuiCond_Always);
        ImGui::Begin("VoiceCC Alpha", &ImGuiEnable, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);


        //ImGui::Checkbox("Mute", &test);

        static float voiceVolume = 0.5f;

        if (voiceMoving) {
            ImGui::Image(micActive, ImVec2(75, 75));
        }
        else {
            ImGui::Image(micOn, ImVec2(75, 75));

        }

        ImGui::SameLine();

        ImGui::PushFont(fontVoice);

        ImGui::Text(u8"Нажмите F9\nЧтобы открыть настройки");

        ImGui::PopFont();

        //ImGui::Image(micOff, ImVec2(50, 50));

        //ImGui::SliderFloat("Volume", &voiceVolume, 0.0f, 1.0f);

        ImGui::End();
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    return std::nullopt;
}

std::optional<HRESULT> D3D9Lost(const decltype(ResetHook)& hook, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentParams) {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    return std::nullopt;
}

void D3D9Reset(const decltype(ResetHook)& hook, HRESULT& return_value, IDirect3DDevice9* device_ptr, D3DPRESENT_PARAMETERS* parameters) {
    ImGui_ImplDX9_InvalidateDeviceObjects();
}

void setD3D9Hooks() {
    DWORD pDevice = *reinterpret_cast<DWORD*>(0xC97C28);
    void** vTable = *reinterpret_cast<void***>(pDevice);

    PresentHook.set_dest(vTable[17]);
    PresentHook.before.connect(&D3D9Present);
    PresentHook.install();

    ResetHook.set_dest(vTable[16]);
    ResetHook.before.connect(&D3D9Lost);
    ResetHook.after.connect(&D3D9Reset);
    ResetHook.install();
}


HRESULT getMicrofones() {
    setlocale(LC_ALL, "rus");
    HRESULT hr;
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDeviceCollection* pDevices = NULL;
    IMMDevice* pDevice = NULL;
    LPWSTR pwszID = NULL;
    IPropertyStore* pProps = NULL;
    PROPVARIANT varName;
    hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        return hr;
    }

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        CoUninitialize();
        return hr;
    }

    hr = pEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &pDevices);
    if (FAILED(hr)) {
        pEnumerator->Release();
        CoUninitialize();
        return hr;
    }

    UINT count;
    hr = pDevices->GetCount(&count);
    if (FAILED(hr)) {
        pDevices->Release();
        pEnumerator->Release();
        CoUninitialize();
        return hr;
    }
    for (UINT i = 0; i < count; i++) {
        hr = pDevices->Item(i, &pDevice);
        if (FAILED(hr)) {
            continue;
        }

        hr = pDevice->GetId(&pwszID);
        if (FAILED(hr)) {
            pDevice->Release();
            continue;
        }

        hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
        if (FAILED(hr)) {
            CoTaskMemFree(pwszID);
            pDevice->Release();
            continue;
        }

        PropVariantInit(&varName);
        hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
        if (FAILED(hr)) {
            pProps->Release();
            CoTaskMemFree(pwszID);
            pDevice->Release();
            continue;
        }

        size_t size = wcslen(varName.pwszVal) + 1;
        char* buffer = new char[size];
        wcstombs(buffer, varName.pwszVal, size);
        microphones.push_back(buffer);

        PropVariantClear(&varName);
        pProps->Release();
        CoTaskMemFree(pwszID);
        pDevice->Release();
    }

    pDevices->Release();
    pEnumerator->Release();
    CoUninitialize();

    return S_OK;
}

const char* convertVectorToCString(const std::vector<const char*>& vec) {
    std::vector<char> result;
    for (const char* item : vec) {
        for (const char* currentChar = item; *currentChar != '\0'; ++currentChar) {
            std::string utf8Char = convert1251toUTF8(*currentChar);
            result.insert(result.end(), utf8Char.begin(), utf8Char.end());
        }
        result.push_back('\0');
    }
    result.push_back('\0');

    char* cString = new char[result.size()];
    memcpy(cString, result.data(), result.size());

    return cString;
}

VoiceClient* voiceClient;

void move() {
    voiceClient = new VoiceClient();
    voiceClient->setReadyMic(selectedMic);
    if (voiceClient != nullptr) {
        voiceClient->talk();
    }
    else {
        sampAddChatMessage("Сервер недоступен");
    }
}

void main() {
    static bool inited = false;
    namespace samp = sampapi::v037r3;
    if (!inited && samp::RefChat() != nullptr && samp::RefNetGame() != nullptr) {
        getMicrofones();
        items = convertVectorToCString(microphones);
        inited = true;
        move();
    }
}




void CTimer__Update(const decltype(CTimerHook)& hook) {
    static bool init{};
    if (!init) {
        setD3D9Hooks();
        init = { true };
    }
    if (init) {
        main();
    }

    hook.get_trampoline()();
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CTimerHook.set_dest(0x561B10);
        CTimerHook.set_cb(&CTimer__Update);
        CTimerHook.install();
        break;
    case DLL_PROCESS_DETACH:
        delete voiceClient;
        break;
    }
    return TRUE;
}
