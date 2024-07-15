class default_global_light : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_global_light", "Node", false, false);
		
		//	TODO: add members and methods to default_global_light.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_global_light::Create();