class common : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("common", "Node", false, false);
		
		//	TODO: add members and methods to common.
		
		inst->CalculateSize();
		
		return inst;
	};
};

common::Create();