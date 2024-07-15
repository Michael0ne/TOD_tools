class default_Lever : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_Lever", "Node", false, false);
		
		//	TODO: add members and methods to default_Lever.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_Lever::Create();