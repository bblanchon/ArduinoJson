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
			// BUG described here:
			// http://forum.arduino.cc/index.php?topic=172578.msg1608219#msg1608219
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

		TEST_METHOD(Repeats)
		{
			Assert::AreEqual(1L, root.getLong("repeats"));
		}

		TEST_METHOD(Wait)
		{
			Assert::AreEqual(0L, root.getLong("wait"));
		}

		TEST_METHOD(Measurements)
		{
			Assert::AreEqual(3L, root.getLong("measurements"));
		}

		TEST_METHOD(Meas2_Light)
		{
			Assert::AreEqual(15L, root.getLong("meas2_light"));
		}

		TEST_METHOD(Meas1_Baseline)
		{
			Assert::AreEqual(0L, root.getLong("meas1_baseline"));
		}

		TEST_METHOD(Act_Light)
		{
			Assert::AreEqual(20L, root.getLong("act_light"));
		}

		TEST_METHOD(Pulsesize)
		{
			Assert::AreEqual(25L, root.getLong("pulsesize"));
		}

		TEST_METHOD(Pulsedistance)
		{
			Assert::AreEqual(10000L, root.getLong("pulsedistance"));
		}

		TEST_METHOD(Actintensity1)
		{
			Assert::AreEqual(50L, root.getLong("actintensity1"));
		}
				
		TEST_METHOD(Actintensity2)
		{
			Assert::AreEqual(255L, root.getLong("actintensity2"));
		}
		
		TEST_METHOD(Measintensity)
		{
			Assert::AreEqual(255L, root.getLong("measintensity"));
		}
		
		TEST_METHOD(Calintensity)
		{
			Assert::AreEqual(255L, root.getLong("calintensity"));
		}

		TEST_METHOD(Pulses)
		{
			// "pulses":[50,50,50]

			JsonArray array = root.getArray("pulses");
			Assert::IsTrue(array.success());

			Assert::AreEqual(3, array.getLength());

			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual(50L, array.getLong(i));
			}
		}

		TEST_METHOD(Act)
		{
			// "act":[2,1,2,2]

			JsonArray array = root.getArray("act");
			Assert::IsTrue(array.success());

			Assert::AreEqual(4, array.getLength());
			Assert::AreEqual(2L, array.getLong(0));
			Assert::AreEqual(1L, array.getLong(1));
			Assert::AreEqual(2L, array.getLong(2));
			Assert::AreEqual(2L, array.getLong(3));			
		}

		TEST_METHOD(Detectors)
		{
			// "detectors":[[34,34,34,34],[34,34,34,34],[34,34,34,34],[34,34,34,34]]

			JsonArray array = root.getArray("detectors");
			Assert::IsTrue(array.success());
			Assert::AreEqual(4, array.getLength());

			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(4, array.getArray(i).getLength());

				for (int j = 0; j < 4; j++)
					Assert::AreEqual(34L, array.getArray(i).getLong(j));
			}
		}

		TEST_METHOD(Alta)
		{
			// alta:[2,2,2,2]

			JsonArray array = root.getArray("alta");
			Assert::IsTrue(array.success());

			Assert::AreEqual(4, array.getLength());

			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(2L, array.getLong(i));
			}
		}

		TEST_METHOD(Altb)
		{
			// altb:[2,2,2,2]

			JsonArray array = root.getArray("altb");
			Assert::IsTrue(array.success());

			Assert::AreEqual(4, array.getLength());

			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(2L, array.getLong(i));
			}
		}

		TEST_METHOD(Measlights)
		{
			// "measlights":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,15,15]]

			JsonArray array = root.getArray("measlights");
			Assert::IsTrue(array.success());
			Assert::AreEqual(4, array.getLength());

			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(4, array.getArray(i).getLength());

				for (int j = 0; j < 4; j++)
					Assert::AreEqual(15L, array.getArray(i).getLong(j));
			}
		}

		TEST_METHOD(Measlights2)
		{
			// "measlights2":[[15,15,15,15],[15,15,15,15],[15,15,15,15],[15,15,15,15]]

			JsonArray array = root.getArray("measlights2");
			Assert::IsTrue(array.success());
			Assert::AreEqual(4, array.getLength());

			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(4, array.getArray(i).getLength());

				for (int j = 0; j < 4; j++)
					Assert::AreEqual(15L, array.getArray(i).getLong(j));
			}
		}

		TEST_METHOD(Altc)
		{
			// altc:[2,2,2,2]

			JsonArray array = root.getArray("altc");
			Assert::IsTrue(array.success());

			Assert::AreEqual(4, array.getLength());

			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(2L, array.getLong(i));
			}
		}

		TEST_METHOD(Altd)
		{
			// altd:[2,2,2,2]

			JsonArray array = root.getArray("altd");
			Assert::IsTrue(array.success());

			Assert::AreEqual(4, array.getLength());

			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(2L, array.getLong(i));
			}
		}
	};
}