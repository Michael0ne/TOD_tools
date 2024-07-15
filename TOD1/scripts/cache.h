class cache : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("cache", "Node", false, false);
		
		//	TODO: add members and methods to cache.
		
		inst->CalculateSize();
		
		return inst;
	};
};

cache::Create();