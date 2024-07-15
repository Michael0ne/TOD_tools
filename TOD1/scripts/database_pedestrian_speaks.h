class database_pedestrian_speaks : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_pedestrian_speaks", "Node", false, false);
		
		//	TODO: add members and methods to database_pedestrian_speaks.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_pedestrian_speaks::Create();