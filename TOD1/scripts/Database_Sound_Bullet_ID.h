class Database_Sound_Bullet_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Database_Sound_Bullet_ID", "Node", false, false);
		
		//	TODO: add members and methods to Database_Sound_Bullet_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Database_Sound_Bullet_ID::Create();