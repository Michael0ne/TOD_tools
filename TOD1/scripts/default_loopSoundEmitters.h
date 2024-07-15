class default_loopSoundEmitters : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("default_loopSoundEmitters", "Node", false, false);
		
		//	TODO: add members and methods to default_loopSoundEmitters.
		
		inst->CalculateSize();
		
		return inst;
	};
};

default_loopSoundEmitters::Create();