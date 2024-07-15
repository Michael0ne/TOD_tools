class bird_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("bird_root", "Node", false, false);
		
		//	TODO: add members and methods to bird_root.
		
		inst->CalculateSize();
		
		return inst;
	};
};

bird_root::Create();