class Conveyor_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Conveyor_root", "Node", false, false);
		
		//	TODO: add members and methods to Conveyor_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Conveyor_root::Create();