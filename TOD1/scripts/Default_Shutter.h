class Default_Shutter : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Default_Shutter", "Node", false, false);
		
		//	TODO: add members and methods to Default_Shutter.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Default_Shutter::Create();