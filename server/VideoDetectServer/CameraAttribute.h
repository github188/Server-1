#pragma once
#include <cstdint>
#include <string>
#include <vector>

typedef struct cameraattribute
{
	uint16_t port;
	std::string device_id;
	std::string camera_vender;
	std::string ip;
	std::vector<std::string> lane_number;
	bool operator ==(const cameraattribute& ca) const
	{
		return ((port == ca.port) && (device_id == ca.device_id)
			&& (camera_vender == ca.camera_vender) && (ip == ca.ip) && lane_number == ca.lane_number);
	}
}CameraAttribute;

struct CameraAttributeHash
{
	std::size_t operator()(const CameraAttribute& ca) const
	{
		auto h1 = std::hash<uint16_t>()(ca.port);
		auto h2 = std::hash<std::string>()(ca.device_id);
		auto h3 = std::hash<std::string>()(ca.camera_vender);
		auto h4 = std::hash<std::string>()(ca.ip);
		return (h1 << 1) ^ (h2 >> 1) ^ (h3 >> 2) ^ (h4 << 2); // or use boost::hash_combine
	}
};