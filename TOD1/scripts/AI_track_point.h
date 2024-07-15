class AI_track_point : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("AI_track_point", "Node", false, false);
		
		//	TODO: add members and methods to AI_track_point.
		
		inst->CalculateSize();
		
		return inst;
	};
};

AI_track_point::Create();