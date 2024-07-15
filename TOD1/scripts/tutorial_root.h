class tutorial_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("tutorial_root", "Node", false, false);
		
		//	TODO: add members and methods to tutorial_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

tutorial_root::Create();