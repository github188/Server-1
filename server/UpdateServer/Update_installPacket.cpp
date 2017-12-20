#include "Update_installPacket.h"
#include <sstream>
#include <Windows.h>

void update::server::UpdateInstallPacket::update(const std::string& install_packet_name, const std::string& last_time_components)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	std::string cmdLine = "./update/";
	cmdLine.append(install_packet_name).append("/COMPONENTS=")
		.append("\"").append(last_time_components).append("\"").append(" /verysilent");
	auto ret = CreateProcess(NULL, const_cast<char*>(cmdLine.c_str()),
		NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	if (ret != 0)
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		exit(0);//for update
	}
	else
	{
		std::ostringstream message;
		message << "CreateProcess failed with error_code:" << GetLastError();
		throw std::runtime_error(message.str());
	}
}
