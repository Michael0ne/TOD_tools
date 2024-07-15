class default_lightEmitters : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_lightEmitters", "Node", false, false);
		
		//	TODO: add members and methods to default_lightEmitters.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_lightEmitters::Create();