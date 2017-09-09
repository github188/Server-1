#pragma once
#ifndef SERVER_MONITORSERVER_SNMP_H
#define SERVER_MONITORSERVER_SNMP_H

#include <string>
#include<Winsnmp.h>
namespace Platform
{
	class Snmp sealed //final
	{
	public:
		explicit Snmp() {}; 
		~Snmp() {} /*noexcept*/;

		void init();
		void recvMessage(void* h_session, void*& SrcEntity,void*& DstEntity,void*&recvContext, void*& recv_pdu);
		void getPduData(void* recv_pdu, void*& vb_list);
		void getCountVbl(void* vb_list, int& vbl_count);
		void translateEntityToIp(void* entity, std::string& str);
		void translateOidToStr(smiLPCOID srcOID, std::string& str);
		void getVb(void* vbl, smiLPOID name, smiUINT32 index, smiLPVALUE value);
	
		void makeSession(const SNMPAPI_CALLBACK& cb, void* extra_data, void*& session);
		void makeContext(const std::string& community, void* session, void*& context);
		void makeVbl(void* session, void*& vbl);
		void setVbl(void* vbl, smiUINT32 index,const std::string& oid_str);
		void deleteVb(void* vbl, smiUINT32 index);
		void setPdu(void* PDU,const HSNMP_VBL*varbindlist);
		void makeManagerEntity(void* session, void*& manager_entity);
		void makeAgentEntity(void* session, const std::string destination_address, void*& agent_entity);
		void makePdu(void* session, void* vbl, void*& pdu);
		void sendRequestMessage(void* session, void* manager_entity, void* agent_entity, void*context, void* pdu) /*noexcept*/;

		void releaseEntity(void* entity) /*noexcept*/;
		void releaseContext(void* context) /*noexcept*/;
		void releaseVbl(void* vbl) /*noexcept*/;
		void releasePdu(void* pdu)/* noexcept*/;
		void releaseSession(void* session) /*noexcept*/;
	};
}
#endif
