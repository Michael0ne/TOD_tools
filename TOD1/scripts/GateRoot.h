class GateRoot : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("GateRoot", "Node", false, false);
		
		//	TODO: add members and methods to GateRoot.
		
		inst->CalculateSize();
		
		return inst;
	};
};

GateRoot::Create();