class test_fade_forwarder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("test_fade_forwarder", "Node", false, false);
		
		//	TODO: add members and methods to test_fade_forwarder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

test_fade_forwarder::Create();