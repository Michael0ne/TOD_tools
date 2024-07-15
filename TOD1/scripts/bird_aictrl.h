class bird_aictrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("bird_aictrl", "Node", false, false);
		
		//	TODO: add members and methods to bird_aictrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

bird_aictrl::Create();