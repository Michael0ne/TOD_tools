class database_trafficPlaceholderResources : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_trafficPlaceholderResources", "Node", false, false);
		
		//	TODO: add members and methods to database_trafficPlaceholderResources.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_trafficPlaceholderResources::Create();