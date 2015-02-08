#include "stdafx.h"

#include <zmq.h>
#include <zmq_utils.h>

#include <assert.h>
#include <string.h>
#include "CppUnitTest.h"
#include "CppUnitTestLogger.h"
#include "CppUnitTestAssert.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace stream_api_test
{		
	TEST_CLASS(zeromq_test)
	{
	public:
		
		TEST_METHOD(basic_test)
		{
			Logger::WriteMessage("here we go");
			
			// Socket to talk to clients
			void *context = zmq_ctx_new();
			void *responder = zmq_socket(context, ZMQ_REP);
			int rc = zmq_bind(responder, "tcp://*:5555");
			assert(rc == 0);

			void *requester = zmq_socket(context, ZMQ_REQ);
			rc = zmq_connect(requester, "tcp://localhost:5555");
			assert(rc == 0);
			
			char buffer[10];
			zmq_recv(responder, buffer, 10, 0);
			printf("Received Hello\n");
			zmq_send(responder, "World", 5, 0);


			int request_nbr;

			printf("Sending Hello %d...\n", request_nbr);
			zmq_send(requester, "Hello", 5, 0);
			zmq_recv(requester, buffer, 10, 0);
			printf("Received World %d\n", request_nbr);
			
			zmq_close(requester);
			zmq_ctx_destroy(context);			
		}
	};
}