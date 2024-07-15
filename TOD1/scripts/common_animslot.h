class common_animslot : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("common_animslot", "Node", false, false);
		
		//	TODO: add members and methods to common_animslot.
		
		inst->CalculateSize();
		
		return inst;
	};
};

common_animslot::Create();