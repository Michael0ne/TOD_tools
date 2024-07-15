class database_enemy_speaks : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_enemy_speaks", "Node", false, false);
		
		//	TODO: add members and methods to database_enemy_speaks.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_enemy_speaks::Create();