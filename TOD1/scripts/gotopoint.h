class gotopoint : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("gotopoint", "Node", false, false);
		
		//	TODO: add members and methods to gotopoint.
		
		inst->CalculateSize();
		
		return inst;
	};
};

gotopoint::Create();