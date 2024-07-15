class bird_movectrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("bird_movectrl", "Node", false, false);
		
		//	TODO: add members and methods to bird_movectrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

bird_movectrl::Create();