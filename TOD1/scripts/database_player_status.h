class database_player_status : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_player_status", "Node", false, false);
		
		//	TODO: add members and methods to database_player_status.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_player_status::Create();