class AI_track : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("AI_track", "Node", false, false);
		
		//	TODO: add members and methods to AI_track.
		
		inst->CalculateSize();
		
		return inst;
	};
};

AI_track::Create();