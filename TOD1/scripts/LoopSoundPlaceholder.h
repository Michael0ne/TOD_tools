class LoopSoundPlaceholder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("LoopSoundPlaceholder", "Node", false, false);
		
		//	TODO: add members and methods to LoopSoundPlaceholder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

LoopSoundPlaceholder::Create();