class Soundtrigger : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Soundtrigger", "Node", false, false);
		
		//	TODO: add members and methods to Soundtrigger.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Soundtrigger::Create();