class Pathtype : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Pathtype", "Node", false, false);
		
		//	TODO: add members and methods to Pathtype.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Pathtype::Create();