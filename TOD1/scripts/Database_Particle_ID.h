class Database_Particle_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Database_Particle_ID", "Node", false, false);
		
		//	TODO: add members and methods to Database_Particle_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Database_Particle_ID::Create();