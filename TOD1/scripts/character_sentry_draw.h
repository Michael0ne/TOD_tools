class character_sentry_draw : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_sentry_draw", "Node", false, false);
		
		//	TODO: add members and methods to character_sentry_draw.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_sentry_draw::Create();