#include "snmp.h"
#include <sstream>
#include <cassert>
#include "LogMacros.h"

#pragma comment(lib,"Wsnmp32.lib")

void Platform::Snmp::init()
{
	smiUINT32 MajorVersion = 0;
	smiUINT32 MinorVersion = 0;
	smiUINT32 level = 0;
	smiUINT32 TranslateMode = 0;
	smiUINT32 RetransmitMode = 0;
	auto ret = SnmpStartup(&MajorVersion, &MinorVersion, &level, &TranslateMode, &RetransmitMode);
	if (ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpStartup failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}

	ret = SnmpSetRetransmitMode(SNMPAPI_ON);
	if (ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpSetRetransmitMode failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::recvMessage(void * h_session, void *& SrcEntity, void *& DstEntity, void *& recvContext, void *& recv_pdu)
{
	auto ret = SnmpRecvMsg(h_session, &SrcEntity, &DstEntity, &recvContext, &recv_pdu);
	if (ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpRecvMsg failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::getPduData(void * recv_pdu, void *& vb_list)
{
	smiINT pdu_type = 0;
	smiINT ErrStatus = 0;
	smiINT ErrIndex = 0;
	smiINT request_id = 0;//not important for me
	auto ret = SnmpGetPduData(recv_pdu, &pdu_type, &request_id, &ErrStatus, &ErrIndex, &vb_list);
	if (ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpGetPduData failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}
	assert(ErrStatus == 0 && ErrIndex == 0);
	if (ErrStatus != 0 || ErrIndex != 0)
	{
		std::ostringstream message;
		message << "Get unexpected PduData," << "ErrStatus:" << ErrStatus << ",ErrIndex" << ErrIndex;
		throw std::logic_error(message.str());
	}
}

void Platform::Snmp::getCountVbl(void * vb_list, int & vbl_count)
{
	vbl_count = SnmpCountVbl(vb_list);
	if (vbl_count == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpCountVbl failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::translateEntityToIp(void * entity, std::string& str)
{
	char s[20] = { 0 }; //ip
	auto ret = SnmpEntityToStr(entity, 20, s);
	if (ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpEntityToStrl failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}
	str = s;
}

void Platform::Snmp::translateOidToStr(smiLPCOID srcOID, std::string & str)
{
	char s[MAXOBJIDSIZE] = { 0 };
	auto Ret = SnmpOidToStr(srcOID, MAXOBJIDSIZE, s);
	if (Ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpOidToStr failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}

	Ret = SnmpFreeDescriptor(SNMP_SYNTAX_OID, (smiLPOPAQUE)(srcOID));
	if (Ret == SNMPAPI_FAILURE)
	{
		MONITERSERVER_WARN("SnmpFreeDescriptor failed with error_code: %d\n", SnmpGetLastError(nullptr));
	}
	str = s;
}

void Platform::Snmp::getVb(void * vbl, smiLPOID name,smiUINT32 index, smiLPVALUE value)
{
	auto ret = SnmpGetVb(vbl, index, name, value);
	if (ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpGetVb failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::makeSession(const SNMPAPI_CALLBACK & cb, void * extra_data, void *& session)
{
	session = SnmpCreateSession(nullptr, 0, cb, extra_data);
	if (session == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpCreateSession failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::makeContext(const std::string & community, void * session, void *& context)
{
	smiOCTETS string;
	string.len = strlen(community.c_str());
	string.ptr = (smiLPBYTE)community.c_str();

	context = SnmpStrToContext(session, &string);
	if (context == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpStrToContext failed with error_code:" << SnmpGetLastError(session)
			<< ",the community is " << community;
		throw std::runtime_error(message.str());
	}
	//auto Ret = SnmpFreeDescriptor(SNMP_SYNTAX_OCTETS, reinterpret_cast<smiLPOPAQUE>(&string));
	//if (Ret == SNMPAPI_FAILURE)
	//{
	//	printf("SnmpFreeDescriptor failed with error_code: %d\n", SnmpGetLastError(nullptr));
	//}
}

void Platform::Snmp::makeVbl(void * session, void *& vbl)
{
	vbl = SnmpCreateVbl(session, nullptr, nullptr);
	if (vbl == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpCreateVbl failed with error_code:" << SnmpGetLastError(session);
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::setVbl(void * vbl, smiUINT32 index, const std::string & oid_str)
{
	smiOID oid;
	auto Ret = SnmpStrToOid(oid_str.c_str(), &oid);
	if (Ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpStrToOid failed with error_code:" << SnmpGetLastError(nullptr)
			<< ", the oid_str is " << oid_str;
		throw std::runtime_error(message.str());
	}
	Ret = SnmpSetVb(vbl, index, &oid, nullptr);
	if (Ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpSetVb failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}
	Ret = SnmpFreeDescriptor(SNMP_SYNTAX_OID, reinterpret_cast<smiLPOPAQUE>(&oid));
	if (Ret == SNMPAPI_FAILURE)
	{
		MONITERSERVER_WARN("SnmpFreeDescriptor failed with error_code: %d\n", SnmpGetLastError(nullptr));
	}
}

void Platform::Snmp::deleteVb(void * vbl, smiUINT32 index)
{
	auto ret = SnmpDeleteVb(vbl, index);
	if (ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << " SnmpDeleteVb failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::setPdu(void* PDU, const HSNMP_VBL*varbindlist)
{
	smiINT pdu_type = SNMP_PDU_GET;
	smiINT request_id = 1; //not important for me
	auto ret = SnmpSetPduData(PDU, &pdu_type, &request_id, 0, 0, varbindlist);
	if (ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << " SnmpSetPduData failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::makeManagerEntity(void * session, void *& manager_entity)
{
	char* SrcAddr = "127.0.0.1";
	manager_entity = SnmpStrToEntity(session, SrcAddr);//manager
	if (manager_entity == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpStrToEntity failed with error_code:" << SnmpGetLastError(session);
		throw std::runtime_error(message.str());
	}

	auto Ret = SnmpSetTimeout(manager_entity, 0);//the implementation selects a value
	if (Ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpSetTimeout failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}

	Ret = SnmpSetRetry(manager_entity, 0); //the implementation selects a value
	if (Ret == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpSetRetry failed with error_code:" << SnmpGetLastError(nullptr);
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::makeAgentEntity(void * session, const std::string destination_address, void *& agent_entity)
{
	agent_entity = SnmpStrToEntity(session, destination_address.c_str());//agent
	if (agent_entity == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpStrToEntity failed with error_code:" << SnmpGetLastError(session)
			<< ",agent ip is " << destination_address;
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::makePdu(void * session, void * vbl, void *& pdu)
{
	int request_id = 1;//not important for me
	pdu = SnmpCreatePdu(session, /*SNMP_PDU_GETNEXT*/SNMP_PDU_GET, request_id, 0, 0, vbl);
	if (pdu == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpCreatePdu failed with error_code:" << SnmpGetLastError(session);
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::sendRequestMessage(void * session, void * manager_entity, void * agent_entity, void * context, void * pdu)
{
	auto SnmpSendMsgRet = SnmpSendMsg(session, manager_entity, agent_entity, context, pdu);
	if (SnmpSendMsgRet == SNMPAPI_FAILURE)
	{
		std::ostringstream message;
		message << "SnmpSendMsg failed with error_code:" << SnmpGetLastError(session);
		throw std::runtime_error(message.str());
	}
}

void Platform::Snmp::releaseEntity(void * entity) /*noexcept*/
{
	auto ret = SnmpFreeEntity(entity);
	if (ret == SNMPAPI_FAILURE)
	{
		auto ErrorRet = SnmpGetLastError(nullptr);
		MONITERSERVER_WARN("SnmpFreeEntity free entity(%p) with error code: %d\n",entity, ErrorRet);
	}
}

void Platform::Snmp::releaseContext(void * context)/* noexcept*/
{
	auto ret = SnmpFreeContext(context);
	if (ret == SNMPAPI_FAILURE)
	{
		auto ErrorRet = SnmpGetLastError(nullptr);
		MONITERSERVER_WARN("SnmpFreeContext free context(%p) with error code: %d\n", context, ErrorRet);
	}
}

void Platform::Snmp::releaseVbl(void * vbl)/* noexcept*/
{
	auto ret = SnmpFreeVbl(vbl);
	if (ret == SNMPAPI_FAILURE)
	{
		auto ErrorRet = SnmpGetLastError(nullptr);
		MONITERSERVER_WARN("SnmpFreeVbl free vbl(%p) with error code: %d\n", vbl, ErrorRet);
	}
}

void Platform::Snmp::releasePdu(void * pdu) /*noexcept*/
{
	auto ret = SnmpFreePdu(pdu);
	if (ret == SNMPAPI_FAILURE)
	{
		auto ErrorRet = SnmpGetLastError(nullptr);
		MONITERSERVER_WARN("SnmpFreePdu free pdu(%p) with error code: %d\n",pdu, ErrorRet);
	}
}

void Platform::Snmp::releaseSession(void * session)/* noexcept*/
{
	auto ret = SnmpClose(session);
	if (ret == SNMPAPI_FAILURE)
	{
		auto ErrorRet = SnmpGetLastError(nullptr);
		MONITERSERVER_WARN("SnmpClose free session(%p) with error code: %d\n", session, ErrorRet);
	}
}
