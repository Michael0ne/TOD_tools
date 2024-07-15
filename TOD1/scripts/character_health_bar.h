class character_health_bar : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_health_bar", "Node", false, false);
		
		//	TODO: add members and methods to character_health_bar.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_health_bar::Create();