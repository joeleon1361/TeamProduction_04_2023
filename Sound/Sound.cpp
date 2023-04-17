﻿#include "Sound.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

Sound* Sound::GetInstance()
{
	static Sound instance;

	return &instance;
}

void Sound::Initialize(const std::string& directoryPath)
{
	directoryPath_ = directoryPath;

	HRESULT result;
	IXAudio2MasteringVoice* masterVoice;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
	}

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
	}
}

void Sound::Finalize()
{
	//XAudio2解放
	xAudio2.Reset();

	//音声データ解放
	std::map<std::string, SoundData>::iterator it = soundDatas.begin();

	for (; it != soundDatas.end(); ++it)
	{
		UnLoad(&it->second);
	}
}

void Sound::LoadWav(const std::string& filename)
{
	if (soundDatas.find(filename) != soundDatas.end())
	{
		//音声の重複読み込みを無視
		return;
	}

	//ディレクトリパスとファイル名を連結してフルパスを得る
	std::string fullpath = directoryPath_ + filename;

	// ファイルストリーム
	std::ifstream file;
	// Waveファイルを開く
	file.open(fullpath, std::ios_base::binary);
	// ファイルオープン失敗をチェック
	assert(file.is_open());

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format = {};

	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	// LISTチャンクを検出した場合
	if (strncmp(data.id, "LIST", 4) == 0)
	{
		//読み取り位置をLISTチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0)
	{
		assert(0);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();

	//returnする為の音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//サウンドデータを連想配列に格納
	soundDatas.insert(std::make_pair(filename, soundData));
}

void Sound::UnLoad(SoundData* soundData)
{
	//バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Sound::PlayWav(const std::string& filename, const float Volume, bool Loop)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas.find(filename);

	//未読み込みの検出
	assert(it != soundDatas.end());

	//サウンドデータの参照を取得
	SoundData& soundData = it->second;

	//波形フォーマットを元にSourceVoiceの生成
	//IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定 Setting of waveform data to be played
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	//buf.pContext = pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (Loop)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	soundData.pSourceVoice->SetVolume(Volume);

	// 波形データの再生 Playback of waveform data
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundData.pSourceVoice->Start();
}

void Sound::StopWav(const std::string& filename)
{
	std::map<std::string, SoundData>::iterator it = soundDatas.find(filename);

	//未読み込みの検出
	assert(it != soundDatas.end());

	//サウンドデータの参照を取得
	SoundData& soundData = it->second;


	XAUDIO2_VOICE_STATE State;
	soundData.pSourceVoice->GetState(&State);
	if (State.BuffersQueued == 0)
	{
		return;
	}

	soundData.pSourceVoice->Stop(0);
	soundData.pSourceVoice->FlushSourceBuffers();
	soundData.pSourceVoice->SubmitSourceBuffer(&buf);
}
