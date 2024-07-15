class trafficsystem_admin : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("trafficsystem_admin", "Node", false, false);
		
		//	TODO: add members and methods to trafficsystem_admin.
		
		inst->CalculateSize();
		
		return inst;
	};
};

trafficsystem_admin::Create();