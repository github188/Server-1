#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>
#include "ivhs_sdk.h"
#pragma comment(lib,"ws2_32.lib")
namespace ITS
{
	typedef struct kedacameraipport
	{
		std::string ip;
		uint16_t port;

		kedacameraipport(const std::string& Ip, uint16_t Port) :ip(Ip), port(Port) {}

		bool operator ==(const kedacameraipport cip) const
		{
			return (ip == cip.ip && port == cip.port);
		}
	} KeDaCameraIpPort;

	struct KeDaCameraIpPortHash
	{
		std::size_t operator()(const KeDaCameraIpPort& k) const
		{
			auto h1 = std::hash<std::string>()(k.ip);
			auto h2 = std::hash<uint16_t>()(k.port);
			return (h2 << 1) ^ (h1 >> 1); // or use boost::hash_combine
		}
	};

	const int MAX_NUM_CONNECT = 2;

	class KeDa
	{
	public:
		typedef std::function<void(TItsMessage *)> CameraMessageCallback;
		static CameraMessageCallback cb_; //declare
		std::mutex mtx_;
		std::unordered_map<KeDaCameraIpPort, bool, KeDaCameraIpPortHash> keda_camera_need_connect_;
		std::unordered_map<KeDaCameraIpPort, KOSA_HANDLE, KeDaCameraIpPortHash> keda_camera_dwHandle_;

	public:
		void initKeda(int MaxNumConnect);
		int connectAndLogin(const std::string& ip, uint16_t port,
			const std::string& user_name, const std::string& password, KOSA_HANDLE& dwHandle);
		int disconnect(KOSA_HANDLE dwHandle);
		int stopKeda();
	private:
		static void cameraMessageCallback(TItsMessage *ptMsg) { cb_(ptMsg); }
	};
}