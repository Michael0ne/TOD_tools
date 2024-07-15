class Soundtrigger_advanced : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Soundtrigger_advanced", "Node", false, false);
		
		//	TODO: add members and methods to Soundtrigger_advanced.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Soundtrigger_advanced::Create();