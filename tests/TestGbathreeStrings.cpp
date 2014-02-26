#include "CppUnitTest.h"
#include "JsonParser.h"
#include <string>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ArduinoJsonParserTests
{
	TEST_CLASS(TestGbathreeSample1)
	{
		char json[1024];
		JsonParser<200> parser;
		JsonHashTable root;
		

	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			strcpy(json, "{ \"protocol_name\":\"fluorescence\",\"repeats\":1,\"wait\":0,\"averages\":1,\"measurements\":3,\"meas2_light\":15,\"meas1_baseline\":0,\"act_light\":20,\"pulsesize\":25,\"pulsedistance\":10000,\"actintensity1\":50,\"actintensity2\":255,\"measintensity\":255,\"calintensity\":255,\"pulses\":[50,50,50],\"act\":[2,1,2,2],\"red\":[2,2,2,2],\"detectors\":[[34,34,34,34],[34,34,34,34],[34,34,34,34],[34,34,34,34]],\"alta\":[2,2,2,2],\"altb\":[2,2,2,2],\"measlights\":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,15,15]],\"measlights2\":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,15,15]],\"altc\":[2,2,2,2],\"altd\":[2,2,2,2]}");
			root = parser.parseHashTable(json);
		}

		TEST_METHOD(Root)
		{
			Assert::IsTrue(root.success());
		}

		TEST_METHOD(ProtocolName)
		{
			string protocol_name = root.getString("protocol_name");
			Assert::AreEqual(string("fluorescence"), protocol_name);
		}

		TEST_METHOD(Measlights)
		{
			// measlights:[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,15,15]]

			JsonArray measlights = root.getArray("altd");
			Assert::IsTrue(measlights.success());
			Assert::AreEqual(4, measlights.getLength());

			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(4, measlights.getArray(i).getLength());

				for (int j = 0; j < 4; j++)
					Assert::AreEqual(15L, measlights.getArray(i).getLong(j));
			}
		}

		TEST_METHOD(Altd)
		{
			// altd:[2,2,2,2]

			JsonArray altd = root.getArray("altd");
			Assert::IsTrue(altd.success());

			Assert::AreEqual(4, altd.getLength());

			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(2L, altd.getLong(i));
			}
		}
	};
}