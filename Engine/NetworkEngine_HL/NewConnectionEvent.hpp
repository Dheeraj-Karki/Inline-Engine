#pragma once

#include <NetworkEngine_LL/TcpClient.hpp>

namespace inl::net::events
{
	using namespace sockets;

	class NewConnectionEvent
	{
	public:
		inline NewConnectionEvent(uint32_t id, void *data)
			: m_id(id)
			, m_data(data)
		{
		}

	private:
		uint32_t m_id;
		void *m_data;
	};
}