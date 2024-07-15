class database_master_bullet_drop : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_master_bullet_drop", "Node", false, false);
		
		//	TODO: add members and methods to database_master_bullet_drop.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_master_bullet_drop::Create();