class prop_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("prop_root", "Node", false, false);
		
		//	TODO: add members and methods to prop_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

prop_root::Create();