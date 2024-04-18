#ifndef VOICE_H
#define VOICE_H
#include "audio/bass.h"
#include "main.h"
#include <opus.h>
#define IPADDRESS "127.0.0.1"
#define PORT "8080"
#define OPUS_MAX_FRAME_SIZE 960 * 6
#define OPUS_SAMPLE_RATE 48000
#define OPUS_FRAME_SIZE 960
#define OPUS_CHANNEL_COUNT 2
#define OPUS_APPLICATION OPUS_APPLICATION_AUDIO

using boost::asio::ip::tcp;

struct UDPMessage {
    std::string message;
    std::string type;
    bool isValid;
    std::string nickname;

    std::string toJSON() const {
        return "{\"message\": \"" + message + "\", \"type\": \"" + type + "\", \"nickname\": \"" + nickname + "\"}";
    }
};

class ConnectionException : public std::runtime_error {
public:
    ConnectionException(const std::string& message) : std::runtime_error(message) {}
};

class VoiceClient {
public:
    VoiceClient();
    ~VoiceClient();
    void initializeWinSock();
    void connectToServer(boost::asio::io_context& io_context, tcp::socket& socket);
    void playSound(const char* buffer, size_t bufferSize);
    void openMicrophone(UINT selectedMic);
    void prepareAudioBuffers();
    void cleanupAudio();
    void setReadyMic(UINT selectedMic);
    void startTalkingThread();
    void stopTalkingThread();
    void talk();


private:
    void talkLoop();
    void Talk();
    boost::asio::io_context io_context;
    tcp::socket socket;
    HWAVEIN wi;
    char buffers[2][48000 * 2 * 5 / 5];
    const char* sampNickname;
    WAVEHDR headers[2];
    std::thread talkingThread;
    std::atomic<bool> threadRunning;
    OpusEncoder* opusEncoder;
    OpusDecoder* opusDecoder;
    HSTREAM stream = BASS_StreamCreate(OPUS_SAMPLE_RATE, 2, BASS_LEVEL_STEREO, STREAMPROC_PUSH, nullptr);
    void playAudioWithBASS(const char* buffer, size_t bufferSize);
};

void VoiceClient::initializeWinSock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        sampAddChatMessage("Failed to initialize WinSock");
    }
}

VoiceClient::VoiceClient() : socket(io_context), headers{ {}, {} }, threadRunning(false) {
    initializeWinSock();
    BASS_Init(-1, 48000, 0, nullptr, nullptr);

    int opusError;
    opusEncoder = opus_encoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNEL_COUNT, OPUS_APPLICATION_VOIP, &opusError);
    if (opusError != OPUS_OK) {
        std::string str = std::to_string(opusError);
        sampAddChatMessage(str.c_str());
    }
    opusDecoder = opus_decoder_create(OPUS_SAMPLE_RATE, OPUS_CHANNEL_COUNT, &opusError);
    if (opusError != 0) {
        std::string str = std::to_string(opusError);
        sampAddChatMessage(str.c_str());
    }

    connectToServer(io_context, socket);
}

VoiceClient::~VoiceClient() {
    stopTalkingThread();

    waveInStop(wi);
    for (int i = 0; i < 2; ++i) {
        waveInUnprepareHeader(wi, &headers[i], sizeof(headers[i]));
    }
    opus_encoder_destroy(opusEncoder);
    opus_decoder_destroy(opusDecoder);
    waveInClose(wi);
    WSACleanup();
}

void VoiceClient::connectToServer(boost::asio::io_context& io_context, tcp::socket& socket) {
    try {
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(IPADDRESS, PORT);

        boost::asio::connect(socket, endpoints);
    }
    catch (const boost::system::system_error& e) {
        sampAddChatMessage("�� �� ������������ � ������� :(");
    }
}

void VoiceClient::playSound(const char* buffer, size_t bufferSize) {
    WAVEFORMATEX waveFormat;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 2;
    waveFormat.nSamplesPerSec = 48000;
    waveFormat.nAvgBytesPerSec = 48000 * 2 * 16 / 8;
    waveFormat.nBlockAlign = 2 * 16 / 8;
    waveFormat.wBitsPerSample = 16;
    waveFormat.cbSize = 0;

    HWAVEOUT hWaveOut;
    WAVEHDR WaveOutHdr;

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

    WaveOutHdr.lpData = const_cast<LPSTR>(buffer);
    WaveOutHdr.dwBufferLength = bufferSize;
    WaveOutHdr.dwBytesRecorded = 0;
    WaveOutHdr.dwFlags = 0L;


    waveOutPrepareHeader(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));

    waveOutSetVolume(hWaveOut, 0xFFFFFFFF);

    waveOutWrite(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));

    waveOutClose(hWaveOut);
}

void VoiceClient::openMicrophone(UINT selectedMic) {
    WAVEFORMATEX wfx = {};
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 2;
    wfx.nSamplesPerSec = 48000;
    wfx.wBitsPerSample = 16;
    wfx.nBlockAlign = wfx.wBitsPerSample * wfx.nChannels / 8;
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

    if (waveInOpen(&wi, selectedMic, &wfx, NULL, NULL, CALLBACK_NULL | WAVE_FORMAT_DIRECT) != MMSYSERR_NOERROR) {
        sampAddChatMessage("Failed to open microphone");
    }
}

void VoiceClient::prepareAudioBuffers() {
    for (int i = 0; i < 2; ++i) {
        headers[i].lpData = buffers[i];
        headers[i].dwBufferLength = 48000 * 2 * 5 / 5;
        if (waveInPrepareHeader(wi, &headers[i], sizeof(headers[i])) != MMSYSERR_NOERROR) {
            sampAddChatMessage("Failed to prepare audio buffer");
        }
        if (waveInAddBuffer(wi, &headers[i], sizeof(headers[i])) != MMSYSERR_NOERROR) {
            sampAddChatMessage("Failed to add audio buffer");
        }
    }
}

// ��������� � ����� ���������� �� ������ BASS
void PrintBASSErrorMessage() {
    int errorCode = BASS_ErrorGetCode();

    switch (errorCode) {
    case BASS_OK:
        sampAddChatMessage("BASS: ��� � �������");
        break;
    case BASS_ERROR_MEM:
        sampAddChatMessage("BASS: ������ ������");
        break;
    case BASS_ERROR_FILEOPEN:
        sampAddChatMessage("BASS: ���������� ������� ����");
        break;
    case BASS_ERROR_DRIVER:
        sampAddChatMessage("BASS: �� ������� ����� ���������/�������������� �������");
        break;
    case BASS_ERROR_BUFLOST:
        sampAddChatMessage("BASS: ����� ������ ��� �������");
        break;
    case BASS_ERROR_HANDLE:
        sampAddChatMessage("BASS: �������� ����������");
        break;
    case BASS_ERROR_FORMAT:
        sampAddChatMessage("BASS: ���������������� ������ ������");
        break;
    case BASS_ERROR_POSITION:
        sampAddChatMessage("BASS: �������� �������");
        break;
    case BASS_ERROR_INIT:
        sampAddChatMessage("BASS: BASS_Init �� ��� ������� ������");
        break;
    case BASS_ERROR_START:
        sampAddChatMessage("BASS: BASS_Start �� ��� ������� ������");
        break;
    case BASS_ERROR_SSL:
        sampAddChatMessage("BASS: SSL/HTTPS ��������� ����������");
        break;
    case BASS_ERROR_REINIT:
        sampAddChatMessage("BASS: ���������� ������� ��������� �������������");
        break;
    case BASS_ERROR_ALREADY:
        sampAddChatMessage("BASS: ��� ����������������/��������������/��� ������ ���");
        break;
    case BASS_ERROR_NOTAUDIO:
        sampAddChatMessage("BASS: ���� �� �������� �����");
        break;
    case BASS_ERROR_NOCHAN:
        sampAddChatMessage("BASS: ���������� �������� ��������� �����");
        break;
    case BASS_ERROR_ILLTYPE:
        sampAddChatMessage("BASS: ������ ������������ ���");
        break;
    case BASS_ERROR_ILLPARAM:
        sampAddChatMessage("BASS: ������ ������������ ��������");
        break;
    case BASS_ERROR_NO3D:
        sampAddChatMessage("BASS: ��� ��������� 3D");
        break;
    case BASS_ERROR_NOEAX:
        sampAddChatMessage("BASS: ��� ��������� EAX");
        break;
    case BASS_ERROR_DEVICE:
        sampAddChatMessage("BASS: ������������ ����� ����������");
        break;
    case BASS_ERROR_NOPLAY:
        sampAddChatMessage("BASS: �� ���������������");
        break;
    case BASS_ERROR_FREQ:
        sampAddChatMessage("BASS: ������������ ������� �������������");
        break;
    case BASS_ERROR_NOTFILE:
        sampAddChatMessage("BASS: ����� �� �������� �������� �������");
        break;
    case BASS_ERROR_NOHW:
        sampAddChatMessage("BASS: ��� ��������� ���������� �������");
        break;
    case BASS_ERROR_EMPTY:
        sampAddChatMessage("BASS: ���� �� �������� ������ � ������");
        break;
    case BASS_ERROR_NONET:
        sampAddChatMessage("BASS: ���������� ������� ���������� � ����������");
        break;
    case BASS_ERROR_CREATE:
        sampAddChatMessage("BASS: ���������� ������� ����");
        break;
    case BASS_ERROR_NOFX:
        sampAddChatMessage("BASS: ������� ����������");
        break;
    case BASS_ERROR_NOTAVAIL:
        sampAddChatMessage("BASS: ����������� ������/�������� ����������");
        break;
    case BASS_ERROR_DECODE:
        sampAddChatMessage("BASS: ����� ��������/�� �������� \"������������ �������\"");
        break;
    case BASS_ERROR_DX:
        sampAddChatMessage("BASS: ������������� ������ DirectX �����������");
        break;
    case BASS_ERROR_TIMEOUT:
        sampAddChatMessage("BASS: ����� ���������� �������");
        break;
    case BASS_ERROR_FILEFORM:
        sampAddChatMessage("BASS: ���������������� ������ �����");
        break;
    case BASS_ERROR_SPEAKER:
        sampAddChatMessage("BASS: ������� ����������");
        break;
    case BASS_ERROR_VERSION:
        sampAddChatMessage("BASS: �������� ������ BASS (������������ ������������)");
        break;
    case BASS_ERROR_CODEC:
        sampAddChatMessage("BASS: ����� ����������/��������������");
        break;
    case BASS_ERROR_ENDED:
        sampAddChatMessage("BASS: �����/���� ����������");
        break;
    case BASS_ERROR_BUSY:
        sampAddChatMessage("BASS: ���������� ������");
        break;
    case BASS_ERROR_UNSTREAMABLE:
        sampAddChatMessage("BASS: ���������� ������������� ����");
        break;
    case BASS_ERROR_PROTOCOL:
        sampAddChatMessage("BASS: ���������������� ��������");
        break;
    case BASS_ERROR_DENIED:
        sampAddChatMessage("BASS: ������ ��������");
        break;
    case BASS_ERROR_UNKNOWN:
        sampAddChatMessage("BASS: ����������� ������");
        break;
    default:
        sampAddChatMessage("BASS: ����������� ��� ������");
        break;
    }
}

void VoiceClient::cleanupAudio() {
    waveInStop(wi);

    for (int i = 0; i < 2; ++i) {
        waveInUnprepareHeader(wi, &headers[i], sizeof(headers[i]));
    }

    waveInClose(wi);
}

void VoiceClient::setReadyMic(UINT selectedMic) {
    openMicrophone(selectedMic);
    prepareAudioBuffers();
    waveInStart(wi);

    std::string nickname = sampapi::v037r3::RefNetGame()->GetPlayerPool()->m_localInfo.m_szName;
    UDPMessage message{ "null", "connect", true, nickname.data() };
    sampNickname = nickname.data();
    try {
        boost::asio::write(socket, boost::asio::buffer(message.toJSON()));

    }
    catch (const boost::system::system_error& e) {

    }
}

void VoiceClient::startTalkingThread() {
    if (!threadRunning) {
        threadRunning = true;
        talkingThread = std::thread(&VoiceClient::talkLoop, this);
    }
}

void VoiceClient::stopTalkingThread() {
    if (threadRunning) {
        threadRunning = false;
        if (talkingThread.joinable()) {
            talkingThread.join();
        }
    }
}

void VoiceClient::talkLoop() {
    while (threadRunning) {
        if (!voiceMoving) {
            continue;
        }
        Talk();
    }
}

void VoiceClient::talk() {
    if (!threadRunning) {
        startTalkingThread();
    }
}

void VoiceClient::Talk() {

    char buffer[48000 * 2 * 5 / 5];
    for (auto& h : headers) {
        if (h.dwFlags & WHDR_DONE) {
            try {
                boost::asio::write(socket, boost::asio::buffer(std::string(h.lpData, h.dwBufferLength) + ", Delano_Vinci"));
            }
            catch (const boost::system::system_error& e) {
            }
            h.dwFlags = 0;
            h.dwBytesRecorded = 0;
            if (waveInPrepareHeader(wi, &h, sizeof(h)) != MMSYSERR_NOERROR) {
                sampAddChatMessage("Failed to prepare audio header");
            }
            if (waveInAddBuffer(wi, &h, sizeof(h)) != MMSYSERR_NOERROR) {
                sampAddChatMessage("Failed to add audio buffer");
            }
            boost::system::error_code error;
            size_t length = socket.read_some(boost::asio::buffer(buffer), error);

            if (!error) {
                playSound(buffer, length);
            }
        }
    }

}

#endif // VOICE_H
