#ifndef SOCKET_WRAPPER_H
#define SOCKET_WRAPPER_H

namespace stream
{
	class SocketWrapper
	{
	public:
		SocketWrapper();
		~SocketWrapper();

		int test() { return 6; }
	};
}

#endif