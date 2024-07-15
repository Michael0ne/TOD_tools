class DoorRoot : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("DoorRoot", "Node", false, false);
		
		//	TODO: add members and methods to DoorRoot.
		
		inst->CalculateSize();
		
		return inst;
	};
};

DoorRoot::Create();