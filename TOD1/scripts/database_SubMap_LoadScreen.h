class database_SubMap_LoadScreen : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_SubMap_LoadScreen", "Node", false, false);
		
		//	TODO: add members and methods to database_SubMap_LoadScreen.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_SubMap_LoadScreen::Create();