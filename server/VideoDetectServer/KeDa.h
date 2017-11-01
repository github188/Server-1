#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>
#include "ivhs_sdk.h"
#include "CameraAttribute.h"
#pragma comment(lib,"ws2_32.lib")
namespace ITS
{
	class KeDa
	{
	public:
		typedef std::function<void(TItsMessage *)> CameraMessageCallback;
		static CameraMessageCallback cb_; //declare
		std::mutex mtx_;
		std::unordered_map<CameraAttribute, bool, CameraAttributeHash> keda_camera_need_connect_;
		std::unordered_map<CameraAttribute, KOSA_HANDLE, CameraAttributeHash> keda_camera_dwHandle_;

	public:
		void initKeda(int MaxNumConnect);
		int connectAndPostLoginMessage(const std::string& ip, uint16_t port,
			const std::string& user_name, const std::string& password, KOSA_HANDLE& dwHandle);
		int disconnect(KOSA_HANDLE dwHandle);
		int stopKeda();
	private:
		static void cameraMessageCallback(TItsMessage *ptMsg) { cb_(ptMsg); }
	};
}