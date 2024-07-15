class Trigger_direct : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_direct", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_direct.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_direct::Create();