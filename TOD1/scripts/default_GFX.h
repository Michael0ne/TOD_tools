class default_GFX : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_GFX", "Node", false, false);
		
		//	TODO: add members and methods to default_GFX.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_GFX::Create();