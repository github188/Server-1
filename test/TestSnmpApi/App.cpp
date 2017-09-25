#include <memory>
#include <thread>
#include "../../server/MoniterServer/snmp.h"
int main()
{
	auto snmp_ptr = std::make_shared<Platform::Snmp>(Platform::Snmp());
	snmp_ptr->init();

	void* session = nullptr;
	snmp_ptr->makeSession([](HSNMP_SESSION hSession,HWND hWnd,UINT wMsg,WPARAM wParam,LPARAM lParam,LPVOID lpClientData)->SNMPAPI_STATUS
	{
		auto ptr = static_cast<Platform::Snmp*>(lpClientData);
		void* recvPdu = nullptr;
		void* SrcEntity = nullptr;
		void* dstEntity = nullptr;
		void* context = nullptr;
		ptr->recvMessage(hSession, SrcEntity, dstEntity, context, recvPdu);

		void* vblist = nullptr;
		smiINT request_id = -3938750;
		ptr->getPduData(recvPdu, vblist);

		int vbl_count = 0;
		ptr->getCountVbl(vblist, vbl_count);
		for (int i = 1; i <= vbl_count; ++i)
		{
			smiOID name;
			smiVALUE value;
			ptr->getVb(vblist, &name, i, &value);
			char s[1000] = { 0 };
			SnmpOidToStr(&name, 1000, s);
			auto Ret = SnmpFreeDescriptor(SNMP_SYNTAX_OID, reinterpret_cast<smiLPOPAQUE>(&name));
			if (Ret == SNMPAPI_FAILURE)
			{
				printf("SnmpFreeDescriptor failed with error_code: %d\n", SnmpGetLastError(nullptr));
			}
			switch (value.syntax)
			{
			case SNMP_SYNTAX_INT32:
			{
				auto x = value.value.sNumber;
				break;
			}
			case SNMP_SYNTAX_UINT32:
			case SNMP_SYNTAX_CNTR32:
			case SNMP_SYNTAX_GAUGE32:
			case SNMP_SYNTAX_TIMETICKS:
			{
				auto x = value.value.uNumber;
				break;
			}
			case SNMP_SYNTAX_CNTR64:
			{
				uint64_t number = (static_cast<uint64_t>(value.value.hNumber.hipart)) << 32 | value.value.hNumber.lopart;
				break;
			}
			case SNMP_SYNTAX_OCTETS:
			case SNMP_SYNTAX_BITS:
			case SNMP_SYNTAX_OPAQUE:
			case SNMP_SYNTAX_IPADDR:
			case SNMP_SYNTAX_NSAPADDR:
			{
				char desc[1024 * 2] = { 0 };
				memcpy(desc, value.value.string.ptr, value.value.string.len);
				std::string str = desc;
				//std::string str1 = reinterpret_cast<char*>(value.value.string.ptr);
				SnmpFreeDescriptor(value.syntax, &(value.value.string));
				break;
			}
			case SNMP_SYNTAX_OID:
			{
				//SnmpOidToStr(&value.value.oid, 1000, s);
				//std::cout << s << std::endl;
				//SnmpFreeDescriptor(value.syntax, reinterpret_cast<smiLPOPAQUE>(&(value.value.oid)));
				break;
			}
			case SNMP_SYNTAX_NULL:
			case SNMP_SYNTAX_NOSUCHOBJECT:
			case SNMP_SYNTAX_NOSUCHINSTANCE:
			case SNMP_SYNTAX_ENDOFMIBVIEW:
			{
				printf("oid is invalid");
				break;
			}
			default:
			{
				printf("out of win define range");
				break;
			}
			}
		}
		
		ptr->releaseVbl(vblist);
		//ptr->releaseContext(context);
		//ptr->releaseEntity(dstEntity);
		//ptr->releaseEntity(SrcEntity);
		ptr->releasePdu(recvPdu);
		return 0;
	}, snmp_ptr.get(), session);

	void* manager_entity = nullptr;
	snmp_ptr->makeManagerEntity(session, manager_entity);

	void* agent_entity = nullptr;
	snmp_ptr->makeAgentEntity(session, "192.168.1.199", agent_entity);

	void* vbl = nullptr;
	snmp_ptr->makeVbl(session, vbl);

	void* context = nullptr;
	snmp_ptr->makeContext("public", session, context);

	void* pdu = nullptr;
	snmp_ptr->makePdu(session, nullptr, pdu);

	while (true)
	{
		snmp_ptr->setVbl(vbl, 0, "1.3.6.1.2.1.1.1.0");
		snmp_ptr->setVbl(vbl, 0, "1.3.6.1.2.1.25.3.3.1.2.4");
		snmp_ptr->setVbl(vbl, 0, "1.3.6.1.2.1.25.3.3.1.2.5");
		snmp_ptr->setVbl(vbl, 0, "1.3.6.1.2.1.25.3.3.1.2.6");
		snmp_ptr->setVbl(vbl, 0, "1.3.6.1.2.1.25.3.3.1.2.8");
		snmp_ptr->setPdu(pdu, &vbl);
		int vbl_count = 0;
		snmp_ptr->getCountVbl(vbl, vbl_count);
		//printf("%d\n", vbl_count);
		snmp_ptr->sendRequestMessage(session, manager_entity, agent_entity, context, pdu);
		snmp_ptr->deleteVb(vbl, 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
	return 0;
}