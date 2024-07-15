class Triger_Root_Mission : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Triger_Root_Mission", "Node", false, false);
		
		//	TODO: add members and methods to Triger_Root_Mission.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Triger_Root_Mission::Create();