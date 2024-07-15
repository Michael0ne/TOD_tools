class default_prop : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_prop", "Node", false, false);
		
		//	TODO: add members and methods to default_prop.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_prop::Create();