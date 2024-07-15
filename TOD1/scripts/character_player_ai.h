class character_player_ai : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_player_ai", "Node", false, false);
		
		//	TODO: add members and methods to character_player_ai.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_player_ai::Create();