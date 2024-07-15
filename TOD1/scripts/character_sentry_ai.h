class character_sentry_ai : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_sentry_ai", "Node", false, false);
		
		//	TODO: add members and methods to character_sentry_ai.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_sentry_ai::Create();