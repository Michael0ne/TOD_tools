class Trigger_Root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Trigger_Root", "Node", false, false);
		
		//	TODO: add members and methods to Trigger_Root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Trigger_Root::Create();