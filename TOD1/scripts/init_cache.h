class init_cache : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("init_cache", "Node", false, false);
		
		//	TODO: add members and methods to init_cache.
		
		inst->CalculateSize();
		
		return inst;
	};
};

init_cache::Create();