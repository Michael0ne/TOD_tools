class database_LoadSprite_ID : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_LoadSprite_ID", "Node", false, false);
		
		//	TODO: add members and methods to database_LoadSprite_ID.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_LoadSprite_ID::Create();